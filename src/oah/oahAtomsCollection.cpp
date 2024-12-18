/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <map>
#include <regex>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"
#include "mfTest.h"

#include "oahOah.h"
#include "oahWae.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "oahBrowsers.h"

#include "mfLibraryComponent.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_oahValueLessAtomAlias oahValueLessAtomAlias::create (
  const std::string&        shortName,
  const std::string&        longName,
  const std::string&        description,
  const S_oahValueLessAtom& originalValueLessAtom)
{
  oahValueLessAtomAlias* obj = new
    oahValueLessAtomAlias (
      longName,
      shortName,
      description,
      originalValueLessAtom);
  assert (obj != nullptr);
  return obj;
}

oahValueLessAtomAlias::oahValueLessAtomAlias (
  const std::string&        shortName,
  const std::string&        longName,
  const std::string&        description,
  const S_oahValueLessAtom& originalValueLessAtom)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{
  fOriginalValueLessAtom = originalValueLessAtom;
}

oahValueLessAtomAlias::~oahValueLessAtomAlias ()
{}

void oahValueLessAtomAlias::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahValueLessAtomAlias";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI ??? v0.9.66

  fSelected = true;
}

void oahValueLessAtomAlias::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtomAlias::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueLessAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahValueLessAtomAlias>*> (v)) {
        S_oahValueLessAtomAlias elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueLessAtomAlias::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahValueLessAtomAlias::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtomAlias::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueLessAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahValueLessAtomAlias>*> (v)) {
        S_oahValueLessAtomAlias elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueLessAtomAlias::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahValueLessAtomAlias::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtomAlias::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOriginalValueLessAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalValueLessAtom);
  }
}

void oahValueLessAtomAlias::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahValueLessAtomAlias::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  fOriginalValueLessAtom->
    displayAtomWithVariableOptionsValues (
      os,
      valueFieldWidth);
}

std::ostream& operator << (std::ostream& os, const S_oahValueLessAtomAlias& elt)
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
S_oahValueFittedAtomAlias oahValueFittedAtomAlias::create (
  const std::string&          shortName,
  const std::string&          longName,
  const std::string&          description,
  const S_oahValueFittedAtom& originalValueFittedAtom)
{
  oahValueFittedAtomAlias* obj = new
    oahValueFittedAtomAlias (
      longName,
      shortName,
      description,
      originalValueFittedAtom);
  assert (obj != nullptr);
  return obj;
}

oahValueFittedAtomAlias::oahValueFittedAtomAlias (
  const std::string&          shortName,
  const std::string&          longName,
  const std::string&          description,
  const S_oahValueFittedAtom& originalValueFittedAtom)
  : oahValueFittedAtom (
      longName,
      shortName,
      description)
{
  fOriginalValueFittedAtom = originalValueFittedAtom;
}

oahValueFittedAtomAlias::~oahValueFittedAtomAlias ()
{}

void oahValueFittedAtomAlias::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOnOffAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
  // JMI ??? v0.9.66
}

void oahValueFittedAtomAlias::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtomAlias::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueFittedAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahValueFittedAtomAlias>*> (v)) {
        S_oahValueFittedAtomAlias elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueFittedAtomAlias::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahValueFittedAtomAlias::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtomAlias::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueFittedAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahValueFittedAtomAlias>*> (v)) {
        S_oahValueFittedAtomAlias elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueFittedAtomAlias::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahValueFittedAtomAlias::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtomAlias::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOriginalValueFittedAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalValueFittedAtom);
  }
}

void oahValueFittedAtomAlias::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahValueFittedAtomAlias::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  fOriginalValueFittedAtom->
    displayAtomWithVariableOptionsValues (
      os,
      valueFieldWidth);
}

std::ostream& operator << (std::ostream& os, const S_oahValueFittedAtomAlias& elt)
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
S_oahMacroAtom oahMacroAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahMacroAtom* obj = new
    oahMacroAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}

oahMacroAtom::oahMacroAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{}

oahMacroAtom::~oahMacroAtom ()
{}

void oahMacroAtom::appendValueLessAtomToMacro (
  S_oahValueLessAtom atomNotExpectingAValue)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    atomNotExpectingAValue != nullptr,
    "atomNotExpectingAValue is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fMacroValueLessAtomsList.push_back (atomNotExpectingAValue);
}

void oahMacroAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahMacroAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    S_oahValueLessAtom atomNotExpectingAValue : fMacroValueLessAtomsList
  ) {
    atomNotExpectingAValue->
      applyValueLessAtom (os);
  } // for

  fSelected = true;
}

Bool oahMacroAtom::atomMatchesString (
	const std::string& lowerCaseString,
	std::ostream&      os) const
{
  Bool result;

	gLog <<
		"=======>>> oahMacroAtom::atomMatchesString ()" <<
		", lowerCaseString: " << lowerCaseString <<
		std::endl;
	gLog << "THIS: " << std::endl;
	printHelp (os);
	gLog << std::endl;

  if (elementMatchesString (lowerCaseString)) {
    result = true;
  }

	for (S_oahValueLessAtom valueLessAtom : fMacroValueLessAtomsList) {
		if (valueLessAtom->elementMatchesString (lowerCaseString)) {
			result = true;
		}
	} // for

	if (result) {
    // append this matching element to foundElementsList
//     foundElementsList.push_back (this); // JMI v0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         fetchNames (),
//         fDescription,
//         containingFindableElementAsString ()));
	}

	gLog <<
		"<<<<====== oahMacroAtom::atomMatchesString ()" <<
		", result: " << result <<
		std::endl;

  return result;
}

void oahMacroAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMacroAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMacroAtom>*
    p =
      dynamic_cast<visitor<S_oahMacroAtom>*> (v)) {
        S_oahMacroAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMacroAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahMacroAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMacroAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMacroAtom>*
    p =
      dynamic_cast<visitor<S_oahMacroAtom>*> (v)) {
        S_oahMacroAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMacroAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahMacroAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMacroAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_oahValueLessAtom valueLessAtom : fMacroValueLessAtomsList) {
    // browse the valueLessAtom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*valueLessAtom);
  } // for
}

void oahMacroAtom::printFull (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  for (S_oahValueLessAtom valueLessAtom : fMacroValueLessAtomsList) {
    os <<
      "valueLessAtom:" <<
      std::endl;
    ++gIndenter;
    os <<
      valueLessAtom;
    --gIndenter;
    os << std::endl;
  } // for

  --gIndenter;
}

void oahMacroAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom: ";

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  std::list <S_oahValueLessAtom>::const_iterator
    iBegin = fMacroValueLessAtomsList.begin (),
    iEnd   = fMacroValueLessAtomsList.end (),
    i      = iBegin;

  for ( ; ; ) {
    S_oahAtom valueLessAtom = (*i);

    os <<
      valueLessAtom->fetchNames ();

    if (++i == iEnd) break;
    os << ", ";
  } // for
}

void oahMacroAtom::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    ":" <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);
  }

  os <<
    "This macro option is equivalent to: ";

  if (! fMacroValueLessAtomsList.size ()) {
    os <<
      "[NONE]" << // JMI
      std::endl;
  }

  else {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahValueLessAtom>::const_iterator
      iBegin = fMacroValueLessAtomsList.begin (),
      iEnd   = fMacroValueLessAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom valueLessAtom = (*i);

      os <<
        valueLessAtom-> fetchNames () <<
        ":" <<
        std::endl;

      ++gIndenter;

      gIndenter.indentMultiLineString (
        valueLessAtom-> getDescription (),
        os);

      --gIndenter;

      if (++i == iEnd) break;
//      os << std::endl;
    } // for

    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  std::list <S_oahValueLessAtom>::const_iterator
    iBegin = fMacroValueLessAtomsList.begin (),
    iEnd   = fMacroValueLessAtomsList.end (),
    i      = iBegin;

  for ( ; ; ) {
    S_oahAtom valueLessAtom = (*i);

    os <<
      valueLessAtom->fetchNames ();

    if (++i == iEnd) break;
    os << ", ";
  } // for
}

void oahMacroAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  oahElement::printOahElementEssentials (
    os, valueFieldWidth);

  ++gIndenter;

  for (S_oahAtom atom : fMacroValueLessAtomsList) {
    atom->
      displayAtomWithVariableOptionsValues (
        os,
        valueFieldWidth);
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahMacroAtom& elt)
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
S_oahOptionsUsageAtom oahOptionsUsageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahOptionsUsageAtom* obj = new
    oahOptionsUsageAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahOptionsUsageAtom::oahOptionsUsageAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahOptionsUsageAtom::~oahOptionsUsageAtom ()
{}

void oahOptionsUsageAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionsUsageAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  printOptionsUsage (os);

  fSelected = true;
}

void oahOptionsUsageAtom::printOptionsUsage (std::ostream& os) const
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    fHelpValueLessAtomServiceName <<
    " options usage:" <<
    std::endl;

  ++gIndenter;

//   gIndenter.indentMultiLineString (
//     foundString,
//     os);
  gIndenter.indentMultiLineString (
    regex_replace (
      regex_replace (
R"(In EXECUTABLE_NAME, '-' as an argument, represents standard input.

Most options have a short and a long name for commodity.
The long name may be empty if the short name is explicit enough.

The options are organized in a group-subgroup-atom hierarchy.
Help can be obtained for groups or subgroups at will,
as well as for any option with the OPTION_NAME_HELP_NAMES option.

A subgroup can be showm as a header only, in which case its description is printed
only when the corresponding short or long names are used.

Both '-' and '--' can be used to introduce options,
even though the help facility only shows them with '-'.

There are some prefixes to allow for shortcuts,
such as '-t=voices,meas' for '-tvoices, -tmeas'.

The options can be placed in any order,
provided the values immediately follow the atoms that need them.

Using options that attempt to create files, such as '-output-file-name, -o',
leads to an error if the environment is read-only access,
as is the case of https://libmusicxml.grame.fr .)",
        std::regex ("OPTION_NAME_HELP_NAMES"),
        gOahOahGroup->
          getHelpAboutOptionName ()->
            fetchNamesBetweenQuotes ()
        ),
      std::regex ("EXECUTABLE_NAME"),
      fHelpValueLessAtomServiceName
      ),
    os);

  --gIndenter;

  gIndenter.setIndentation (saveIndent);
}

void oahOptionsUsageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOptionsUsageAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahOptionsUsageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOptionsUsageAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahOptionsUsageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOptionsUsageAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahOptionsUsageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUsageAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahOptionsUsageAtom& elt)
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
S_oahMusicFormatsTestAtom oahMusicFormatsTestAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahMusicFormatsTestAtom* obj = new
    oahMusicFormatsTestAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahMusicFormatsTestAtom::oahMusicFormatsTestAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahMusicFormatsTestAtom::~oahMusicFormatsTestAtom ()
{}

void oahMusicFormatsTestAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahMusicFormatsTestAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfTest (os);

  fSelected = true;
}

void oahMusicFormatsTestAtom::printOptionsMfTest (std::ostream& os) const
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    fHelpValueLessAtomServiceName <<
    " MfTest:" <<
    std::endl;

  ++gIndenter;

//   gIndenter.indentMultiLineString (
//     foundString,
//     os);
  gIndenter.indentMultiLineString (
R"(This option is meant for developpers/maintainer.
It executes function mfTest() in src/mflibrary/mftest.cpp,
in which they can do anything they need for their purposes.
Such code is not supposed to be long-term living.)",
			os);

  --gIndenter;

  gIndenter.setIndentation (saveIndent);
}

void oahMusicFormatsTestAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMusicFormatsTestAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMusicFormatsTestAtom>*
    p =
      dynamic_cast<visitor<S_oahMusicFormatsTestAtom>*> (v)) {
        S_oahMusicFormatsTestAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMusicFormatsTestAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahMusicFormatsTestAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMusicFormatsTestAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMusicFormatsTestAtom>*
    p =
      dynamic_cast<visitor<S_oahMusicFormatsTestAtom>*> (v)) {
        S_oahMusicFormatsTestAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMusicFormatsTestAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahMusicFormatsTestAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMusicFormatsTestAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahMusicFormatsTestAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsMusicFormatsTestAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahMusicFormatsTestAtom& elt)
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
S_oahHelpAtom oahHelpAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahHelpAtom* obj = new
    oahHelpAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahHelpAtom::oahHelpAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpAtom::~oahHelpAtom ()
{}

void oahHelpAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  fetchAtomUpLinkToHandler ()->
    printHelp (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahHelpAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahHelpAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahHelpAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahHelpAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahHelpAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpAtom::printOptionsSummary (std::ostream& os) const
{
  os <<
    gOahOahGroup->getOahOahGroupServiceName () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahHelpAtom& elt)
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
S_oahHelpSummaryAtom oahHelpSummaryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahHelpSummaryAtom* obj = new
    oahHelpSummaryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahHelpSummaryAtom::oahHelpSummaryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpSummaryAtom::~oahHelpSummaryAtom ()
{}

void oahHelpSummaryAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpSummaryAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  fetchAtomUpLinkToHandler ()->
    printOptionsSummary (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahHelpSummaryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpSummaryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahHelpSummaryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpSummaryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahHelpSummaryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpSummaryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahHelpSummaryAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsSummaryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpSummaryAtom::printOptionsSummary (std::ostream& os) const
{
  os <<
    gOahOahGroup->getOahOahGroupServiceName () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahHelpSummaryAtom& elt)
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
S_oahAboutAtom oahAboutAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahAboutAtom* obj = new
    oahAboutAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahAboutAtom::oahAboutAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahAboutAtom::~oahAboutAtom ()
{}

void oahAboutAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahAboutAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printAbout (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahAboutAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAboutAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAboutAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahAboutAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAboutAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAboutAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahAboutAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAboutAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahAboutAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAboutAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAboutAtom::printAbout (std::ostream& os) const
{
  os <<
    fetchAtomUpLinkToHandler ()->
      handlerServiceAboutInformation () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAboutAtom& elt)
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
std::string oahVersionKindAsString (
  oahVersionKind versionKind)
{
  std::string result;

  switch (versionKind) {
    case oahVersionKind::kOahVersion:
      result = "kOahVersion";
      break;
    case oahVersionKind::kOahVersionFull:
      result = "kOahVersionFull";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahVersionKind& elt)
{
  os << oahVersionKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_oahVersionAtom oahVersionAtom::create (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  const std::string&  serviceName,
  oahVersionKind      versionKind)
{
  oahVersionAtom* obj = new
    oahVersionAtom (
      longName,
      shortName,
      description,
      serviceName,
      versionKind);
  assert (obj != nullptr);
  return obj;
}

oahVersionAtom::oahVersionAtom (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  const std::string&  serviceName,
  oahVersionKind      versionKind)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{
  fVersionKind = versionKind;
}

oahVersionAtom::~oahVersionAtom ()
{}

void oahVersionAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahVersionAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  switch (fVersionKind) {
    case oahVersionKind::kOahVersion:
      printVersionShort (os);
      break;
    case oahVersionKind::kOahVersionFull:
      printVersionFull (os);
      break;
  } // switch

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahVersionAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahVersionAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahVersionAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahVersionAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahVersionAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahVersionAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahVersionAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahVersionAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahVersionAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahVersionAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahVersionAtom::printVersionShort (std::ostream& os) const
{
  // get the handler version
  S_mfcMultiComponent
    handlerMultiComponent =
      fetchAtomUpLinkToHandler ()->
        getHandlerMultiComponent ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  handlerMultiComponent->
    printVersionShort (os);
}

void oahVersionAtom::printVersionFull (std::ostream& os) const
{
  // get the handler version
  S_mfcMultiComponent
    handlerMultiComponent =
      fetchAtomUpLinkToHandler ()->
        getHandlerMultiComponent ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  handlerMultiComponent->
    printVersionFull (os);
}

std::ostream& operator << (std::ostream& os, const S_oahVersionAtom& elt)
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
S_oahLibraryVersionAtom oahLibraryVersionAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahLibraryVersionAtom* obj = new
    oahLibraryVersionAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahLibraryVersionAtom::oahLibraryVersionAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryVersionAtom::~oahLibraryVersionAtom ()
{}

void oahLibraryVersionAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahLibraryVersionAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printVersion (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahLibraryVersionAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryVersionAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLibraryVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryVersionAtom>*> (v)) {
        S_oahLibraryVersionAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLibraryVersionAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahLibraryVersionAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryVersionAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLibraryVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryVersionAtom>*> (v)) {
        S_oahLibraryVersionAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLibraryVersionAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahLibraryVersionAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryVersionAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahLibraryVersionAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahLibraryVersionAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahLibraryVersionAtom::printVersion (std::ostream& os) const
{
  // get the handler version
  S_mfcMultiComponent
    handlerMultiComponent =
      fetchAtomUpLinkToHandler ()->
        getHandlerMultiComponent ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  createLibraryComponent ()->
    printVersion (os);
}

std::ostream& operator << (std::ostream& os, const S_oahLibraryVersionAtom& elt)
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
S_oahHistoryAtom oahHistoryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahHistoryAtom* obj = new
    oahHistoryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahHistoryAtom::oahHistoryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHistoryAtom::~oahHistoryAtom ()
{}

void oahHistoryAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahHistoryAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printHistory (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahHistoryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHistoryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahHistoryAtom>*> (v)) {
        S_oahHistoryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHistoryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahHistoryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHistoryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahHistoryAtom>*> (v)) {
        S_oahHistoryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHistoryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahHistoryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHistoryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahHistoryAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahHistoryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHistoryAtom::printHistory (std::ostream& os) const
{
  // get the handler history
  S_mfcMultiComponent
    handlerMultiComponent =
      fetchAtomUpLinkToHandler ()->
        getHandlerMultiComponent ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  handlerMultiComponent->
    printHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_oahHistoryAtom& elt)
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
S_oahLibraryHistoryAtom oahLibraryHistoryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahLibraryHistoryAtom* obj = new
    oahLibraryHistoryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahLibraryHistoryAtom::oahLibraryHistoryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryHistoryAtom::~oahLibraryHistoryAtom ()
{}

void oahLibraryHistoryAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahLibraryHistoryAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printHistory (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahLibraryHistoryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryHistoryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLibraryHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryHistoryAtom>*> (v)) {
        S_oahLibraryHistoryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLibraryHistoryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahLibraryHistoryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryHistoryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLibraryHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryHistoryAtom>*> (v)) {
        S_oahLibraryHistoryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLibraryHistoryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahLibraryHistoryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLibraryHistoryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahLibraryHistoryAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahLibraryHistoryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahLibraryHistoryAtom::printHistory (std::ostream& os) const
{
  // get the handler history
  S_mfcMultiComponent
    handlerMultiComponent =
      fetchAtomUpLinkToHandler ()->
        getHandlerMultiComponent ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  createLibraryComponent ()->
    printHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_oahLibraryHistoryAtom& elt)
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
S_oahContactAtom oahContactAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahContactAtom* obj = new
    oahContactAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahContactAtom::oahContactAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}


oahContactAtom::~oahContactAtom ()
{}

void oahContactAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahContactAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printContact (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahContactAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahContactAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahContactAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahContactAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahContactAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahContactAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahContactAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahContactAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahContactAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahContactAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahContactAtom::printContact (std::ostream& os) const
{
  gIndenter.indentMultiLineString (
    regex_replace (
R"(To contact the maintainers of EXECUTABLE_NAME:
  Create an issue at https://github.com/jacques-menu/musicformats,
  describing the problem and any error messages you get if relevant.
  You should sign up for GitHub for that.)",
        std::regex ("EXECUTABLE_NAME"),
        fHelpValueLessAtomServiceName),
    os);
}

std::ostream& operator << (std::ostream& os, const S_oahContactAtom& elt)
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
S_oahDisplayPrefixes oahDisplayPrefixes::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahDisplayPrefixes* obj = new
    oahDisplayPrefixes (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahDisplayPrefixes::oahDisplayPrefixes (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplayPrefixes::~oahDisplayPrefixes ()
{}

void oahDisplayPrefixes::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahDisplayPrefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printPrefixes (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahDisplayPrefixes::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayPrefixes::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayPrefixes>*
    p =
      dynamic_cast<visitor<S_oahDisplayPrefixes>*> (v)) {
        S_oahDisplayPrefixes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayPrefixes::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahDisplayPrefixes::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayPrefixes::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayPrefixes>*
    p =
      dynamic_cast<visitor<S_oahDisplayPrefixes>*> (v)) {
        S_oahDisplayPrefixes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayPrefixes::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahDisplayPrefixes::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayPrefixes::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahDisplayPrefixes::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayPrefixes:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahDisplayPrefixes::printPrefixes (std::ostream& os) const
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

  handler->
    printKnownPrefixes (os);
}

std::ostream& operator << (std::ostream& os, const S_oahDisplayPrefixes& elt)
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
S_oahDisplaySingleCharacterOptions oahDisplaySingleCharacterOptions::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahDisplaySingleCharacterOptions* obj = new
    oahDisplaySingleCharacterOptions (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahDisplaySingleCharacterOptions::oahDisplaySingleCharacterOptions (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplaySingleCharacterOptions::~oahDisplaySingleCharacterOptions ()
{}

void oahDisplaySingleCharacterOptions::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a oahDisplaySingleCharacterOptions";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printSingleCharacterOptions (os);

  gIndenter.setIndentation (saveIndent);

  fSelected = true;
}

void oahDisplaySingleCharacterOptions::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplaySingleCharacterOptions>*
    p =
      dynamic_cast<visitor<S_oahDisplaySingleCharacterOptions>*> (v)) {
        S_oahDisplaySingleCharacterOptions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplaySingleCharacterOptions::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahDisplaySingleCharacterOptions::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplaySingleCharacterOptions>*
    p =
      dynamic_cast<visitor<S_oahDisplaySingleCharacterOptions>*> (v)) {
        S_oahDisplaySingleCharacterOptions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplaySingleCharacterOptions::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahDisplaySingleCharacterOptions::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahDisplaySingleCharacterOptions::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplaySingleCharacterOptions:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahDisplaySingleCharacterOptions::printSingleCharacterOptions (std::ostream& os) const
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

  handler->
    printKnownSingleCharacterOptions (os);
}

std::ostream& operator << (std::ostream& os, const S_oahDisplaySingleCharacterOptions& elt)
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
S_oahOnOffAtom oahOnOffAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfOnOffKind&       onOffKindVariable)
{
  oahOnOffAtom* obj = new
    oahOnOffAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      onOffKindVariable);
  assert (obj != nullptr);
  return obj;
}

oahOnOffAtom::oahOnOffAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfOnOffKind&       onOffKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fOnOffKindVariable (
      onOffKindVariable)
{}

oahOnOffAtom::~oahOnOffAtom ()
{}

void oahOnOffAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOnOffAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (theString == "on") {
    setOnOffKindVariable (
      mfOnOffKind::kMfOnOffOn);
  }
  else if (theString == "off") {
    setOnOffKindVariable (
      mfOnOffKind::kMfOnOffOn);
  }
  else if (theString == "unknown") {
    setOnOffKindVariable (
      mfOnOffKind::kMfOnOffUnknown);
  }
  else {
    std::stringstream ss;

    ss <<
      "Ill-formed value for option " <<
      fetchNamesBetweenQuotes () <<
      ", \"on\", \"off\" or \"unknown\" expected";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahOnOffAtom::setOnOffKindVariable (mfOnOffKind value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    if (fLongName == K_TRACE_OAH_OPTION_LONG_NAME) {
      // get the options handler
      S_oahHandler
        handler =
          fetchAtomUpLinkToHandler (); // JMI

      gLog <<
        "Setting option '" <<
        fetchNames () <<
        "' onOffKind variable to '" <<
        value <<
        '\'' <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fOnOffKindVariable = value;

  fSelected = true;
}

void oahOnOffAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOnOffAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOnOffAtom>*
    p =
      dynamic_cast<visitor<S_oahOnOffAtom>*> (v)) {
        S_oahOnOffAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahOnOffAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahOnOffAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOnOffAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOnOffAtom>*
    p =
      dynamic_cast<visitor<S_oahOnOffAtom>*> (v)) {
        S_oahOnOffAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahOnOffAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahOnOffAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahOnOffAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahOnOffAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OnOffAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << ": " <<
    fValueSpecification <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fOnOffKindVariable" << ": " <<
    fOnOffKindVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSelected" << ": " <<
    fSelected <<
    std::endl;

  --gIndenter;
}

void oahOnOffAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fOnOffKindVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahOnOffAtom& elt)
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
S_oahBooleanAtom oahBooleanAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
{
  oahBooleanAtom* obj = new
    oahBooleanAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable);
  assert (obj != nullptr);
  return obj;
}

oahBooleanAtom::oahBooleanAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
  : oahValueLessAtom (
      longName,
      shortName,
      description),
    fVariableName (
      variableName),
    fBooleanVariable (
      booleanVariable)
{}

oahBooleanAtom::~oahBooleanAtom ()
{}

void oahBooleanAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahBooleanAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setBooleanVariable (true);
}

void oahBooleanAtom::setBooleanVariable (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    if (fLongName == K_TRACE_OAH_OPTION_LONG_NAME) {
      // get the options handler
      S_oahHandler
        handler =
          fetchAtomUpLinkToHandler (); // JMI

      gLog <<
        "Setting option '" <<
        fetchNames () <<
        "' boolean variable to '" <<
        value <<
        '\'' <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fBooleanVariable = value;

  fSelected = true;
}

void oahBooleanAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahBooleanAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahBooleanAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahBooleanAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahBooleanAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahBooleanAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahBooleanAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahBooleanAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahBooleanAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "BooleanAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanVariable" << ": " <<
    fBooleanVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSelected" << ": " <<
    fSelected <<
    std::endl;

  --gIndenter;
}

void oahBooleanAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahBooleanAtom& elt)
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
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom)
{
  oahTwoBooleansAtom* obj = new
    oahTwoBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom);
  assert (obj != nullptr);
  return obj;
}

oahTwoBooleansAtom::oahTwoBooleansAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom)
  : oahBooleanAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable),
    fSecondBooleanAtom (
      secondBooleanAtom)
{}

oahTwoBooleansAtom::~oahTwoBooleansAtom ()
{}

void oahTwoBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahTwoBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setTwoBooleansVariables (true);
}

void oahTwoBooleansAtom::setTwoBooleansVariables (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' two booleans variables to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setBooleanVariable (value);

  fSecondBooleanAtom->setBooleanVariable (value);
}

void oahTwoBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahTwoBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahTwoBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahTwoBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "TwoBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    std::setw (fieldWidth) <<
    "fDescription" << ": " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    fDescription,
    os);

  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "booleanVariable" << ": " <<
    fBooleanVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "secondBooleanAtom" << ": " <<
    fSecondBooleanAtom <<
    std::endl;

  --gIndenter;
}

void oahTwoBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahTwoBooleansAtom& elt)
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
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom)
{
  oahThreeBooleansAtom* obj = new
    oahThreeBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom);
  assert (obj != nullptr);
  return obj;
}

oahThreeBooleansAtom::oahThreeBooleansAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom)
  : oahTwoBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom),
    fThirdBooleanAtom (
      thirdBooleanAtom)
{}

oahThreeBooleansAtom::~oahThreeBooleansAtom ()
{}

void oahThreeBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahThreeBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setThreeBooleansVariables (true);
}

void oahThreeBooleansAtom::setThreeBooleansVariables (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' three booleans variables to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setTwoBooleansVariables (value);

  fThirdBooleanAtom->setBooleanVariable (value);
}

void oahThreeBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahThreeBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahThreeBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahThreeBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahThreeBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahThreeBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahThreeBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "ThreeBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    std::setw (fieldWidth) <<
    "fDescription" << ": " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    fDescription,
    os);

  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanVariable" << ": " <<
    fBooleanVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSecondBooleanAtom" << ": " <<
    fSecondBooleanAtom <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fThirdBooleanAtom" << ": " <<
    fThirdBooleanAtom <<
    std::endl;

  --gIndenter;
}

void oahThreeBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

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

std::ostream& operator << (std::ostream& os, const S_oahThreeBooleansAtom& elt)
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
S_oahFourBooleansAtom oahFourBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom,
  const S_oahBooleanAtom& fourthBooleanAtom)
{
  oahFourBooleansAtom* obj = new
    oahFourBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom,
      fourthBooleanAtom);
  assert (obj != nullptr);
  return obj;
}

oahFourBooleansAtom::oahFourBooleansAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom,
  const S_oahBooleanAtom& fourthBooleanAtom)
  : oahThreeBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom),
    fFourthBooleanAtom (
      fourthBooleanAtom)
{}

oahFourBooleansAtom::~oahFourBooleansAtom ()
{}

void oahFourBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFourBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setFourBooleansVariables (true);
}

void oahFourBooleansAtom::setFourBooleansVariables (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' four booleans variables to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setThreeBooleansVariables (value);

  fFourthBooleanAtom->setBooleanVariable (value);
}

void oahFourBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFourBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFourBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahFourBooleansAtom>*> (v)) {
        S_oahFourBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFourBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahFourBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFourBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFourBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahFourBooleansAtom>*> (v)) {
        S_oahFourBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFourBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahFourBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFourBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahFourBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FourBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    std::setw (fieldWidth) <<
    "fDescription" << ": " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    fDescription,
    os);

  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanVariable" << ": " <<
    fBooleanVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSecondBooleanAtom" << ": " <<
    fSecondBooleanAtom <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fThirdBooleanAtom" << ": " <<
    fThirdBooleanAtom <<
    "fFourthBooleanAtom" << ": " <<
    fFourthBooleanAtom <<
    std::endl;

  --gIndenter;
}

void oahFourBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

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

std::ostream& operator << (std::ostream& os, const S_oahFourBooleansAtom& elt)
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
S_oahFiveBooleansAtom oahFiveBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom,
  const S_oahBooleanAtom& fourthBooleanAtom,
  const S_oahBooleanAtom& fifthBooleanAtom)
{
  oahFiveBooleansAtom* obj = new
    oahFiveBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom,
      fourthBooleanAtom,
      fifthBooleanAtom);
  assert (obj != nullptr);
  return obj;
}

oahFiveBooleansAtom::oahFiveBooleansAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom,
  const S_oahBooleanAtom& fourthBooleanAtom,
  const S_oahBooleanAtom& fifthBooleanAtom)
  : oahFourBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom,
      fourthBooleanAtom)
{
  fFifthBooleanAtom = fifthBooleanAtom;
}

oahFiveBooleansAtom::~oahFiveBooleansAtom ()
{}

void oahFiveBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFiveBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setFiveBooleansVariables (true);
}

void oahFiveBooleansAtom::setFiveBooleansVariables (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' five booleans variables to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setFourBooleansVariables (value);

  fFifthBooleanAtom->setBooleanVariable (value);
}

void oahFiveBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFiveBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFiveBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahFiveBooleansAtom>*> (v)) {
        S_oahFiveBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFiveBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahFiveBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFiveBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFiveBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahFiveBooleansAtom>*> (v)) {
        S_oahFiveBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFiveBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahFiveBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFiveBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahFiveBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FiveBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    std::setw (fieldWidth) <<
    "fDescription" << ": " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    fDescription,
    os);

  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanVariable" << ": " <<
    fBooleanVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSecondBooleanAtom" << ": " <<
    fSecondBooleanAtom <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fThirdBooleanAtom" << ": " <<
    fThirdBooleanAtom <<
    "fFourthBooleanAtom" << ": " <<
    fFourthBooleanAtom <<
    "fFifthBooleanAtom" << ": " <<
    fFifthBooleanAtom <<
    std::endl;

  --gIndenter;
}

void oahFiveBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

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

std::ostream& operator << (std::ostream& os, const S_oahFiveBooleansAtom& elt)
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
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahCombinedBooleansAtom* obj = new
    oahCombinedBooleansAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}

oahCombinedBooleansAtom::oahCombinedBooleansAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::addBooleanAtom (
  const S_oahBooleanAtom& booleanAtom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    booleanAtom != nullptr,
    "booleanAtom is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBooleanAtomsList.push_back (
    booleanAtom);
}

void oahCombinedBooleansAtom::addBooleanAtomByName (
  const std::string& name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    handler->
      unknownOptionNameError (
        name,
        "in combined booleans atom");
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // handle the atom
      fBooleanAtomsList.push_back (atom);
    }

    else {
      handler->
        unknownOptionNameError (
          name,
          "not that of an atom");
    }
  }
}

void oahCombinedBooleansAtom::setCombinedBooleanVariables (Bool value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' combined boolean variables to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the value of the atoms in the list
  if (fBooleanAtomsList.size ()) {
    for (
      std::list <S_oahBooleanAtom>::const_iterator i =
        fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // set the boolean variable
        booleanAtom->
          setBooleanVariable (value);
      }
    } // for
  }
}

void oahCombinedBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCombinedBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setCombinedBooleanVariables (true);

  fSelected = true;
}

Bool oahCombinedBooleansAtom::atomMatchesString (
	const std::string&       lowerCaseString,
	std::ostream&            os) const
{
  Bool result;

	gLog <<
		"=======>>> oahCombinedBooleansAtom::atomMatchesString ()" <<
		", lowerCaseString: " << lowerCaseString <<
		std::endl;
	gLog << "THIS: " << std::endl;
	printHelp (os);
	gLog << std::endl;

  if (elementMatchesString (lowerCaseString)) {
    result = true;
  }

	for (S_oahBooleanAtom booleanAtom : fBooleanAtomsList) {
		if (booleanAtom->elementMatchesString (lowerCaseString)) {
			result = true;
		}
	} // for

	if (result) {
    // append this matching element to foundElementsList
//     foundElementsList.push_back (this);// JMI v0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         fetchNames (),
//         fDescription,
//         containingFindableElementAsString ()));
	}

	gLog <<
		"<<<<====== oahCombinedBooleansAtom::atomMatchesString ()" <<
		", result: " << result <<
		std::endl;

  return result;
}

void oahCombinedBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahCombinedBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahCombinedBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCombinedBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      std::list <S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahCombinedBooleansAtom::printCombinedBooleansEssentials (
  std::ostream& os,
  int fieldWidth) const
{
/* JMI
  printAtomWithVariableNameEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "valueSpecification" << ": " <<
    fValueSpecification <<
    std::endl;
    */
}

void oahCombinedBooleansAtom::printCombinedBooleansEssentialsFull (
  std::ostream& os,
  int fieldWidth) const
{
/* JMI
  printAtomWithVariableNameEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "valueSpecification" << ": " <<
    fValueSpecification <<
    std::endl;
    */
}

void oahCombinedBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "CombinedBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanAtomsList" << ": ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    os << std::endl;

    ++gIndenter;

    os << '\'';

    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << '\'';

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

void oahCombinedBooleansAtom::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    ":" <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);
  }

  os <<
    "This combined option is equivalent to: ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "[NONE]" << // JMI
      std::endl;
  }

  else {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahBooleanAtom booleanAtom = (*i);

      os <<
        booleanAtom-> fetchNames () <<
        ":" <<
        std::endl;

      ++gIndenter;

      gIndenter.indentMultiLineString (
        booleanAtom-> getDescription (),
        os);

      --gIndenter;

      if (++i == iEnd) break;
//      os << std::endl;
    } // for

    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os << std::left <<
    "fBooleanAtomsList" << ": ";

  if (! fBooleanAtomsList.size ()) {
    os << "[EMPTY]";
  }
  else {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    --gIndenter;
  }
  os << std::endl;
}

void oahCombinedBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  int fieldWidth =
    valueFieldWidth - gIndenter.getIndentation () + 1;

  ++gIndenter; // only now

  if (! fBooleanAtomsList.size ()) {
    os <<
      "[NONE]" << // JMI
      std::endl;
  }

  else {
    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom
        atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // print the boolean value
        booleanAtom->
          displayAtomWithVariableOptionsValues (
            os, fieldWidth);
      }

      if (++i == iEnd) break;

  // JMI    os << std::endl;
    } // for
  }

  --gIndenter;

}

std::ostream& operator << (std::ostream& os, const S_oahCombinedBooleansAtom& elt)
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
S_oahIntegerAtom oahIntegerAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable)
{
  oahIntegerAtom* obj = new
    oahIntegerAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      integerVariable);
  assert (obj != nullptr);
  return obj;
}

oahIntegerAtom::oahIntegerAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fIntegerVariable (
      integerVariable)
{}

oahIntegerAtom::~oahIntegerAtom ()
{}

void oahIntegerAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the integer value

  // check whether it is well-formed
  std::string regularExpression (
    "([[:digit:]]+)");

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream ss;
      ss << theString;
      ss >> integerValue;
    }

    setIntegerVariable (integerValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "integer value \"" << theString <<
      "\" for option \"" << fetchNames () <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahIntegerAtom::setIntegerVariable (int value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fIntegerVariable = value;

  fSelected = true;
}

void oahIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntegerAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIntegerAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntegerAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIntegerAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahIntegerAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntegerAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahIntegerAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' << fIntegerVariable;

  return ss.str ();
}

std::string oahIntegerAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' << fIntegerVariable;

  return ss.str ();
}

void oahIntegerAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "integerVariable" << ": " <<
    fIntegerVariable <<
    std::endl;

  --gIndenter;
}

void oahIntegerAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fIntegerVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahIntegerAtom& elt)
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
S_oahTwoIntegersAtom oahTwoIntegersAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable,
  int&               integerSecondaryVariable)
{
  oahTwoIntegersAtom* obj = new
    oahTwoIntegersAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      integerVariable,
      integerSecondaryVariable);
  assert (obj != nullptr);
  return obj;
}

oahTwoIntegersAtom::oahTwoIntegersAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable,
  int&               integerSecondaryVariable)
  : oahIntegerAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      integerVariable),
    fIntegerSecondaryVariable (
      integerSecondaryVariable)
{}

oahTwoIntegersAtom::~oahTwoIntegersAtom ()
{}

void oahTwoIntegersAtom::applyAtomWithValue ( // NOT USE YET JMI
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the two integer values

  // check whether it is well-formed
  std::string regularExpression (
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // integer secondary value
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) {
    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream ss;
      ss << sm [ 1 ];
      ss >> integerValue;
    }
    fIntegerVariable = integerValue;

    {
      std::stringstream ss;
      ss << sm [ 2 ];
      ss >> integerValue;
    }
    fIntegerSecondaryVariable = integerValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahTwoIntegersAtom::setIntegerVariable (int value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' two integers variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fIntegerVariable = value;

  fSelected = true;
}

void oahTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoIntegersAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoIntegersAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahTwoIntegersAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahTwoIntegersAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return ss.str ();
}

std::string oahTwoIntegersAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return ss.str ();
}

void oahTwoIntegersAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "integerVariable" << ": " <<
    fIntegerVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "integerSecondaryVariable" << ": " <<
    fIntegerSecondaryVariable <<
    std::endl;

  --gIndenter;
}

void oahTwoIntegersAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahTwoIntegersAtom& elt)
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
S_oahFloatAtom oahFloatAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  float&             floatVariable)
{
  oahFloatAtom* obj = new
    oahFloatAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      floatVariable);
  assert (obj != nullptr);
  return obj;
}

oahFloatAtom::oahFloatAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  float&             floatVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fFloatVariable (
      floatVariable)
{}

oahFloatAtom::~oahFloatAtom ()
{}

void oahFloatAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the float value

  // check whether it is well-formed
  std::string regularExpression (
    "([+|-]?[[:digit:]]+)(.[[:digit:]]*)?"
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for float string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) {
    // leave the low level details to the STL...
    float floatValue;
    {
      std::stringstream ss;

      ss << sm [ 0 ];
      ss >> floatValue;
    }

    setFloatVariable (floatValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "float value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahFloatAtom::setFloatVariable (float value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' float variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fFloatVariable = value;

  fSelected = true;
}

void oahFloatAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFloatAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFloatAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahFloatAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFloatAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFloatAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahFloatAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFloatAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahFloatAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' << fFloatVariable;

  return ss.str ();
}

std::string oahFloatAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' << fFloatVariable;

  return ss.str ();
}

void oahFloatAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FloatAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "floatVariable" << ": " <<
    fFloatVariable <<
    std::endl;

  --gIndenter;
}

void oahFloatAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fFloatVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahFloatAtom& elt)
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
S_oahStringAtom oahStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  oahStringAtom* obj = new
    oahStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringAtom::oahStringAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable)
{}

oahStringAtom::~oahStringAtom ()
{}

void oahStringAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  setStringVariable (theString);
}

void oahStringAtom::setStringVariable (const std::string& value)
  {
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringVariable = value;

  fSelected = true;
}

void oahStringAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << " '" << fStringVariable << '\'';

  return ss.str ();
}

std::string oahStringAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << " '" << fStringVariable << '\'';

  return ss.str ();
}

void oahStringAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl;

  --gIndenter;
}

void oahStringAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    '[' << fStringVariable << ']';

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAtom& elt)
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
S_oahFactorizedStringAtom oahFactorizedStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& atomNameDescriptor,
  const std::string& stringValueDescriptor)
{
  oahFactorizedStringAtom* obj = new
    oahFactorizedStringAtom (
      longName,
      shortName,
      description,
      atomNameDescriptor,
      stringValueDescriptor);
  assert (obj != nullptr);
  return obj;
}

oahFactorizedStringAtom::oahFactorizedStringAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& atomNameDescriptor,
  const std::string& stringValueDescriptor)
  : oahValueLessAtom (
      longName,
      shortName,
      description),
    fAtomNameDescriptor (
      atomNameDescriptor),
    fStringValueDescriptor (
      stringValueDescriptor)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
  mfAssert (
    __FILE__, __LINE__,
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
}

oahFactorizedStringAtom::~oahFactorizedStringAtom ()
{}

void oahFactorizedStringAtom::addStringAtom (
  const S_oahStringAtom& stringAtom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stringAtom != nullptr,
    "stringAtom is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // atom long name consistency check
  std::string stringAtomLongName =
    stringAtom->getLongName ();

  if (stringAtomLongName.size () == 0) {
    std::stringstream ss;

    ss <<
      "option long name \"" << stringAtomLongName <<
      "\" is empty";

    stringAtom->print (ss);

    oahError (ss.str ());
  }

  // atom short name consistency check
  std::string stringAtomShortName =
    stringAtom->getShortName ();

  if (stringAtomShortName.size () == 0) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyOahVerboseMode ()) {
      std::stringstream ss;

      ss <<
        "Option short name \"" << stringAtomShortName << "\"" <<
        " for long name \"" << stringAtomLongName << "\"" <<
        " is empty";

      oahWarning (ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
    // register this string atom's suffix in the list
    fAtomNamesList.push_back (stringAtomShortName);
  }

  // append the string atom to the list
  fStringAtomsList.push_back (
    stringAtom);

  // make this atom imvisible
  stringAtom->
    setElementVisibilityKind (
      oahElementVisibilityKind::kElementVisibilityNone);
}

void oahFactorizedStringAtom::addStringAtomByName (
  const std::string& name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    handler->
      unknownOptionNameError (
        name,
        "in factorized strings atom");
  }

  else {
    // name is known, let's handle it

    if (
      // string atom?
      S_oahStringAtom
        atom =
          dynamic_cast<oahStringAtom*>(&(*element))
      ) {
      // add the string atom
      addStringAtom (atom);
    }

    else {
      handler->
        unknownOptionNameError (
          name,
          "in factorized string atom, not the name of an atom");
    }
  }
}

void oahFactorizedStringAtom::applyValueLessAtom (std::ostream& os) // JMI v0.9.66
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Applying monoplex string atom '" <<
      fetchNames () <<
      "' which is a oahFactorizedStringAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle it at once JMI ???

  fSelected = true;
}

void oahFactorizedStringAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFactorizedStringAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFactorizedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFactorizedStringAtom>*> (v)) {
        S_oahFactorizedStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFactorizedStringAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahFactorizedStringAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFactorizedStringAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFactorizedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFactorizedStringAtom>*> (v)) {
        S_oahFactorizedStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFactorizedStringAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahFactorizedStringAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFactorizedStringAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the string atoms
  if (fStringAtomsList.size ()) {
    for (
      std::list <S_oahStringAtom>::const_iterator i = fStringAtomsList.begin ();
      i != fStringAtomsList.end ();
      ++i
    ) {
      S_oahStringAtom stringAtom = (*i);

      // browse the string atom
      oahBrowser<oahStringAtom> browser (v);
      browser.browse (*(stringAtom));
    } // for
  }
}

void oahFactorizedStringAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FactorizedStringAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    "atomNameDescriptor" << ": " <<
    fAtomNameDescriptor <<
    std::endl <<
    "stringValueDescriptor" << ": " <<
    fStringValueDescriptor <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringAtomsList" << ": ";

  if (! fStringAtomsList.size ()) {
    os << "[EMPTY]";
  }

  else {
    os << std::endl;

    ++gIndenter;

    os << '\'';

    std::list <S_oahStringAtom>::const_iterator
      iBegin = fStringAtomsList.begin (),
      iEnd   = fStringAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << '\'';

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

void oahFactorizedStringAtom::printHelp (std::ostream& os) const
{
  os <<
    '-' << fAtomNameDescriptor << ' ' << fStringValueDescriptor <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);
  }

  os <<
    "The " <<
    fAtomNamesList.size () <<
    " known " << fAtomNameDescriptor << "s are: ";

  if (! fAtomNamesList.size ()) {
    os <<
      "[NONE]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::list <std::string>::const_iterator
      iBegin = fAtomNamesList.begin (),
      iEnd   = fAtomNamesList.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      std::string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
        os << std::endl;
        cumulatedLength = 0;
        break;
      }

      os << suffix;

      if (++i == iEnd) break;
      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << std::endl;
    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahFactorizedStringAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the string atoms in the list
}

std::ostream& operator << (std::ostream& os, const S_oahFactorizedStringAtom& elt)
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
/* this class is purely virtual
S_oahDefaultedStringAtom oahDefaultedStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultStringValue)
{
  oahDefaultedStringAtom* obj = new
    oahDefaultedStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultStringValue);
  assert (obj != nullptr);
  return obj;
}
*/

oahDefaultedStringAtom::oahDefaultedStringAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultStringValue)
  : oahValueDefaultedAtom (
      longName,
      shortName,
      description,
      defaultStringValue),
    fValueSpecification (
      valueSpecification),
    fVariableName (
      variableName),
    fStringVariable (
      stringVariable)
{}

oahDefaultedStringAtom::~oahDefaultedStringAtom ()
{}

void oahDefaultedStringAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahDefaultedStringAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  setStringVariable (theString);

  fSelected = true;
}

void oahDefaultedStringAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDefaultedStringAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDefaultedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahDefaultedStringAtom>*> (v)) {
        S_oahDefaultedStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDefaultedStringAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahDefaultedStringAtom::setStringVariable (const std::string& value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringVariable = value;

  fSelected = true;
}

void oahDefaultedStringAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDefaultedStringAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDefaultedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahDefaultedStringAtom>*> (v)) {
        S_oahDefaultedStringAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDefaultedStringAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahDefaultedStringAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDefaultedStringAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahDefaultedStringAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' << fStringVariable;

  return ss.str ();
}

std::string oahDefaultedStringAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' << fStringVariable;

  return ss.str ();
}

void oahDefaultedStringAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << ": " <<
    fValueSpecification <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fDefaultStringValue" << ": " <<
    "\"" << fDefaultStringValue << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSelected" << ": " <<
    fSelected <<
    std::endl;

  --gIndenter;
}

void oahDefaultedStringAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fStringVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahDefaultedStringAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahCommonPrefixBooleansAtom oahCommonPrefixBooleansAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& shortSuffixDescriptor,
  const std::string& longSuffixDescriptor,
  const S_oahPrefix& shortNamesPrefix,
  const S_oahPrefix& longNamesPrefix)
{
  oahCommonPrefixBooleansAtom* obj = new
    oahCommonPrefixBooleansAtom (
      longName,
      shortName,
      description,
      shortSuffixDescriptor,
      longSuffixDescriptor,
      shortNamesPrefix,
      longNamesPrefix);
  assert (obj != nullptr);
  return obj;
}

oahCommonPrefixBooleansAtom::oahCommonPrefixBooleansAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& shortSuffixDescriptor,
  const std::string& longSuffixDescriptor,
  const S_oahPrefix& shortNamesPrefix,
  const S_oahPrefix& longNamesPrefix)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    shortNamesPrefix != nullptr,
    "shortNamesPrefix is null");
  mfAssert (
    __FILE__, __LINE__,
    longNamesPrefix != nullptr,
    "fLongNamesPrefix is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fShortSuffixDescriptor = shortSuffixDescriptor;
  fLongSuffixDescriptor  = longSuffixDescriptor;

  fShortNamesPrefix = shortNamesPrefix;
  fLongNamesPrefix  = longNamesPrefix;

  // get prefixes names
  fShortNamesPrefixName =
    fShortNamesPrefix->getPrefixName ();
  fLongNamesPrefixName =
    fLongNamesPrefix->getPrefixName ();
}

oahCommonPrefixBooleansAtom::~oahCommonPrefixBooleansAtom ()
{}

void oahCommonPrefixBooleansAtom::addBooleanAtom (
  const S_oahBooleanAtom& booleanAtom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    booleanAtom != nullptr,
    "booleanAtom is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // long name consistency check
  {
    std::string booleanAtomLongName =
      booleanAtom->getLongName ();

    if (booleanAtomLongName.size ()) {
      std::size_t found =
        booleanAtomLongName.find (fLongNamesPrefixName);

      if (found == std::string::npos) {
        std::stringstream ss;

        ss <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" is different than the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          std::endl;

        booleanAtom->print (ss);

//         oahError (ss.str ());
        oahWarning (ss.str ());
      }

      else if (found != 0) {
        std::stringstream ss;

        ss <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" doesn't start by the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          std::endl;

        booleanAtom->print (ss);

        oahError (ss.str ());
      }

      else {
        std::string booleanAtomLongNameSuffix =
          booleanAtomLongName.substr (
            fLongNamesPrefixName.size ());

        if (booleanAtomLongNameSuffix.size () == 0) {
          std::stringstream ss;

          ss <<
            "Option long name \"" <<
            booleanAtomLongName <<
            "\" is nothing more than the long names prefix name \"" <<
            fLongNamesPrefixName <<
            "\"" <<
          std::endl;

          booleanAtom->print (ss);

          oahError (ss.str ());
        }
        else {
          // register this boolean atom's suffix in the list
          fLongNamesSuffixes.push_back (booleanAtomLongNameSuffix);
        }
      }
    }

    else {
      std::stringstream ss;

      ss <<
        "Option long name \"" <<
        booleanAtomLongName <<
        "\" is empty, atom \"" <<
        fLongNamesPrefixName <<
        "\" cannot be used in a multiplex booleans atom";

      booleanAtom->print (ss);

      oahError (ss.str ());
    }
  }

  // short name consistency check
  {
    std::string booleanAtomShortName =
      booleanAtom->getShortName ();

    std::size_t found =
      booleanAtomShortName.find (fShortNamesPrefixName);

    if (found == std::string::npos) {
      std::stringstream ss;

      ss <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" is different than the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (ss);

      oahError (ss.str ());
    }

    else if (found != 0) {
      std::stringstream ss;

      ss <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" doesn't start by the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (ss);

//       oahError (ss.str ());
      oahWarning (ss.str ());
    }

    else {
      std::string booleanAtomShortNameSuffix =
        booleanAtomShortName.substr (
          fShortNamesPrefixName.size ());

      if (booleanAtomShortNameSuffix.size () == 0) {
        std::stringstream ss;

        ss <<
          "Option short name \"" <<
          booleanAtomShortName <<
          "\" is nothing more than the short names prefix name \"" <<
          fShortNamesPrefixName <<
          "\"";

        booleanAtom->print (ss);

        oahError (ss.str ());
      }
      else {
        // register this boolean atom's suffix in the list
        fShortNamesSuffixes.push_back (booleanAtomShortNameSuffix);
      }
    }
  }

  // append the boolean atom to the list
  fBooleanAtomsList.push_back (
    booleanAtom);

  // make this atom imvisible
  booleanAtom->
    setElementVisibilityKind (
      oahElementVisibilityKind::kElementVisibilityNone);
}

void oahCommonPrefixBooleansAtom::addBooleanAtomByName (
  const std::string& name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    handler->
      unknownOptionNameError (
        name,
        "in common prefix booleans atom");
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // add the boolean atom
      addBooleanAtom (atom);
    }

    else {
      handler->
        unknownOptionNameError (
          name,
          "in combined booleans atom, not the name of an atom");
    }
  }
}

void oahCommonPrefixBooleansAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCommonPrefixBooleansAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle it at once JMI ??? v0.9.66

  fSelected = true;
}

Bool oahCommonPrefixBooleansAtom::atomMatchesString (
	const std::string&       lowerCaseString,
	std::ostream&            os) const
{
  Bool result;

	gLog <<
		"=======>>> oahCommonPrefixBooleansAtom::atomMatchesString ()" <<
		", lowerCaseString: " << lowerCaseString <<
		std::endl;
	gLog << "THIS: " << std::endl;
	printHelp (os);
	gLog << std::endl;

  if (elementMatchesString (lowerCaseString)) {
    result = true;
  }

	for (S_oahBooleanAtom booleanAtom : fBooleanAtomsList) {
		if (booleanAtom->elementMatchesString (lowerCaseString)) {
			result = true;
		}
	} // for

	if (result) {
    // append this matching element to foundElementsList
//     foundElementsList.push_back (this);// JMI v0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         fetchNames (),
//         fDescription,
//         containingFindableElementAsString ()));
	}

	gLog <<
		"<<<<====== oahCommonPrefixBooleansAtom::atomMatchesString ()" <<
		", result: " << result <<
		std::endl;

  return result;
}

void oahCommonPrefixBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahCommonPrefixBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCommonPrefixBooleansAtom>*> (v)) {
        S_oahCommonPrefixBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahCommonPrefixBooleansAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahCommonPrefixBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahCommonPrefixBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCommonPrefixBooleansAtom>*> (v)) {
        S_oahCommonPrefixBooleansAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahCommonPrefixBooleansAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahCommonPrefixBooleansAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fShortNamesPrefix) {
    // browse the short names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fShortNamesPrefix));
  }

  if (fLongNamesPrefix) {
    // browse the long names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fLongNamesPrefix));
  }

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      std::list <S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahCommonPrefixBooleansAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiplexBooleansAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    "shortSuffixDescriptor" << ": " <<
    fShortSuffixDescriptor <<
    std::endl <<
    "longSuffixDescriptor" << ": " <<
    fLongSuffixDescriptor <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "shortNamesPrefix" << ": ";
  if (fShortNamesPrefix) {
    os << fShortNamesPrefix;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "longNamesPrefix" << ": ";
  if (fLongNamesPrefix) {
    os << fLongNamesPrefix;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBooleanAtomsList" << ": ";

  if (! fBooleanAtomsList.size ()) {
    os << "[EMPTY]";
  }
  else {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    --gIndenter;
  }
  os << std::endl;

  --gIndenter;
}

void oahCommonPrefixBooleansAtom::printHelp (std::ostream& os) const
{
  os <<
    '-' << fShortNamesPrefixName << "<" << fShortSuffixDescriptor << ">" <<
    ", " <<
    '-' << fLongNamesPrefixName << "-<" << fLongSuffixDescriptor << ">" <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os <<
    "The " <<
    fShortNamesSuffixes.size () <<
    " known " << fShortSuffixDescriptor << "s are: ";

  if (fShortNamesSuffixes.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list <std::string>::const_iterator
      iBegin = fShortNamesSuffixes.begin (),
      iEnd   = fShortNamesSuffixes.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      std::string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
        os << std::endl;
        cumulatedLength = 0;
        break;
      }

      os << suffix;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << std::endl;
    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      std::endl;
  }

  if (fLongSuffixDescriptor != fShortSuffixDescriptor) {
//     int longNamesSuffixesCount = 0; // JMI v9.9.69
//
//     {
//       std::list <std::string>::const_iterator
//         iBegin = fLongNamesSuffixes.begin (),
//         iEnd   = fLongNamesSuffixes.end (),
//         i      = iBegin;
//
//       for ( ; ; ) {
//         if ((*i).size ()) {
//           ++longNamesSuffixesCount;
//         }
//
//         if (++i == iEnd) break;
//       } // for
//     }

    os <<
      "The " <<
      fLongNamesSuffixes.size () <<
 // JMI     " -- " << longNamesSuffixesCount <<
      " known " << fLongSuffixDescriptor << "s are: ";

    if (fLongNamesSuffixes.size ()) {
      os << std::endl;
      ++gIndenter;

      std::list <std::string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      int cumulatedLength = 0;

      for ( ; ; ) {
        std::string suffix = (*i);

        cumulatedLength += suffix.size ();
        if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
          os << std::endl;
          cumulatedLength = 0;
//        break;
        }

        os << suffix;

        if (++i == iEnd) break;

        if (next (i) == iEnd) {
          os << " and ";
        }
        else {
          os << ", ";
        }
      } // for

      os << "." << std::endl;
      --gIndenter;
    }
    else {
      os <<
        "[NONE]" <<
        std::endl;
    }
  }

  if (fDescription.size ()) { // ??? JMI
// JMI    g_Indenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os << std::left <<
    "fBooleanAtomsList" << ": ";

  if (! fBooleanAtomsList.size ()) {
    os << "[EMPTY]";
  }
  else {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    --gIndenter;
  }
  os << std::endl;
}

void oahCommonPrefixBooleansAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the boolean atoms in the list
}

std::ostream& operator << (std::ostream& os, const S_oahCommonPrefixBooleansAtom& elt)
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
S_oahRegexAtom oahRegexAtom::create ( // JMI UNUSED as of v0.9.66
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& regexString)
{
  oahRegexAtom* obj = new
    oahRegexAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      regexString);
  assert (obj != nullptr);
  return obj;
}

oahRegexAtom::oahRegexAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& regexString)
  : oahStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable),
    fRegexString (
      regexString)
{}

oahRegexAtom::~oahRegexAtom ()
{}

void oahRegexAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahRegexAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI to do!

  oahStringAtom::applyAtomWithValue (
    theString,
    os);

  fSelected = true;
}

void oahRegexAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRegexAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahRegexAtom>*
    p =
      dynamic_cast<visitor<S_oahRegexAtom>*> (v)) {
        S_oahRegexAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahRegexAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahRegexAtom::setStringVariable (const std::string& value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahStringAtom::setStringVariable (value);

  fSelected = true;
}

void oahRegexAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRegexAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahRegexAtom>*
    p =
      dynamic_cast<visitor<S_oahRegexAtom>*> (v)) {
        S_oahRegexAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahRegexAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahRegexAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRegexAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahRegexAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' << fStringVariable;

  return ss.str ();
}

std::string oahRegexAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' << fStringVariable;

  return ss.str ();
}

void oahRegexAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "stringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRegexString" << ": " <<
    "\"" << fRegexString << "\"" <<
    std::endl;

  --gIndenter;
}

void oahRegexAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fStringVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahRegexAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahRationalAtom oahRationalAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfRational&          rationalVariable)
{
  oahRationalAtom* obj = new
    oahRationalAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      rationalVariable);
  assert (obj != nullptr);
  return obj;
}

oahRationalAtom::oahRationalAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfRational&          rationalVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fRationalVariable (
      rationalVariable)
{}

oahRationalAtom::~oahRationalAtom ()
{}

void oahRationalAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the fraction:
  // decipher it to extract numerator and denominator values

  std::string regularExpression (
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // numerator
    "/"
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // denominator
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for mfRational string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) { // JMI ???
    int
      numerator,
      denominator;

    {
      std::stringstream ss;
      ss << sm [1];
      ss >> numerator;
    }
    {
      std::stringstream ss;
      ss << sm [2];
      ss >> denominator;
    }

    mfRational
      rationalValue =
        mfRational (numerator, denominator);

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "rationalValue: " <<
        rationalValue;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    setRationalVariable (rationalValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "mfRational atom value \"" << theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahRationalAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRationalAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahRationalAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahRationalAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRationalAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahRationalAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahRationalAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahRationalAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahRationalAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' << fRationalVariable;

  return ss.str ();
}

std::string oahRationalAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' << fRationalVariable;

  return ss.str ();
}

void oahRationalAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RationalAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "rationalVariable" << ": " <<
    fRationalVariable <<
    std::endl;

  --gIndenter;
}

void oahRationalAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fRationalVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahRationalAtom& elt)
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
S_oahNaturalNumbersSetElementAtom oahNaturalNumbersSetElementAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <int>&     naturalNumbersSetVariable)
{
  oahNaturalNumbersSetElementAtom* obj = new
    oahNaturalNumbersSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      naturalNumbersSetVariable);
  assert (obj != nullptr);
  return obj;
}

oahNaturalNumbersSetElementAtom::oahNaturalNumbersSetElementAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <int>&     naturalNumbersSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fNaturalNumbersSetVariable (
      naturalNumbersSetVariable)
{}

oahNaturalNumbersSetElementAtom::~oahNaturalNumbersSetElementAtom ()
{}

void oahNaturalNumbersSetElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  fNaturalNumbersSetVariable =
    mfDecipherNaturalNumbersSetSpecification (
      theString,
      false); // 'true' to debug it

  fSelected = true;
}

void oahNaturalNumbersSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahNaturalNumbersSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahNaturalNumbersSetElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahNaturalNumbersSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    '[';

  std::set <int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    ss << (*i);
    if (++i == iEnd) break;
    ss << ' ';
  } // for

  ss <<
    ']';

  return ss.str ();
}

std::string oahNaturalNumbersSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    '[';

  std::set <int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    ss << (*i);
    if (++i == iEnd) break;
    ss << ' ';
  } // for

  ss <<
    ']';

  return ss.str ();
}

void oahNaturalNumbersSetElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "NaturalNumbersSetElementAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNaturalNumbersSetVariable" << ": " <<
    std::endl;

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    os <<
      '\'';

    std::set <int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os <<
      '\'';
  }

  os << std::endl;

  --gIndenter;
}

void oahNaturalNumbersSetElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    switch (fEarlyOptionKind) {
      case oahEarlyOptionKind::kEarlyOptionNo:
        break;
      case oahEarlyOptionKind::kEarlyOptionYes:
        os <<
          ", early";
        break;
    } // switch
    if (fSelected) {
      os <<
        ", selected";
    }

    std::set <int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << '\'';
  }
}

std::ostream& operator << (std::ostream& os, const S_oahNaturalNumbersSetElementAtom& elt)
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
S_oahColorRGBAtom oahColorRGBAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrColorRGB&       colorRGBVariable)
{
  oahColorRGBAtom* obj = new
    oahColorRGBAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      colorRGBVariable);
  assert (obj != nullptr);
  return obj;
}

oahColorRGBAtom::oahColorRGBAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrColorRGB&       colorRGBVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fColorRGBVariable (
      colorRGBVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahColorRGBAtom::~oahColorRGBAtom ()
{}

void oahColorRGBAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  msrColorRGB theColorRGB (theString);

  setColorRGBVariable (theColorRGB);

  fSelected = true;
}

void oahColorRGBAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahColorRGBAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahColorRGBAtom>*
    p =
      dynamic_cast<visitor<S_oahColorRGBAtom>*> (v)) {
        S_oahColorRGBAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahColorRGBAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahColorRGBAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahColorRGBAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahColorRGBAtom>*
    p =
      dynamic_cast<visitor<S_oahColorRGBAtom>*> (v)) {
        S_oahColorRGBAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahColorRGBAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahColorRGBAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahColorRGBAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahColorRGBAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    '[' <<
    fColorRGBVariable.asString () <<
    ']';

  return ss.str ();
}

std::string oahColorRGBAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    '[' <<
    fColorRGBVariable.asString () <<
    ']';

  return ss.str ();
}

void oahColorRGBAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "ColorRGBAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "colorRGBVariable" << ": " <<
    fColorRGBVariable.asString () <<
    std::endl;

  --gIndenter;
}

void oahColorRGBAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fColorRGBVariable.asString ();
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected" <<
      std::endl;
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahColorRGBAtom& elt)
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
S_oahIntSetElementAtom oahIntSetElementAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <int>&     intSetVariable)
{
  oahIntSetElementAtom* obj = new
    oahIntSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      intSetVariable);
  assert (obj != nullptr);
  return obj;
}

oahIntSetElementAtom::oahIntSetElementAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <int>&     intSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fIntSetVariable (
      intSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahIntSetElementAtom::~oahIntSetElementAtom ()
{}

void oahIntSetElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the bar number specification
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling atom '" <<
      fetchNames () <<
      "' of type 'oahIntSetElementAtom'" <<
      " with value \"" << theString << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "([[:digit:]]+)"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for reset measure number string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 2) {
    std::stringstream ss;

    ss <<
      "-boxAroundBarNumber argument \"" << theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  int lilypondMeasureNumber;
  {
    std::stringstream ss;
    ss << sm [1];
    ss >> lilypondMeasureNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "lilypondMeasureNumber: " <<
      lilypondMeasureNumber;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fIntSetVariable.insert (lilypondMeasureNumber);

  fSelected = true;
}

void oahIntSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntSetElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIntSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetElementAtom>*> (v)) {
        S_oahIntSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIntSetElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahIntSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntSetElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIntSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetElementAtom>*> (v)) {
        S_oahIntSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIntSetElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahIntSetElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIntSetElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahIntSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fIntSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string oahIntSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fIntSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void oahIntSetElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntSetElementAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "intSetVariable" << " : '" <<
    std::endl;

  if (! fIntSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::set <int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  os << std::endl;

  --gIndenter;
}

void oahIntSetElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fIntSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::set <int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "selected" <<
      std::endl;

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahIntSetElementAtom& elt)
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
S_oahStringSetElementAtom oahStringSetElementAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::set <std::string>& stringSetVariable)
{
  oahStringSetElementAtom* obj = new
    oahStringSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringSetElementAtom::oahStringSetElementAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::set <std::string>& stringSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringSetElementAtom::~oahStringSetElementAtom ()
{}

void oahStringSetElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringSetElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the name of the part to be ignored

  std::string partName = theString;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partName = \"" << partName << "\", ";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::set <std::string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "part \"" << partName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }

  fSelected = true;
}

void oahStringSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringSetElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetElementAtom>*> (v)) {
        S_oahStringSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringSetElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringSetElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetElementAtom>*> (v)) {
        S_oahStringSetElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringSetElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringSetElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringSetElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string oahStringSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void oahStringSetElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringSetElementAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringSetVariable" << ": " <<
    std::endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  os << std::endl;

  --gIndenter;
}

void oahStringSetElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os <<
      "selected" <<
      std::endl;

    ++gIndenter;

    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahStringSetElementAtom& elt)
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
S_oahStringToIntMapElementAtom oahStringToIntMapElementAtom::create (
  const std::string&     shortName,
  const std::string&     longName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::map <std::string, int>& stringToIntMapVariable)
{
  oahStringToIntMapElementAtom* obj = new
    oahStringToIntMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToIntMapVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringToIntMapElementAtom::oahStringToIntMapElementAtom (
  const std::string&     shortName,
  const std::string&     longName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::map <std::string, int>& stringToIntMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToIntMapVariable (
      stringToIntMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringToIntMapElementAtom::~oahStringToIntMapElementAtom ()
{}

void oahStringToIntMapElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the string int map specification
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"     // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:digit:]]+)" // int
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string musicxmlMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    std::stringstream ss;
    ss << sm [2];
    ss >> lilypondMeasureNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "musicxmlMeasureNumber: " <<
      musicxmlMeasureNumber <<
      std::endl <<
      "lilypondMeasureNumber: " <<
      lilypondMeasureNumber;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringToIntMapVariable [musicxmlMeasureNumber] =
    lilypondMeasureNumber;

  fSelected = true;
}

void oahStringToIntMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToIntMapElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToIntMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapElementAtom>*> (v)) {
        S_oahStringToIntMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToIntMapElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringToIntMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToIntMapElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToIntMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapElementAtom>*> (v)) {
        S_oahStringToIntMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToIntMapElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringToIntMapElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToIntMapElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringToIntMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string oahStringToIntMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void oahStringToIntMapElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToIntMapAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringToIntMapVariable" << ": " <<
    std::endl;

  if (! fStringToIntMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map <std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  os << std::endl;

  --gIndenter;
}

void oahStringToIntMapElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringToIntMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map <std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "selected" <<
      std::endl;

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahStringToIntMapElementAtom& elt)
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
S_oahStringToStringMapElementAtom oahStringToStringMapElementAtom::create (
  const std::string&                  longName,
  const std::string&                  shortName,
  const std::string&                  description,
  const std::string&                  valueSpecification,
  const std::string&                  variableName,
  std::map <std::string, std::string>& stringToStringMapVariable)
{
  oahStringToStringMapElementAtom* obj = new
    oahStringToStringMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringToStringMapElementAtom::oahStringToStringMapElementAtom (
  const std::string&                  longName,
  const std::string&                  shortName,
  const std::string&                  description,
  const std::string&                  valueSpecification,
  const std::string&                  variableName,
  std::map <std::string, std::string>& stringToStringMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToStringMapVariable (
      stringToStringMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringToStringMapElementAtom::~oahStringToStringMapElementAtom ()
{}

void oahStringToStringMapElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the string string map specification // JMI v0.9.66
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    key   = sm [1],
    value = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "key: " <<
      key <<
      std::endl <<
      "value: " <<
      value;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringToStringMapVariable [key] = value;

  fSelected = true;
}

void oahStringToStringMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMapElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToStringMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMapElementAtom>*> (v)) {
        S_oahStringToStringMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToStringMapElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringToStringMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMapElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToStringMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMapElementAtom>*> (v)) {
        S_oahStringToStringMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToStringMapElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringToStringMapElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMapElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringToStringMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string oahStringToStringMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void oahStringToStringMapElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToStringMapAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringToStringMapVariable" << ": ";

  if (! fStringToStringMapVariable.size ()) {
    os << "[EMPTY]" << std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        std::endl;
      if (++i == iEnd) break;
//       os << std::endl; // JMI
    } // for

    --gIndenter;
  }

  os << std::endl;
  --gIndenter;
}

void oahStringToStringMapElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringToStringMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "selected" <<
      std::endl;

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahStringToStringMapElementAtom& elt)
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
S_oahStringToStringMultiMapElementAtom oahStringToStringMultiMapElementAtom::create (
  const std::string&                       longName,
  const std::string&                       shortName,
  const std::string&                       description,
  const std::string&                       valueSpecification,
  const std::string&                       variableName,
  std::multimap <std::string, std::string>& stringToStringMultiMapVariable)
{
  oahStringToStringMultiMapElementAtom* obj = new
    oahStringToStringMultiMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMultiMapVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringToStringMultiMapElementAtom::oahStringToStringMultiMapElementAtom (
  const std::string&                       longName,
  const std::string&                       shortName,
  const std::string&                       description,
  const std::string&                       valueSpecification,
  const std::string&                       variableName,
  std::multimap <std::string, std::string>& stringToStringMultiMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToStringMultiMapVariable (
      stringToStringMultiMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringToStringMultiMapElementAtom::~oahStringToStringMultiMapElementAtom ()
{}

void oahStringToStringMultiMapElementAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the string string std::multimap specification
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    key   = sm [1],
    value = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "key: " <<
      key <<
      std::endl <<
      "value: " <<
      value;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringToStringMultiMapVariable.insert (
    std::pair <std::string, std::string> (key, value));

  fSelected = true;
}

void oahStringToStringMultiMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToStringMultiMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMultiMapElementAtom>*> (v)) {
        S_oahStringToStringMultiMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToStringMultiMapElementAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringToStringMultiMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringToStringMultiMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMultiMapElementAtom>*> (v)) {
        S_oahStringToStringMultiMapElementAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringToStringMultiMapElementAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringToStringMultiMapElementAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringToStringMultiMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::multimap <std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string oahStringToStringMultiMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::multimap <std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void oahStringToStringMultiMapElementAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToStringMultiMapAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringToStringMultiMapVariable" << ": ";

  if (! fStringToStringMultiMapVariable.size ()) {
    os << "[EMPTY]" << std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::multimap <std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        std::endl;
      if (++i == iEnd) break;
//       os << std::endl; // JMI
    } // for

    --gIndenter;
  }

  os << std::endl;
  --gIndenter;
}

void oahStringToStringMultiMapElementAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringToStringMultiMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::multimap <std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "selected" <<
      std::endl;

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahStringToStringMultiMapElementAtom& elt)
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
S_oahStringAndIntegerAtom oahStringAndIntegerAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& stringVariableName,
  std::string&       stringVariable,
  const std::string& integerVariableName,
  int&               integerVariable)
{
  oahStringAndIntegerAtom* obj = new
    oahStringAndIntegerAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      stringVariableName,
      stringVariable,
      integerVariableName,
      integerVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringAndIntegerAtom::oahStringAndIntegerAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& stringVariableName,
  std::string&       stringVariable,
  const std::string& integerVariableName,
  int&               integerVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      stringVariableName),
    fStringVariableName (
      stringVariableName),
    fStringVariable (
      stringVariable),
    fIntegerVariableName (
      integerVariableName),
    fIntegerVariable (
      integerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndIntegerAtom::~oahStringAndIntegerAtom ()
{}

void oahStringAndIntegerAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  std::string regularExpression (
    "(.+)"           // string
    "[[:space:]]+"
    ":"
    "[[:space:]]+"
    "([[:digit:]]+)" // int
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream ss;
      ss << sm [ 2 ];
      ss >> integerValue;
    }

    fIntegerVariable = integerValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahStringAndIntegerAtom::setIntegerVariable (int value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fIntegerVariable = value;

  fSelected = true;
}

void oahStringAndIntegerAtom::setStringVariable (const std::string& value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
      value <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringVariable = value;

  fSelected = true;
}

void oahStringAndIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringAndIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringAndIntegerAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndIntegerAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringAndIntegerAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return ss.str ();
}

std::string oahStringAndIntegerAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return ss.str ();
}

void oahStringAndIntegerAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "stringVariableName" << ": " <<
    "\"" << fStringVariableName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "stringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "integerVariableName" << ": " <<
    fIntegerVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "integerVariable" << ": " <<
    fIntegerVariable <<
    std::endl;

  --gIndenter;
}

void oahStringAndIntegerAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fStringVariableName <<
    ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<

    std::setw (valueFieldWidth) <<
    fIntegerVariableName <<
    ": " <<
    "\"" << fIntegerVariable << "\"";

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAndIntegerAtom& elt)
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
S_oahStringAndTwoIntegersAtom oahStringAndTwoIntegersAtom::create ( // UNUSED JMI
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const std::string&  valueSpecification,
  const std::string&  variableName,
  std::string&        stringVariable,
  int&                primaryIntegerVariable,
  int&                secondaryIntegerVariable)
{
  oahStringAndTwoIntegersAtom* obj = new
    oahStringAndTwoIntegersAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      primaryIntegerVariable,
      secondaryIntegerVariable);
  assert (obj != nullptr);
  return obj;
}

oahStringAndTwoIntegersAtom::oahStringAndTwoIntegersAtom (
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const std::string&  valueSpecification,
  const std::string&  variableName,
  std::string&        stringVariable,
  int&                primaryIntegerVariable,
  int&                secondaryIntegerVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable),
    fPrimaryIntegerVariable (
      primaryIntegerVariable),
    fSecondaryIntegerVariable (
      secondaryIntegerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndTwoIntegersAtom::~oahStringAndTwoIntegersAtom ()
{}

void oahStringAndTwoIntegersAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  std::string regularExpression (
    "([[:alpha:]]+)" // string
    "[[:space:]]+"
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // secondary integer value
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 4) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream ss;
      ss << sm [ 2 ];
      ss >> integerValue;
    }
    fPrimaryIntegerVariable = integerValue; // JMI

    {
      std::stringstream ss;
      ss << sm [ 3 ];
      ss >> integerValue;
    }
    fSecondaryIntegerVariable = integerValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahStringAndTwoIntegersAtom::setStringVariable (const std::string& value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStringVariable = value;

  fSelected = true;
}

void oahStringAndTwoIntegersAtom::setPrimaryIntegerVariable (int value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' primary integer variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPrimaryIntegerVariable = value;

  fSelected = true;
}

void oahStringAndTwoIntegersAtom::setSecondaryIntegerVariable (int value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting option '" <<
      fetchNames () <<
      "' secondary integer variable to '" <<
      value <<
      '\'';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSecondaryIntegerVariable = value;

  fSelected = true;
}

void oahStringAndTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahStringAndTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahStringAndTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahStringAndTwoIntegersAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    ' ' <<
    fSecondaryIntegerVariable;

  return ss.str ();
}

std::string oahStringAndTwoIntegersAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    ' ' <<
    fSecondaryIntegerVariable;

  return ss.str ();
}

void oahStringAndTwoIntegersAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    std::setw (fieldWidth) <<
    "stringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    "primaryIntegerVariable" << ": " <<
    fPrimaryIntegerVariable <<
    "secondaryIntegerVariable" << ": " <<
    fSecondaryIntegerVariable <<
    std::endl;

  --gIndenter;
}

void oahStringAndTwoIntegersAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<
    std::setw (valueFieldWidth) <<
    "stringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<
    std::setw (valueFieldWidth) <<
    "primaryIntegerVariable" <<
    ": " <<
    fPrimaryIntegerVariable <<
    std::endl <<
    std::setw (valueFieldWidth) <<
    "secondaryIntegerVariable" <<
    ": " <<
    fSecondaryIntegerVariable;

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAndTwoIntegersAtom& elt)
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
S_oahLengthUnitKindAtom oahLengthUnitKindAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
{
  oahLengthUnitKindAtom* obj = new
    oahLengthUnitKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lengthUnitKindVariable);
  assert (obj != nullptr);
  return obj;
}

oahLengthUnitKindAtom::oahLengthUnitKindAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLengthUnitKindVariable (
      lengthUnitKindVariable)
{}

oahLengthUnitKindAtom::~oahLengthUnitKindAtom ()
{}

void oahLengthUnitKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msrLengthUnitKind>::const_iterator
    it =
      gGlobalMsrLengthUnitKindsMap.find (
        theString);

  if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
    // no, score output kind is unknown in the map

    std::stringstream ss;

    ss <<
      "LPSR score output kind \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsrLengthUnitKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsrLengthUnitKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLengthUnitKindVariable (
    (*it).second);

  fSelected = true;
}

void oahLengthUnitKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahLengthUnitKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahLengthUnitKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthUnitKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahLengthUnitKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return ss.str ();
}

std::string oahLengthUnitKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return ss.str ();
}

void oahLengthUnitKindAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthUnitKindAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "optionsLengthUnitKindVariable" << ": " <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    std::endl;

  --gIndenter;
}

void oahLengthUnitKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable);

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahLengthUnitKindAtom& elt)
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
S_oahLengthAtom oahLengthAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLength&         lengthVariable)
{
  oahLengthAtom* obj = new
    oahLengthAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lengthVariable);
  assert (obj != nullptr);
  return obj;
}

oahLengthAtom::oahLengthAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLength&         lengthVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLengthVariable (
      lengthVariable)
{}

oahLengthAtom::~oahLengthAtom ()
{}

void oahLengthAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahLengthAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the score output kind:

  // check whether it is well-formed
   // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
  std::string regularExpression (
    "([[:digit:]]+)"     // integer part
    "(.[[:digit:]]*)"    // decimal part
    "?([[:alpha:]]{2,})" // length unit
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        '[' << sm [i] << "] ";
    } // for

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 4) {
    // leave the low level details to the STL...
    float floatValue;
    {
      std::stringstream ss;
      // concatenate the integer and decimal parts
      ss << sm [ 1 ] << sm [ 2 ];
      ss >> floatValue;
    }

    std::string lengthUnitName = sm [ 3 ];

    // is lengthUnitName known in the length unit names map?
    std::map <std::string, msrLengthUnitKind>::const_iterator
      it =
        gGlobalMsrLengthUnitKindsMap.find (
          lengthUnitName);

    if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
      // no, length unit name is unknown in the map

      std::stringstream ss;

      ss <<
        "length unit name \"" << lengthUnitName <<
        "\" is unknown" <<
        std::endl <<
        "The " <<
        gGlobalMsrLengthUnitKindsMap.size () <<
        " known length unit names are:" <<
        std::endl;

      ++gIndenter;

      ss <<
        availableMsrLengthUnitKinds (K_MF_NAMES_LIST_MAX_LENGTH);

      --gIndenter;

      oahError (ss.str ());
    }

    setLengthVariable (
      msrLength (
        (*it).second,
        floatValue));
  }

  else {
    std::stringstream ss;

    ss <<
      "length value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  fSelected = true;
}

void oahLengthAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLengthAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahLengthAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahLengthAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahLengthAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahLengthAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahLengthAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fLengthVariable.asString ();

  return ss.str ();
}

std::string oahLengthAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fLengthVariable.asString ();

  return ss.str ();
}

void oahLengthAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "optionsLengthVariable" << ": " <<
    fLengthVariable.asString () <<
    std::endl;

  --gIndenter;
}

void oahLengthAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fLengthVariable.asString ();

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahLengthAtom& elt)
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
S_oahMidiTempoAtom oahMidiTempoAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrMidiTempo&      midiTempoVariable)
{
  oahMidiTempoAtom* obj = new
    oahMidiTempoAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      midiTempoVariable);
  assert (obj != nullptr);
  return obj;
}

oahMidiTempoAtom::oahMidiTempoAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrMidiTempo&      midiTempoVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMidiTempoVariable (
      midiTempoVariable)
{}

oahMidiTempoAtom::~oahMidiTempoAtom ()
{}

void oahMidiTempoAtom::setMidiTempoVariable (
  const msrMidiTempo& value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting midi tempo atom variable to " <<
      value.asString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMidiTempoVariable = value;

  fSelected = true;
}

void oahMidiTempoAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahMidiTempoAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'oahMidiTempoAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // midiTempoNotesDuration
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // midiTempoPerSecond
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "-midiTempo argument \"" << theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string midiTempoNotesDuration = sm [1];

  int midiTempoPerSecond;
  {
    std::stringstream ss;
    ss << sm [2];
    ss >> midiTempoPerSecond;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "midiTempoNotesDuration: " <<
      midiTempoNotesDuration <<
      std::endl <<
      "midiTempoPerSecond: " <<
      midiTempoPerSecond <<
      std::endl;

  msrMidiTempo
    theMidiTempo (
      0, // inputLineNumber
      midiTempoNotesDuration,
      midiTempoPerSecond);

  setMidiTempoVariable (
    theMidiTempo);
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void oahMidiTempoAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMidiTempoAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahMidiTempoAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMidiTempoAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahMidiTempoAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMidiTempoAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahMidiTempoAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fMidiTempoVariable.asString ();

  return ss.str ();
}

std::string oahMidiTempoAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fMidiTempoVariable.asString ();

  return ss.str ();
}

void oahMidiTempoAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MidiTempoAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "midiTempoVariable" << ": \"" <<
    fMidiTempoVariable.asString () <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void oahMidiTempoAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fMidiTempoVariable.asString ();
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected" <<
      std::endl;
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahMidiTempoAtom& elt)
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
S_oahHelpAboutOptionName oahHelpAboutOptionName::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahHelpAboutOptionName* obj = new
    oahHelpAboutOptionName (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahHelpAboutOptionName::oahHelpAboutOptionName (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpValueFittedAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahHelpAboutOptionName::~oahHelpAboutOptionName ()
{}

void oahHelpAboutOptionName::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpAboutOptionName";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

// void oahHelpAboutOptionName::applyAtomWithDefaultValue (std::ostream& os)
// {
//   // delegate this to the handler
//   fetchAtomUpLinkToHandler ()->
//     printNameIntrospectiveHelp (
//       os,
//       fDefaultStringValue);
// }

void oahHelpAboutOptionName::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAboutOptionName::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpAboutOptionName>*
    p =
      dynamic_cast<visitor<S_oahHelpAboutOptionName>*> (v)) {
        S_oahHelpAboutOptionName elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpAboutOptionName::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahHelpAboutOptionName::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAboutOptionName::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHelpAboutOptionName>*
    p =
      dynamic_cast<visitor<S_oahHelpAboutOptionName>*> (v)) {
        S_oahHelpAboutOptionName elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHelpAboutOptionName::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahHelpAboutOptionName::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHelpAboutOptionName::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahHelpAboutOptionName::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahHelpAboutOptionName::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

void oahHelpAboutOptionName::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpAboutOptionName:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLog, fieldWidth);

  --gIndenter;
}

void oahHelpAboutOptionName::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahHelpAboutOptionName& elt)
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
S_oahQueryOptionNameAtom oahQueryOptionNameAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahQueryOptionNameAtom* obj = new
    oahQueryOptionNameAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahQueryOptionNameAtom::oahQueryOptionNameAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpValueFittedAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  this->setMultipleOccurrencesAllowed ();
}

oahQueryOptionNameAtom::~oahQueryOptionNameAtom ()
{}

void oahQueryOptionNameAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahQueryOptionNameAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahQueryOptionNameAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahQueryOptionNameAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahQueryOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahQueryOptionNameAtom>*> (v)) {
        S_oahQueryOptionNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahQueryOptionNameAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahQueryOptionNameAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahQueryOptionNameAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahQueryOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahQueryOptionNameAtom>*> (v)) {
        S_oahQueryOptionNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahQueryOptionNameAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahQueryOptionNameAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahQueryOptionNameAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahQueryOptionNameAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahQueryOptionNameAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahQueryOptionNameAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "QueryOptionNameAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLog, fieldWidth);

  --gIndenter;
}

void oahQueryOptionNameAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahQueryOptionNameAtom& elt)
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
S_oahFindStringResult oahFindStringResult::create (
  const std::string&  theString,
  const S_oahElement& theElement)
{
  oahFindStringResult* obj = new
    oahFindStringResult (
      theString,
      theElement);
  assert (obj != nullptr);
  return obj;
}

oahFindStringResult::oahFindStringResult (
  const std::string&  theString,
  const S_oahElement& theElement)
{
  fTheString  = theString;
  fTheElement = theElement;
}

oahFindStringResult::~oahFindStringResult ()
{}

std::string oahFindStringResult::asString () const
{
  std::stringstream ss;

  ss <<
    fTheString <<
    " - " <<
    fTheElement->asShortString ();

  return ss.str ();
}

void oahFindStringResult::print (std::ostream& os) const
{
  os <<
    "FindStringInHelpAtom:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    fTheString <<
    std::endl <<
    fTheElement->asShortString () <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const oahFindStringResult& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_oahFindStringResult& elt)
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
S_oahFindStringInHelpAtom oahFindStringInHelpAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahFindStringInHelpAtom* obj = new
    oahFindStringInHelpAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahFindStringInHelpAtom::oahFindStringInHelpAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpValueFittedAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  this->setMultipleOccurrencesAllowed ();
}

oahFindStringInHelpAtom::~oahFindStringInHelpAtom ()
{}

void oahFindStringInHelpAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFindStringAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString is the string to be found in the help

  // a strings list to collect the results
  std::list <S_oahFindStringMatch> foundStringMatchesList;

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    findStringInFindableElement (
      mfStringToLowerCase (theString),
      foundStringMatchesList,
      os);

  //  print the found strings
  size_t foundStringMatchesListSize =
    foundStringMatchesList.size ();

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    mfSingularOrPlural (
      foundStringMatchesListSize, "occurrence", "occurrences") <<
    " of string \"" <<
    theString <<
    "\" " <<
    mfSingularOrPluralWithoutNumber (
      foundStringMatchesListSize, "has", "have") <<
    " been found";

  if (foundStringMatchesListSize) {
    os <<
      ":" <<
      std::endl;

    ++gIndenter;

    std::list <S_oahFindStringMatch>::const_iterator
      iBegin = foundStringMatchesList.begin (),
      iEnd   = foundStringMatchesList.end (),
      i      = iBegin;

    int counter = 0;

    for ( ; ; ) {
      oahFindStringMatch* theFindStringMatch = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        theFindStringMatch != nullptr,
        "theFindStringMatch is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      std::string
        elementName =
          theFindStringMatch->
            getElementName (),
        foundString =
          theFindStringMatch->
            getFoundString (),
        containingFindableElementInfo =
          theFindStringMatch->
            getContainingFindableElementInfo ();

      ++counter;

      os << std::right <<
        std::setw (2) << counter << ": " << containingFindableElementInfo <<
        std::endl;

      ++gIndenter;

      os << elementName << std::endl;

      // indent a bit more for readability
      gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

      gIndenter.indentMultiLineString (
        foundString,
        os);

      // unindent a bit more for readability
      gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
      --gIndenter;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << std::endl;
  }

  gIndenter.setIndentation (saveIndent);
}

void oahFindStringInHelpAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFindStringInHelpAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFindStringInHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringInHelpAtom>*> (v)) {
        S_oahFindStringInHelpAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFindStringInHelpAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahFindStringInHelpAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFindStringInHelpAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahFindStringInHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringInHelpAtom>*> (v)) {
        S_oahFindStringInHelpAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahFindStringInHelpAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahFindStringInHelpAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahFindStringInHelpAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahFindStringInHelpAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahFindStringInHelpAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahFindStringInHelpAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FindStringInHelpAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLog, fieldWidth);

  --gIndenter;
}

void oahFindStringInHelpAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahFindStringInHelpAtom& elt)
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
S_oahMatchHelpWithPatternAtom oahMatchHelpWithPatternAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahMatchHelpWithPatternAtom* obj = new
    oahMatchHelpWithPatternAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahMatchHelpWithPatternAtom::oahMatchHelpWithPatternAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpValueFittedAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  this->setMultipleOccurrencesAllowed ();
}

oahMatchHelpWithPatternAtom::~oahMatchHelpWithPatternAtom ()
{}

void oahMatchHelpWithPatternAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahMatchHelpWithPatternAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString is the pattern to be found in the help

  // an element list to collect the results
  std::list <S_oahElement> foundElementsList; // JMI v0.9.71

  // delegate this to the handler // JMI v0.9.71
  Bool foundString =
		fetchAtomUpLinkToHandler ()->
			fetchElementsMatchingString (
				mfStringToLowerCase (theString),
				foundElementsList,
				os);

  //  print the found elements
  size_t foundMatchesListSize =
    foundElementsList.size ();

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    mfSingularOrPlural (
      foundMatchesListSize, "occurrence", "occurrences") <<
    " of pattern \"" <<
    theString <<
    "\" " <<
    mfSingularOrPluralWithoutNumber (
      foundMatchesListSize, "has", "have") <<
    " been found";

  if (foundMatchesListSize) {
    os <<
      ":" <<
      std::endl;

    ++gIndenter;

    std::list <S_oahElement>::const_iterator // JMI v0.9.71
      iBegin = foundElementsList.begin (),
      iEnd   = foundElementsList.end (),
      i      = iBegin;

    int counter = 0;

    for ( ; ; ) {
      S_oahElement matchingElement = (*i); // JMI v0.9.71

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        matchingElement != nullptr,
        "matchingElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//       std::string
//         elementName =
//           matchingElement->
//             getElementName (),
//         foundString =
//           matchingElement->
//             getFoundString (),
//         containingFilterableElementInfo =
//           matchingElement->
//             getContainingFindableElementInfo ();

      ++counter;

      os << std::right <<
        std::setw (2) << counter << ": " << // containingFilterableElementInfo <<
        std::endl;

      ++gIndenter;

      os << matchingElement << std::endl;

      // indent a bit more for readability
      gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

      gIndenter.indentMultiLineString (
        "foundString HOO",
        os);

      // unindent a bit more for readability
      gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
      --gIndenter;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << std::endl;
  }

  gIndenter.setIndentation (saveIndent);
}

void oahMatchHelpWithPatternAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMatchHelpWithPatternAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMatchHelpWithPatternAtom>*
    p =
      dynamic_cast<visitor<S_oahMatchHelpWithPatternAtom>*> (v)) {
        S_oahMatchHelpWithPatternAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMatchHelpWithPatternAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahMatchHelpWithPatternAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMatchHelpWithPatternAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahMatchHelpWithPatternAtom>*
    p =
      dynamic_cast<visitor<S_oahMatchHelpWithPatternAtom>*> (v)) {
        S_oahMatchHelpWithPatternAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahMatchHelpWithPatternAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahMatchHelpWithPatternAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahMatchHelpWithPatternAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahMatchHelpWithPatternAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahMatchHelpWithPatternAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahMatchHelpWithPatternAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MatchHelpWithPatternAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLog, fieldWidth);

  --gIndenter;
}

void oahMatchHelpWithPatternAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahMatchHelpWithPatternAtom& elt)
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
S_oahIncludeOptionsAndArgumentsFileAtom oahIncludeOptionsAndArgumentsFileAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahIncludeOptionsAndArgumentsFileAtom* obj = new
    oahIncludeOptionsAndArgumentsFileAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
}

oahIncludeOptionsAndArgumentsFileAtom::oahIncludeOptionsAndArgumentsFileAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahValueFittedAtom (
      longName,
      shortName,
      description)
{
//       valueSpecification, JMI v0.9.66
//       serviceName)

  this->setMultipleOccurrencesAllowed ();
}

oahIncludeOptionsAndArgumentsFileAtom::~oahIncludeOptionsAndArgumentsFileAtom ()
{}

void oahIncludeOptionsAndArgumentsFileAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // nothing to do, this option has already be applied early JMIJMIJMI
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahIncludeOptionsAndArgumentsFileAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // delegate this to the handler JMIJMIJMI v0.9.66 ??? see oahHandler::handleArgvOptionValueOrArgument()
//   fetchAtomUpLinkToHandler ()->
//     includeOptionsFileInHandler (
//       theString,
//       os);
}

void oahIncludeOptionsAndArgumentsFileAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*
    p =
      dynamic_cast<visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*> (v)) {
        S_oahIncludeOptionsAndArgumentsFileAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIncludeOptionsAndArgumentsFileAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahIncludeOptionsAndArgumentsFileAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*
    p =
      dynamic_cast<visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*> (v)) {
        S_oahIncludeOptionsAndArgumentsFileAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahIncludeOptionsAndArgumentsFileAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahIncludeOptionsAndArgumentsFileAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahIncludeOptionsAndArgumentsFileAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahIncludeOptionsAndArgumentsFileAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahIncludeOptionsAndArgumentsFileAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IncludeOptionsAndArgumentsFileAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLog, fieldWidth);

  --gIndenter;
}

void oahIncludeOptionsAndArgumentsFileAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahIncludeOptionsAndArgumentsFileAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
