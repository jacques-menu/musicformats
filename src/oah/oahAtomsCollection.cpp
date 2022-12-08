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
#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "oahBrowsers.h"

#include "mfcLibraryComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_oahAtomAlias oahAtomAlias::create (
  const std::string&    shortName,
  const std::string&    longName,
  const std::string&    description,
  const S_oahAtom&      originalOahAtom)
{
  oahAtomAlias* o = new
    oahAtomAlias (
      longName,
      shortName,
      description,
      originalOahAtom);
  assert (o != nullptr);
  return o;
}

oahAtomAlias::oahAtomAlias (
  const std::string&    shortName,
  const std::string&    longName,
  const std::string&    description,
  const S_oahAtom&      originalOahAtom)
  : oahAtom (
      longName,
      shortName,
      description,
      originalOahAtom->getElementValueKind ())
{
  fOriginalOahAtom = originalOahAtom;
}

oahAtomAlias::~oahAtomAlias ()
{}

void oahAtomAlias::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAtomAlias" <<
      std::endl;
  }
#endif

  // JMI ???
}

void oahAtomAlias::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomAlias::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahAtomAlias>*> (v)) {
        S_oahAtomAlias elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomAlias::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomAlias::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomAlias::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomAlias>*
    p =
      dynamic_cast<visitor<S_oahAtomAlias>*> (v)) {
        S_oahAtomAlias elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomAlias::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomAlias::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomAlias::browseData ()" <<
      std::endl;
  }
#endif

  if (fOriginalOahAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalOahAtom);
  }
}

void oahAtomAlias::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomAlias::printShort (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fOriginalOahAtom->fetchNames () <<
    std::endl;
}

void oahAtomAlias::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  fOriginalOahAtom->
    printAtomWithVariableOptionsValues (
      os,
      valueFieldWidth);
}

std::ostream& operator << (std::ostream& os, const S_oahAtomAlias& elt)
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
S_oahMacroAtom oahMacroAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahMacroAtom* o = new
    oahMacroAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

oahMacroAtom::oahMacroAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

oahMacroAtom::~oahMacroAtom ()
{}

void oahMacroAtom::appendAtomToMacro (S_oahAtom atom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    atom != nullptr,
    "atom is null");

  fMacroAtomsList.push_back (atom);
}

void oahMacroAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahMacroAtom" <<
      std::endl;
  }
#endif

  for (
    std::list<S_oahAtom>::const_iterator i =
      fMacroAtomsList.begin ();
    i != fMacroAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    if (
      // oahAtomStoringAValue?
      S_oahAtomStoringAValue
        atomWithVariable =
          dynamic_cast<oahAtomStoringAValue*>(&(*atom))
    ) {
//       atomWithVariable-> JMI ???
//         applyAtomWithValue (theString, os);
    }
    else {
      // valueless atom
      atom->
        applyElement (os);
    }
  } // for
}

void oahMacroAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMacroAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahMacroAtom>*
    p =
      dynamic_cast<visitor<S_oahMacroAtom>*> (v)) {
        S_oahMacroAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMacroAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMacroAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMacroAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahMacroAtom>*
    p =
      dynamic_cast<visitor<S_oahMacroAtom>*> (v)) {
        S_oahMacroAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMacroAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMacroAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMacroAtom::browseData ()" <<
      std::endl;
  }
#endif

  for (
    std::list<S_oahAtom>::const_iterator i =
      fMacroAtomsList.begin ();
    i != fMacroAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    // browse the atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*atom);
  } // for
}

void oahMacroAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  for (
    std::list<S_oahAtom>::const_iterator i =
      fMacroAtomsList.begin ();
    i != fMacroAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    os <<
      "atom:" <<
      std::endl;
    ++gIndenter;
    os <<
      atom;
    --gIndenter;
    os << std::endl;
  } // for

  --gIndenter;
}

void oahMacroAtom::printShort (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  std::list<S_oahAtom>::const_iterator
    iBegin = fMacroAtomsList.begin (),
    iEnd   = fMacroAtomsList.end (),
    i      = iBegin;

  for ( ; ; ) {
    S_oahAtom atom = (*i);

    os <<
      atom->fetchNames ();

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

  if (! fMacroAtomsList.size ()) {
    os <<
      "[NONE]" << // JMI
      std::endl;
  }

  else {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
      iBegin = fMacroAtomsList.begin (),
      iEnd   = fMacroAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom atom = (*i);

      os <<
        atom-> fetchNames () <<
        ":" <<
        std::endl;

      ++gIndenter;

      gIndenter.indentMultiLineString (
        atom-> getDescription (),
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
}

void oahMacroAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  oahElement::printOahElementEssentialsShort (
    os, valueFieldWidth);

  ++gIndenter;

  for (S_oahAtom atom : fMacroAtomsList) {
    atom->
      printAtomWithVariableOptionsValues (
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
    os << "[NONE]" << std::endl;
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
  oahOptionsUsageAtom* o = new
    oahOptionsUsageAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahOptionsUsageAtom::oahOptionsUsageAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahOptionsUsageAtom::~oahOptionsUsageAtom ()
{}

void oahOptionsUsageAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionsUsageAtom" <<
      std::endl;
  }
#endif

  printOptionsUsage (os);
}

void oahOptionsUsageAtom::printOptionsUsage (std::ostream& os) const
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    fHelpAtomWithoutAValueServiceName <<
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
        gGlobalOahOahGroup->
          getOptionNameHelpAtom ()->
            fetchNamesBetweenQuotes ()
        ),
      std::regex ("EXECUTABLE_NAME"),
      fHelpAtomWithoutAValueServiceName
      ),
    os);

  --gIndenter;

  gIndenter.setIndentation (saveIndent);
}

void oahOptionsUsageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionsUsageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionsUsageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahOptionsUsageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    os << "[NONE]" << std::endl;
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
  oahHelpAtom* o = new
    oahHelpAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahHelpAtom::oahHelpAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpAtom::~oahHelpAtom ()
{}

void oahHelpAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  fetchAtomUpLinkToHandler ()->
    printHelp (os);

  gIndenter.setIndentation (saveIndent);
}

void oahHelpAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHelpAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHelpAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahHelpAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    gGlobalOahOahGroup->getOahOahGroupServiceName () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahHelpAtom& elt)
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
S_oahHelpSummaryAtom oahHelpSummaryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahHelpSummaryAtom* o = new
    oahHelpSummaryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahHelpSummaryAtom::oahHelpSummaryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpSummaryAtom::~oahHelpSummaryAtom ()
{}

void oahHelpSummaryAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpSummaryAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  fetchAtomUpLinkToHandler ()->
    printOptionsSummary (os);

  gIndenter.setIndentation (saveIndent);
}

void oahHelpSummaryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHelpSummaryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHelpSummaryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahHelpSummaryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    gGlobalOahOahGroup->getOahOahGroupServiceName () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahHelpSummaryAtom& elt)
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
S_oahAboutAtom oahAboutAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahAboutAtom* o = new
    oahAboutAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahAboutAtom::oahAboutAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahAboutAtom::~oahAboutAtom ()
{}

void oahAboutAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAboutAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printAbout (os);

  gIndenter.setIndentation (saveIndent);
}

void oahAboutAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAboutAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAboutAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAboutAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAboutAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahAboutAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
std::string oahVersionKindAsString (
  oahVersionKind versionKind)
{
  std::string result;

  switch (versionKind) {
    case oahVersionKind::kOahVersionShort:
      result = "kOahVersionShort";
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
  oahVersionAtom* o = new
    oahVersionAtom (
      longName,
      shortName,
      description,
      serviceName,
      versionKind);
  assert (o != nullptr);
  return o;
}

oahVersionAtom::oahVersionAtom (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  const std::string&  serviceName,
  oahVersionKind      versionKind)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{
  fVersionKind = versionKind;
}

oahVersionAtom::~oahVersionAtom ()
{}

void oahVersionAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahVersionAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  switch (fVersionKind) {
    case oahVersionKind::kOahVersionShort:
      printVersionShort (os);
      break;
    case oahVersionKind::kOahVersionFull:
      printVersionFull (os);
      break;
  } // switch

  gIndenter.setIndentation (saveIndent);
}

void oahVersionAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahVersionAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahVersionAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahVersionAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahVersionAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahVersionAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");

  handlerMultiComponent->
    printVersionFull (os);
}

std::ostream& operator << (std::ostream& os, const S_oahVersionAtom& elt)
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
S_oahLibraryVersionAtom oahLibraryVersionAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahLibraryVersionAtom* o = new
    oahLibraryVersionAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahLibraryVersionAtom::oahLibraryVersionAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryVersionAtom::~oahLibraryVersionAtom ()
{}

void oahLibraryVersionAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahLibraryVersionAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printVersion (os);

  gIndenter.setIndentation (saveIndent);
}

void oahLibraryVersionAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryVersionAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLibraryVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryVersionAtom>*> (v)) {
        S_oahLibraryVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLibraryVersionAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLibraryVersionAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryVersionAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLibraryVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryVersionAtom>*> (v)) {
        S_oahLibraryVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLibraryVersionAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLibraryVersionAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryVersionAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahLibraryVersionAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");

  createLibraryComponent ()->
    printVersion (os);
}

std::ostream& operator << (std::ostream& os, const S_oahLibraryVersionAtom& elt)
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
S_oahHistoryAtom oahHistoryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahHistoryAtom* o = new
    oahHistoryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahHistoryAtom::oahHistoryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHistoryAtom::~oahHistoryAtom ()
{}

void oahHistoryAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahHistoryAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printHistory (os);

  gIndenter.setIndentation (saveIndent);
}

void oahHistoryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHistoryAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahHistoryAtom>*> (v)) {
        S_oahHistoryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHistoryAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHistoryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHistoryAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahHistoryAtom>*> (v)) {
        S_oahHistoryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHistoryAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHistoryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHistoryAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahHistoryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");

  handlerMultiComponent->
    printHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_oahHistoryAtom& elt)
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
S_oahLibraryHistoryAtom oahLibraryHistoryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahLibraryHistoryAtom* o = new
    oahLibraryHistoryAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahLibraryHistoryAtom::oahLibraryHistoryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryHistoryAtom::~oahLibraryHistoryAtom ()
{}

void oahLibraryHistoryAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahLibraryHistoryAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printHistory (os);

  gIndenter.setIndentation (saveIndent);
}

void oahLibraryHistoryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryHistoryAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLibraryHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryHistoryAtom>*> (v)) {
        S_oahLibraryHistoryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLibraryHistoryAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLibraryHistoryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryHistoryAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLibraryHistoryAtom>*
    p =
      dynamic_cast<visitor<S_oahLibraryHistoryAtom>*> (v)) {
        S_oahLibraryHistoryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLibraryHistoryAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLibraryHistoryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLibraryHistoryAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahLibraryHistoryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handlerMultiComponent != nullptr,
    "handlerMultiComponent is null");

  createLibraryComponent ()->
    printHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_oahLibraryHistoryAtom& elt)
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
S_oahContactAtom oahContactAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahContactAtom* o = new
    oahContactAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahContactAtom::oahContactAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahContactAtom::~oahContactAtom ()
{}

void oahContactAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahContactAtom" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printContact (os);

  gIndenter.setIndentation (saveIndent);
}

void oahContactAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahContactAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahContactAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahContactAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahContactAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahContactAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
        fHelpAtomWithoutAValueServiceName),
    os);
}

std::ostream& operator << (std::ostream& os, const S_oahContactAtom& elt)
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
S_oahDisplayPrefixes oahDisplayPrefixes::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahDisplayPrefixes* o = new
    oahDisplayPrefixes (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahDisplayPrefixes::oahDisplayPrefixes (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplayPrefixes::~oahDisplayPrefixes ()
{}

void oahDisplayPrefixes::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahDisplayPrefixes" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printPrefixes (os);

  gIndenter.setIndentation (saveIndent);
}

void oahDisplayPrefixes::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayPrefixes::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayPrefixes>*
    p =
      dynamic_cast<visitor<S_oahDisplayPrefixes>*> (v)) {
        S_oahDisplayPrefixes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayPrefixes::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayPrefixes::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayPrefixes::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayPrefixes>*
    p =
      dynamic_cast<visitor<S_oahDisplayPrefixes>*> (v)) {
        S_oahDisplayPrefixes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayPrefixes::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayPrefixes::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayPrefixes::browseData ()" <<
      std::endl;
  }
#endif
}

void oahDisplayPrefixes::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    os << "[NONE]" << std::endl;
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
  oahDisplaySingleCharacterOptions* o = new
    oahDisplaySingleCharacterOptions (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahDisplaySingleCharacterOptions::oahDisplaySingleCharacterOptions (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplaySingleCharacterOptions::~oahDisplaySingleCharacterOptions ()
{}

void oahDisplaySingleCharacterOptions::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahDisplaySingleCharacterOptions" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  printSingleCharacterOptions (os);

  gIndenter.setIndentation (saveIndent);
}

void oahDisplaySingleCharacterOptions::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplaySingleCharacterOptions>*
    p =
      dynamic_cast<visitor<S_oahDisplaySingleCharacterOptions>*> (v)) {
        S_oahDisplaySingleCharacterOptions elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplaySingleCharacterOptions::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplaySingleCharacterOptions::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplaySingleCharacterOptions>*
    p =
      dynamic_cast<visitor<S_oahDisplaySingleCharacterOptions>*> (v)) {
        S_oahDisplaySingleCharacterOptions elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplaySingleCharacterOptions::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplaySingleCharacterOptions::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplaySingleCharacterOptions::browseData ()" <<
      std::endl;
  }
#endif
}

void oahDisplaySingleCharacterOptions::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    os << "[NONE]" << std::endl;
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
  oahOnOffAtom* o = new
    oahOnOffAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      onOffKindVariable);
  assert (o != nullptr);
  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOnOffAtom" <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s <<
      "Ill-formed value for option " <<
      fetchNamesBetweenQuotes () <<
      ", \"on\", \"off\" or \"unknown\" expected";

    oahError (s.str ());
  }
}

void oahOnOffAtom::setOnOffKindVariable (mfOnOffKind value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    if (fLongName == K_TRACE_OAH_LONG_OPTION_NAME) {
      // get the options handler
      S_oahHandler
        handler =
          fetchAtomUpLinkToHandler (); // JMI

      gLogStream <<
        "Setting option '" <<
        fetchNames () <<
        "' onOffKind variable to '" <<
        value <<
        "'" <<
        std::endl;
    }
  }
#endif

  fOnOffKindVariable = value;
  fSetByAnOption = true;
}

void oahOnOffAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOnOffAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOnOffAtom>*
    p =
      dynamic_cast<visitor<S_oahOnOffAtom>*> (v)) {
        S_oahOnOffAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOnOffAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOnOffAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOnOffAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOnOffAtom>*
    p =
      dynamic_cast<visitor<S_oahOnOffAtom>*> (v)) {
        S_oahOnOffAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOnOffAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOnOffAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOnOffAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahOnOffAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OnOffAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOnOffKindVariable" << ": " <<
    fOnOffKindVariable <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSetByAnOption" << ": " <<
    fSetByAnOption <<
    std::endl;

  --gIndenter;
}

void oahOnOffAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fOnOffKindVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahOnOffAtom& elt)
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
S_oahBooleanAtom oahBooleanAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
{
  oahBooleanAtom* o = new
    oahBooleanAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable);
  assert (o != nullptr);
  return o;
}

oahBooleanAtom::oahBooleanAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout), // kElementValueImplicit ??? JMI
    fVariableName (
      variableName),
    fBooleanVariable (
      booleanVariable)
{}

oahBooleanAtom::~oahBooleanAtom ()
{}

void oahBooleanAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahBooleanAtom" <<
      std::endl;
  }
#endif

  setBooleanVariable (true);
}

void oahBooleanAtom::setBooleanVariable (Bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    if (fLongName == K_TRACE_OAH_LONG_OPTION_NAME) {
      // get the options handler
      S_oahHandler
        handler =
          fetchAtomUpLinkToHandler (); // JMI

      gLogStream <<
        "Setting option '" <<
        fetchNames () <<
        "' boolean variable to '" <<
        value <<
        "'" <<
        std::endl;
    }
  }
#endif

  fBooleanVariable = value;
  fSetByAnOption = true;
}

void oahBooleanAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahBooleanAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahBooleanAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahBooleanAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    "fSetByAnOption" << ": " <<
    fSetByAnOption <<
    std::endl;

  --gIndenter;
}

void oahBooleanAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahBooleanAtom& elt)
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
S_oahBooleanAtomWithTracePasses oahBooleanAtomWithTracePasses::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
{
  oahBooleanAtomWithTracePasses* o = new
    oahBooleanAtomWithTracePasses (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable);
  assert (o != nullptr);
  return o;
}

oahBooleanAtomWithTracePasses::oahBooleanAtomWithTracePasses (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  Bool&              booleanVariable)
  : oahBooleanAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable)
{}

oahBooleanAtomWithTracePasses::~oahBooleanAtomWithTracePasses ()
{}

void oahBooleanAtomWithTracePasses::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahBooleanAtomWithTracePasses" <<
      std::endl;
  }
#endif

  setBooleanVariable (true);

  gGlobalOahEarlyOptions.setEarlyTracePasses ();
}

void oahBooleanAtomWithTracePasses::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtomWithTracePasses::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahBooleanAtomWithTracePasses>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtomWithTracePasses>*> (v)) {
        S_oahBooleanAtomWithTracePasses elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtomWithTracePasses::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahBooleanAtomWithTracePasses::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtomWithTracePasses::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahBooleanAtomWithTracePasses>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtomWithTracePasses>*> (v)) {
        S_oahBooleanAtomWithTracePasses elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtomWithTracePasses::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahBooleanAtomWithTracePasses::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtomWithTracePasses::browseData ()" <<
      std::endl;
  }
#endif
}

void oahBooleanAtomWithTracePasses::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    "fSetByAnOption" << ": " <<
    fSetByAnOption <<
    std::endl;

  --gIndenter;
}

void oahBooleanAtomWithTracePasses::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahBooleanAtomWithTracePasses& elt)
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
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom)
{
  oahTwoBooleansAtom* o = new
    oahTwoBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom);
  assert (o != nullptr);
  return o;
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

void oahTwoBooleansAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahTwoBooleansAtom" <<
      std::endl;
  }
#endif

  setTwoBooleansVariables (true);
}

void oahTwoBooleansAtom::setTwoBooleansVariables (Bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' two booleans variables to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fBooleanVariable = value;

  fSecondBooleanAtom->setBooleanVariable (value);

  fSetByAnOption = true;
}

void oahTwoBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahTwoBooleansAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahTwoBooleansAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahTwoBooleansAtom& elt)
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
S_oahTwoBooleansAtomWithTracePasses oahTwoBooleansAtomWithTracePasses::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom)
{
  oahTwoBooleansAtomWithTracePasses* o = new
    oahTwoBooleansAtomWithTracePasses (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom);
  assert (o != nullptr);
  return o;
}

oahTwoBooleansAtomWithTracePasses::oahTwoBooleansAtomWithTracePasses (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom)
  : oahTwoBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom)
{}

oahTwoBooleansAtomWithTracePasses::~oahTwoBooleansAtomWithTracePasses ()
{}

void oahTwoBooleansAtomWithTracePasses::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahTwoBooleansAtomWithTracePasses" <<
      std::endl;
  }
#endif

  setTwoBooleansVariables (true);

  gGlobalOahEarlyOptions.setEarlyTracePasses ();
}

void oahTwoBooleansAtomWithTracePasses::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtomWithTracePasses::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtomWithTracePasses>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtomWithTracePasses>*> (v)) {
        S_oahTwoBooleansAtomWithTracePasses elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtomWithTracePasses::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoBooleansAtomWithTracePasses::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtomWithTracePasses::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtomWithTracePasses>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtomWithTracePasses>*> (v)) {
        S_oahTwoBooleansAtomWithTracePasses elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtomWithTracePasses::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoBooleansAtomWithTracePasses::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtomWithTracePasses::browseData ()" <<
      std::endl;
  }
#endif
}

void oahTwoBooleansAtomWithTracePasses::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "TwoBooleansAtomWithTracePasses:" <<
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

void oahTwoBooleansAtomWithTracePasses::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahTwoBooleansAtomWithTracePasses& elt)
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
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom)
{
  oahThreeBooleansAtom* o = new
    oahThreeBooleansAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable,
      secondBooleanAtom,
      thirdBooleanAtom);
  assert (o != nullptr);
  return o;
}

oahThreeBooleansAtom::oahThreeBooleansAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  Bool&                   booleanVariable,
  const S_oahBooleanAtom& secondBooleanAtom,
  const S_oahBooleanAtom& thirdBooleanAtom)
  : oahBooleanAtom (
      longName,
      shortName,
      description,
      variableName,
      booleanVariable),
    fSecondBooleanAtom (
      secondBooleanAtom),
    fThirdBooleanAtom (
      thirdBooleanAtom)
{}

oahThreeBooleansAtom::~oahThreeBooleansAtom ()
{}

void oahThreeBooleansAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahThreeBooleansAtom" <<
      std::endl;
  }
#endif

  setThreeBooleansVariables (true);
}

void oahThreeBooleansAtom::setThreeBooleansVariables (Bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' three booleans variables to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fBooleanVariable = value;

  fSecondBooleanAtom->setBooleanVariable (value);
  fThirdBooleanAtom->setBooleanVariable (value);

  fSetByAnOption = true;
}

void oahThreeBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahThreeBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahThreeBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahThreeBooleansAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahThreeBooleansAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fBooleanVariable;

  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahThreeBooleansAtom& elt)
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
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahCombinedBooleansAtom* o = new
    oahCombinedBooleansAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

oahCombinedBooleansAtom::oahCombinedBooleansAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::addBooleanAtom (
  const S_oahBooleanAtom& booleanAtom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    booleanAtom != nullptr,
    "booleanAtom is null");

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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // is name known in options std::map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the std::map
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' combined boolean variables to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  // set the value of the atoms in the std::list
  if (fBooleanAtomsList.size ()) {
    for (
      std::list<S_oahBooleanAtom>::const_iterator i =
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

void oahCombinedBooleansAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCombinedBooleansAtom" <<
      std::endl;
  }
#endif

  setCombinedBooleanVariables (true);
}

void oahCombinedBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahCombinedBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahCombinedBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::browseData ()" <<
      std::endl;
  }
#endif

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      std::list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      const S_oahBooleanAtom& booleanAtom = (*i);

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

void oahCombinedBooleansAtom::printCombinedBooleansEssentialsShort (
  std::ostream& os,
  int fieldWidth) const
{
/* JMI
  printAtomWithVariableNameEssentialsShort (
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
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

    os << "'";

    std::list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << "'";

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

    std::list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      const S_oahBooleanAtom& booleanAtom = (*i);

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
}

void oahCombinedBooleansAtom::printAtomWithVariableOptionsValues (
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
    std::list<S_oahBooleanAtom>::const_iterator
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
          printAtomWithVariableOptionsValues (
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
    os << "[NONE]" << std::endl;
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
  oahCommonPrefixBooleansAtom* o = new
    oahCommonPrefixBooleansAtom (
      longName,
      shortName,
      description,
      shortSuffixDescriptor,
      longSuffixDescriptor,
      shortNamesPrefix,
      longNamesPrefix);
  assert (o != nullptr);
  return o;
}

oahCommonPrefixBooleansAtom::oahCommonPrefixBooleansAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& shortSuffixDescriptor,
  const std::string& longSuffixDescriptor,
  const S_oahPrefix& shortNamesPrefix,
  const S_oahPrefix& longNamesPrefix)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    shortNamesPrefix != nullptr,
    "shortNamesPrefix is null");
  mfAssert (
    __FILE__, __LINE__,
    longNamesPrefix != nullptr,
    "fLongNamesPrefix is null");

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
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    booleanAtom != nullptr,
    "booleanAtom is null");

  // long name consistency check
  {
    std::string booleanAtomLongName =
      booleanAtom->getLongName ();

    if (booleanAtomLongName.size ()) {
      std::size_t found =
        booleanAtomLongName.find (fLongNamesPrefixName);

      if (found == std::string::npos) {
        std::stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" is different than the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          std::endl;

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else if (found != 0) {
        std::stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" doesn't start by the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          std::endl;

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else {
        std::string booleanAtomLongNameSuffix =
          booleanAtomLongName.substr (
            fLongNamesPrefixName.size ());

        if (booleanAtomLongNameSuffix.size () == 0) {
          std::stringstream s;

          s <<
            "Option long name \"" <<
            booleanAtomLongName <<
            "\" is nothing more than the long names prefix name \"" <<
            fLongNamesPrefixName <<
            "\"" <<
          std::endl;

          booleanAtom->print (s);

          oahError (s.str ());
        }
        else {
          // register this boolean atom's suffix in the std::list
          fLongNamesSuffixes.push_back (booleanAtomLongNameSuffix);
        }
      }
    }

    else {
      std::stringstream s;

      s <<
        "Option long name \"" <<
        booleanAtomLongName <<
        "\" is empty, atom \"" <<
        fLongNamesPrefixName <<
        "\" cannot be used in a multiplex booleans atom";

      booleanAtom->print (s);

      oahError (s.str ());
    }
  }

  // short name consistency check
  {
    std::string booleanAtomShortName =
      booleanAtom->getShortName ();

    std::size_t found =
      booleanAtomShortName.find (fShortNamesPrefixName);

    if (found == std::string::npos) {
      std::stringstream s;

      s <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" is different than the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (s);

      oahError (s.str ());
    }

    else if (found != 0) {
      std::stringstream s;

      s <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" doesn't start by the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (s);

      oahError (s.str ());
    }

    else {
      std::string booleanAtomShortNameSuffix =
        booleanAtomShortName.substr (
          fShortNamesPrefixName.size ());

      if (booleanAtomShortNameSuffix.size () == 0) {
        std::stringstream s;

        s <<
          "Option short name \"" <<
          booleanAtomShortName <<
          "\" is nothing more than the short names prefix name \"" <<
          fShortNamesPrefixName <<
          "\"";

        booleanAtom->print (s);

        oahError (s.str ());
      }
      else {
        // register this boolean atom's suffix in the std::list
        fShortNamesSuffixes.push_back (booleanAtomShortNameSuffix);
      }
    }
  }

  // append the boolean atom to the std::list
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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // is name known in options std::map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the std::map
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

void oahCommonPrefixBooleansAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCommonPrefixBooleansAtom" <<
      std::endl;
  }
#endif

  // handle it at once JMI ???
}

void oahCommonPrefixBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahCommonPrefixBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCommonPrefixBooleansAtom>*> (v)) {
        S_oahCommonPrefixBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCommonPrefixBooleansAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahCommonPrefixBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahCommonPrefixBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCommonPrefixBooleansAtom>*> (v)) {
        S_oahCommonPrefixBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCommonPrefixBooleansAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahCommonPrefixBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCommonPrefixBooleansAtom::browseData ()" <<
      std::endl;
  }
#endif

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
      std::list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      const S_oahBooleanAtom& booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahCommonPrefixBooleansAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    os << "null" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "longNamesPrefix" << ": ";
  if (fLongNamesPrefix) {
    os << fLongNamesPrefix;
  }
  else {
    os << "null" << std::endl;
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

    std::list<S_oahBooleanAtom>::const_iterator
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

    std::list<std::string>::const_iterator
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
    int longNamesSuffixesCount = 0;

    {
      std::list<std::string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      for ( ; ; ) {
        if ((*i).size ()) {
          ++longNamesSuffixesCount;
        }

        if (++i == iEnd) break;
      } // for
    }

    os <<
      "The " <<
      fLongNamesSuffixes.size () <<
 // JMI     " -- " << longNamesSuffixesCount <<
      " known " << fLongSuffixDescriptor << "s are: ";

    if (fLongNamesSuffixes.size ()) {
      os << std::endl;
      ++gIndenter;

      std::list<std::string>::const_iterator
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
}

void oahCommonPrefixBooleansAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the boolean atoms in the std::list
}

std::ostream& operator << (std::ostream& os, const S_oahCommonPrefixBooleansAtom& elt)
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
S_oahIntegerAtom oahIntegerAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable)
{
  oahIntegerAtom* o = new
    oahIntegerAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      integerVariable);
  assert (o != nullptr);
  return o;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream s;
      s << theString;
      s >> integerValue;
    }

    setIntegerVariable (integerValue);
  }

  else {
    std::stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option \"" << fetchNames () <<
      "\" is ill-formed";

    oahError (s.str ());
  }
}

void oahIntegerAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fIntegerVariable = value;
  fSetByAnOption = true;
}

void oahIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntegerAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntegerAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahIntegerAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fIntegerVariable;

  return s.str ();
}

std::string oahIntegerAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fIntegerVariable;

  return s.str ();
}

void oahIntegerAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahIntegerAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fIntegerVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahIntegerAtom& elt)
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
S_oahTwoIntegersAtom oahTwoIntegersAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  int&               integerVariable,
  int&               integerSecondaryVariable)
{
  oahTwoIntegersAtom* o = new
    oahTwoIntegersAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      integerVariable,
      integerSecondaryVariable);
  assert (o != nullptr);
  return o;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream s;
      s << sm [ 1 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;

    {
      std::stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerSecondaryVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    std::stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahTwoIntegersAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' two integers variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fIntegerVariable = value;
  fSetByAnOption = true;
}

void oahTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahTwoIntegersAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

std::string oahTwoIntegersAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

void oahTwoIntegersAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahTwoIntegersAtom::printAtomWithVariableOptionsValues (
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

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahTwoIntegersAtom& elt)
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
S_oahFloatAtom oahFloatAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  float&             floatVariable)
{
  oahFloatAtom* o = new
    oahFloatAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      floatVariable);
  assert (o != nullptr);
  return o;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for float std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    float floatValue;
    {
      std::stringstream s;

      s << sm [ 0 ];
      s >> floatValue;
    }

    setFloatVariable (floatValue);
  }

  else {
    std::stringstream s;

    s <<
      "float value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahFloatAtom::setFloatVariable (float value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' float variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fFloatVariable = value;
  fSetByAnOption = true;
}

void oahFloatAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFloatAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFloatAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFloatAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFloatAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahFloatAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fFloatVariable;

  return s.str ();
}

std::string oahFloatAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fFloatVariable;

  return s.str ();
}

void oahFloatAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahFloatAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fFloatVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahFloatAtom& elt)
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
S_oahStringAtom oahStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  oahStringAtom* o = new
    oahStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' std::string variable to \"" <<
     value <<
      "\"" <<
      std::endl;
  }
#endif

  fStringVariable = value;
  fSetByAnOption = true;
}

void oahStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << " '" << fStringVariable << "'";

  return s.str ();
}

std::string oahStringAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << " '" << fStringVariable << "'";

  return s.str ();
}

void oahStringAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahStringAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    '[' << fStringVariable << ']';

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAtom& elt)
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
S_oahFactorizedStringAtom oahFactorizedStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& atomNameDescriptor,
  const std::string& stringValueDescriptor)
{
  oahFactorizedStringAtom* o = new
    oahFactorizedStringAtom (
      longName,
      shortName,
      description,
      atomNameDescriptor,
      stringValueDescriptor);
  assert (o != nullptr);
  return o;
}

oahFactorizedStringAtom::oahFactorizedStringAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& atomNameDescriptor,
  const std::string& stringValueDescriptor)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout),
    fAtomNameDescriptor (
      atomNameDescriptor),
    fStringValueDescriptor (
      stringValueDescriptor)
{
  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
  mfAssert (
    __FILE__, __LINE__,
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
}

oahFactorizedStringAtom::~oahFactorizedStringAtom ()
{}

void oahFactorizedStringAtom::addStringAtom (
  const S_oahStringAtom& stringAtom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stringAtom != nullptr,
    "stringAtom is null");

  // atom long name consistency check
  std::string stringAtomLongName =
    stringAtom->getLongName ();

  if (stringAtomLongName.size () == 0) {
    std::stringstream s;

    s <<
      "option long name \"" << stringAtomLongName <<
      "\" is empty";

    stringAtom->print (s);

    oahError (s.str ());
  }

  // atom short name consistency check
  std::string stringAtomShortName =
    stringAtom->getShortName ();

  if (stringAtomShortName.size () == 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyOahVerboseMode ()) {
      std::stringstream s;

      s <<
        "Option short name \"" << stringAtomShortName << "\"" <<
        " for long name \"" << stringAtomLongName << "\"" <<
        " is empty";

      oahWarning (s.str ());
    }
#endif
  }
  else {
    // register this std::string atom's suffix in the std::list
    fAtomNamesList.push_back (stringAtomShortName);
  }

  // append the std::string atom to the std::list
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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // is name known in options std::map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the std::map
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
      // std::string atom?
      S_oahStringAtom
        atom =
          dynamic_cast<oahStringAtom*>(&(*element))
      ) {
      // add the std::string atom
      addStringAtom (atom);
    }

    else {
      handler->
        unknownOptionNameError (
          name,
          "in factorized std::string atom, not the name of an atom");
    }
  }
}

void oahFactorizedStringAtom::applyElement (std::ostream& os) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying monoplex std::string atom '" <<
      fetchNames () <<
      "' which is a oahFactorizedStringAtom" <<
      std::endl;
  }
#endif

  // handle it at once JMI ???
}

void oahFactorizedStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFactorizedStringAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFactorizedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFactorizedStringAtom>*> (v)) {
        S_oahFactorizedStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFactorizedStringAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFactorizedStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFactorizedStringAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFactorizedStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFactorizedStringAtom>*> (v)) {
        S_oahFactorizedStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFactorizedStringAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFactorizedStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFactorizedStringAtom::browseData ()" <<
      std::endl;
  }
#endif

  // browse the std::string atoms
  if (fStringAtomsList.size ()) {
    for (
      std::list<S_oahStringAtom>::const_iterator i = fStringAtomsList.begin ();
      i != fStringAtomsList.end ();
      ++i
    ) {
      S_oahStringAtom stringAtom = (*i);

      // browse the std::string atom
      oahBrowser<oahStringAtom> browser (v);
      browser.browse (*(stringAtom));
    } // for
  }
}

void oahFactorizedStringAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

    os << "'";

    std::list<S_oahStringAtom>::const_iterator
      iBegin = fStringAtomsList.begin (),
      iEnd   = fStringAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << "'";

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

    std::list<std::string>::const_iterator
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

void oahFactorizedStringAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the std::string atoms in the std::list
}

std::ostream& operator << (std::ostream& os, const S_oahFactorizedStringAtom& elt)
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
S_oahStringWithDefaultValueAtom oahStringWithDefaultValueAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultStringValue)
{
  oahStringWithDefaultValueAtom* o = new
    oahStringWithDefaultValueAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultStringValue);
  assert (o != nullptr);
  return o;
}

oahStringWithDefaultValueAtom::oahStringWithDefaultValueAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultStringValue)
  : oahStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable),
    fDefaultStringValue (
      defaultStringValue)
{
  this->setElementValueKind (oahElementValueKind::kElementValueOptional);
}

oahStringWithDefaultValueAtom::~oahStringWithDefaultValueAtom ()
{}

void oahStringWithDefaultValueAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahStringWithDefaultValueAtom" <<
      std::endl;
  }
#endif

  oahStringAtom::applyAtomWithValue (
    theString,
    os);
}

void oahStringWithDefaultValueAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithDefaultValueAtom::setStringVariable (const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' std::string variable to \"" <<
     value <<
      "\"" <<
      std::endl;
  }
#endif

  oahStringAtom::setStringVariable (value);
  fSetByAnOption = true;
}

void oahStringWithDefaultValueAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringWithDefaultValueAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringWithDefaultValueAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fStringVariable;

  return s.str ();
}

std::string oahStringWithDefaultValueAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fStringVariable;

  return s.str ();
}

void oahStringWithDefaultValueAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStringVariable" << ": " <<
    "\"" << fStringVariable << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fDefaultStringValue" << ": " <<
    "\"" << fDefaultStringValue << "\"" <<
    std::endl;

  --gIndenter;
}

void oahStringWithDefaultValueAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fStringVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringWithDefaultValueAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringWithRegexAtom oahStringWithRegexAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& regexString)
{
  oahStringWithRegexAtom* o = new
    oahStringWithRegexAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      regexString);
  assert (o != nullptr);
  return o;
}

oahStringWithRegexAtom::oahStringWithRegexAtom (
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
{
// JMI  this->setElementValueKind (oahElementValueKind::kElementValueOptional);
}

oahStringWithRegexAtom::~oahStringWithRegexAtom ()
{}

void oahStringWithRegexAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahStringWithRegexAtom" <<
      std::endl;
  }
#endif

  // JMI to do!

  oahStringAtom::applyAtomWithValue (
    theString,
    os);
}

void oahStringWithRegexAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithRegexAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringWithRegexAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithRegexAtom>*> (v)) {
        S_oahStringWithRegexAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithRegexAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithRegexAtom::setStringVariable (const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' std::string variable to \"" <<
     value <<
      "\"" <<
      std::endl;
  }
#endif

  oahStringAtom::setStringVariable (value);
  fSetByAnOption = true;
}

void oahStringWithRegexAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithRegexAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringWithRegexAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithRegexAtom>*> (v)) {
        S_oahStringWithRegexAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithRegexAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringWithRegexAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithRegexAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringWithRegexAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fStringVariable;

  return s.str ();
}

std::string oahStringWithRegexAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fStringVariable;

  return s.str ();
}

void oahStringWithRegexAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahStringWithRegexAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fStringVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringWithRegexAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    std::endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
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
  Rational&          rationalVariable)
{
  oahRationalAtom* o = new
    oahRationalAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      rationalVariable);
  assert (o != nullptr);
  return o;
}

oahRationalAtom::oahRationalAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  Rational&          rationalVariable)
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Rational std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 3) { // JMI ???
    int
      numerator,
      denominator;

    {
      std::stringstream s;
      s << sm [1];
      s >> numerator;
    }
    {
      std::stringstream s;
      s << sm [2];
      s >> denominator;
    }

    Rational
      rationalValue =
        Rational (numerator, denominator);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "rationalValue = " <<
        rationalValue <<
        std::endl;
    }
#endif

    setRationalVariable (rationalValue);
  }

  else {
    std::stringstream s;

    s <<
      "Rational atom value \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }
}

void oahRationalAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRationalAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRationalAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRationalAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRationalAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahRationalAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fRationalVariable;

  return s.str ();
}

std::string oahRationalAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fRationalVariable;

  return s.str ();
}

void oahRationalAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahRationalAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fRationalVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahRationalAtom& elt)
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
S_oahNaturalNumbersSetElementAtom oahNaturalNumbersSetElementAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set<int>&     naturalNumbersSetVariable)
{
  oahNaturalNumbersSetElementAtom* o = new
    oahNaturalNumbersSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      naturalNumbersSetVariable);
  assert (o != nullptr);
  return o;
}

oahNaturalNumbersSetElementAtom::oahNaturalNumbersSetElementAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set<int>&     naturalNumbersSetVariable)
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
}

void oahNaturalNumbersSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahNaturalNumbersSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetElementAtom>*> (v)) {
        S_oahNaturalNumbersSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahNaturalNumbersSetElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahNaturalNumbersSetElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahNaturalNumbersSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    '[';

  std::set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << ' ';
  } // for

  s <<
    ']';

  return s.str ();
}

std::string oahNaturalNumbersSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    '[';

  std::set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << ' ';
  } // for

  s <<
    ']';

  return s.str ();
}

void oahNaturalNumbersSetElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
      "'";

    std::set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os <<
      "'";
  }

  os << std::endl;

  --gIndenter;
}

void oahNaturalNumbersSetElementAtom::printAtomWithVariableOptionsValues (
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
    if (fSetByAnOption) {
      os <<
        "set by an option" <<
        std::endl;
    }

    std::set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ' ';
    } // for

    os << "'";
  }
}

std::ostream& operator << (std::ostream& os, const S_oahNaturalNumbersSetElementAtom& elt)
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
S_oahColorRGBAtom oahColorRGBAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrColorRGB&       colorRGBVariable)
{
  oahColorRGBAtom* o = new
    oahColorRGBAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      colorRGBVariable);
  assert (o != nullptr);
  return o;
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
}

void oahColorRGBAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahColorRGBAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahColorRGBAtom>*
    p =
      dynamic_cast<visitor<S_oahColorRGBAtom>*> (v)) {
        S_oahColorRGBAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahColorRGBAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahColorRGBAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahColorRGBAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahColorRGBAtom>*
    p =
      dynamic_cast<visitor<S_oahColorRGBAtom>*> (v)) {
        S_oahColorRGBAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahColorRGBAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahColorRGBAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahColorRGBAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahColorRGBAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    '[' <<
    fColorRGBVariable.asString () <<
    ']';

  return s.str ();
}

std::string oahColorRGBAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    '[' <<
    fColorRGBVariable.asString () <<
    ']';

  return s.str ();
}

void oahColorRGBAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahColorRGBAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fColorRGBVariable.asString ();
  if (fSetByAnOption) {
    os <<
      ", set by an option" <<
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
    os << "[NONE]" << std::endl;
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
  std::set<int>&     intSetVariable)
{
  oahIntSetElementAtom* o = new
    oahIntSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      intSetVariable);
  assert (o != nullptr);
  return o;
}

oahIntSetElementAtom::oahIntSetElementAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set<int>&     intSetVariable)
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling atom '" <<
      fetchNames () <<
      "' of type 'oahIntSetElementAtom'" <<
      " with value \"" << theString << "\"" <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "([[:digit:]]+)"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (smSize != 2) {
    std::stringstream s;

    s <<
      "-boxAroundBarNumber argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  int lilypondMeasureNumber;
  {
    std::stringstream s;
    s << sm [1];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      std::endl;
  }
#endif

  fIntSetVariable.insert (lilypondMeasureNumber);
  fSetByAnOption = true;
}

void oahIntSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIntSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetElementAtom>*> (v)) {
        S_oahIntSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntSetElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIntSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetElementAtom>*> (v)) {
        S_oahIntSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntSetElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntSetElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahIntSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fIntSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

std::string oahIntSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fIntSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahIntSetElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    std::set<int>::const_iterator
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

void oahIntSetElementAtom::printAtomWithVariableOptionsValues (
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

    std::set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "set by an option" <<
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
    os << "[NONE]" << std::endl;
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
  std::set<std::string>& stringSetVariable)
{
  oahStringSetElementAtom* o = new
    oahStringSetElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (o != nullptr);
  return o;
}

oahStringSetElementAtom::oahStringSetElementAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::set<std::string>& stringSetVariable)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringSetElementAtom'" <<
      std::endl;
  }
#endif

  // theString contains the name of the part to be ignored

  std::string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      std::endl;
  }
#endif

  // is this part name in the part renaming std::map?
  std::set<std::string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    std::stringstream s;

    s <<
      "part \"" << partName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
    fSetByAnOption = true;
  }
}

void oahStringSetElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetElementAtom>*> (v)) {
        S_oahStringSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringSetElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringSetElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringSetElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetElementAtom>*> (v)) {
        S_oahStringSetElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringSetElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringSetElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringSetElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::set<std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

std::string oahStringSetElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::set<std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringSetElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    std::set<std::string>::const_iterator
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

void oahStringSetElementAtom::printAtomWithVariableOptionsValues (
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
      "set by an option" <<
      std::endl;

    ++gIndenter;

    std::set<std::string>::const_iterator
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
    os << "[NONE]" << std::endl;
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
  std::map<std::string, int>& stringToIntMapVariable)
{
  oahStringToIntMapElementAtom* o = new
    oahStringToIntMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToIntMapVariable);
  assert (o != nullptr);
  return o;
}

oahStringToIntMapElementAtom::oahStringToIntMapElementAtom (
  const std::string&     shortName,
  const std::string&     longName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::map<std::string, int>& stringToIntMapVariable)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'" <<
      std::endl;
  }
#endif

  // theString contains the std::string int std::map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'" <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"     // std::string
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    std::stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  std::string musicxmlMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    std::stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "musicxmlMeasureNumber = " <<
      musicxmlMeasureNumber <<
      std::endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      std::endl;
  }
#endif

  fStringToIntMapVariable [musicxmlMeasureNumber] =
    lilypondMeasureNumber;
  fSetByAnOption = true;
}

void oahStringToIntMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToIntMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapElementAtom>*> (v)) {
        S_oahStringToIntMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToIntMapElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringToIntMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToIntMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapElementAtom>*> (v)) {
        S_oahStringToIntMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToIntMapElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringToIntMapElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringToIntMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::map<std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

std::string oahStringToIntMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::map<std::string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringToIntMapElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    std::map<std::string, int>::const_iterator
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

void oahStringToIntMapElementAtom::printAtomWithVariableOptionsValues (
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

    std::map<std::string, int>::const_iterator
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
      "set by an option" <<
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
    os << "[NONE]" << std::endl;
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
  std::map<std::string, std::string>& stringToStringMapVariable)
{
  oahStringToStringMapElementAtom* o = new
    oahStringToStringMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (o != nullptr);
  return o;
}

oahStringToStringMapElementAtom::oahStringToStringMapElementAtom (
  const std::string&                  longName,
  const std::string&                  shortName,
  const std::string&                  description,
  const std::string&                  valueSpecification,
  const std::string&                  variableName,
  std::map<std::string, std::string>& stringToStringMapVariable)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'" <<
      std::endl;
  }
#endif

  // theString contains the std::string std::string std::map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'" <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // std::string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // std::string
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    std::stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  std::string
    key   = sm [1],
    value = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "key = " <<
      key <<
      std::endl <<
      "value = " <<
      value <<
      std::endl;
  }
#endif

  fStringToStringMapVariable [key] =
    value;
  fSetByAnOption = true;
}

void oahStringToStringMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMapElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToStringMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMapElementAtom>*> (v)) {
        S_oahStringToStringMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToStringMapElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringToStringMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMapElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToStringMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMapElementAtom>*> (v)) {
        S_oahStringToStringMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToStringMapElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringToStringMapElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMapElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringToStringMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::map<std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

std::string oahStringToStringMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::map<std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringToStringMapElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

    std::map<std::string, std::string>::const_iterator
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

void oahStringToStringMapElementAtom::printAtomWithVariableOptionsValues (
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

    std::map<std::string, std::string>::const_iterator
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
      "set by an option" <<
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
    os << "[NONE]" << std::endl;
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
  std::multimap<std::string, std::string>& stringToStringMultiMapVariable)
{
  oahStringToStringMultiMapElementAtom* o = new
    oahStringToStringMultiMapElementAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMultiMapVariable);
  assert (o != nullptr);
  return o;
}

oahStringToStringMultiMapElementAtom::oahStringToStringMultiMapElementAtom (
  const std::string&                       longName,
  const std::string&                       shortName,
  const std::string&                       description,
  const std::string&                       valueSpecification,
  const std::string&                       variableName,
  std::multimap<std::string, std::string>& stringToStringMultiMapVariable)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'" <<
      std::endl;
  }
#endif

  // theString contains the std::string std::string std::multimap specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'" <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // std::string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // std::string
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    std::stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  std::string
    key   = sm [1],
    value = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "key = " <<
      key <<
      std::endl <<
      "value = " <<
      value <<
      std::endl;
  }
#endif

  fStringToStringMultiMapVariable.insert (
    std::pair<std::string, std::string> (key, value));
  fSetByAnOption = true;
}

void oahStringToStringMultiMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToStringMultiMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMultiMapElementAtom>*> (v)) {
        S_oahStringToStringMultiMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToStringMultiMapElementAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringToStringMultiMapElementAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringToStringMultiMapElementAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToStringMultiMapElementAtom>*> (v)) {
        S_oahStringToStringMultiMapElementAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToStringMultiMapElementAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringToStringMultiMapElementAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringToStringMultiMapElementAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::multimap<std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

std::string oahStringToStringMultiMapElementAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    std::multimap<std::string, std::string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringToStringMultiMapElementAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

    std::multimap<std::string, std::string>::const_iterator
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

void oahStringToStringMultiMapElementAtom::printAtomWithVariableOptionsValues (
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

    std::multimap<std::string, std::string>::const_iterator
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
      "set by an option" <<
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
    os << "[NONE]" << std::endl;
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
  oahStringAndIntegerAtom* o = new
    oahStringAndIntegerAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      stringVariableName,
      stringVariable,
      integerVariableName,
      integerVariable);
  assert (o != nullptr);
  return o;
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
  // theString contains the std::string and integer values

  // check whether it is well-formed
  std::string regularExpression (
    "(.+)"           // std::string
    "[[:space:]]+"
    ":"
    "[[:space:]]+"
    "([[:digit:]]+)" // int
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 3) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }

    fIntegerVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    std::stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndIntegerAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fIntegerVariable = value;
  fSetByAnOption = true;
}

void oahStringAndIntegerAtom::setStringVariable (const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' std::string variable to \"" <<
      value <<
      "\"" <<
      std::endl;
  }
#endif

  fStringVariable = value;
  fSetByAnOption = true;
}

void oahStringAndIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringAndIntegerAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

std::string oahStringAndIntegerAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

void oahStringAndIntegerAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahStringAndIntegerAtom::printAtomWithVariableOptionsValues (
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

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAndIntegerAtom& elt)
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
  oahStringAndTwoIntegersAtom* o = new
    oahStringAndTwoIntegersAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      primaryIntegerVariable,
      secondaryIntegerVariable);
  assert (o != nullptr);
  return o;
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
  // theString contains the std::string and integer values

  // check whether it is well-formed
  std::string regularExpression (
    "([[:alpha:]]+)" // std::string
    "[[:space:]]+"
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // secondary integer value
    );

  std::regex e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 4) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      std::stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fPrimaryIntegerVariable = integerValue; // JMI

    {
      std::stringstream s;
      s << sm [ 3 ];
      s >> integerValue;
    }
    fSecondaryIntegerVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    std::stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndTwoIntegersAtom::setStringVariable (const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' std::string variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fStringVariable = value;
  fSetByAnOption = true;
}

void oahStringAndTwoIntegersAtom::setPrimaryIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' primary integer variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fPrimaryIntegerVariable = value;
  fSetByAnOption = true;
}

void oahStringAndTwoIntegersAtom::setSecondaryIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' secondary integer variable to '" <<
      value <<
      "'" <<
      std::endl;
  }
#endif

  fSecondaryIntegerVariable = value;
  fSetByAnOption = true;
}

void oahStringAndTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahStringAndTwoIntegersAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    ' ' <<
    fSecondaryIntegerVariable;

  return s.str ();
}

std::string oahStringAndTwoIntegersAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    ' ' <<
    fSecondaryIntegerVariable;

  return s.str ();
}

void oahStringAndTwoIntegersAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahStringAndTwoIntegersAtom::printAtomWithVariableOptionsValues (
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

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahStringAndTwoIntegersAtom& elt)
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
S_oahLengthUnitKindAtom oahLengthUnitKindAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
{
  oahLengthUnitKindAtom* o = new
    oahLengthUnitKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lengthUnitKindVariable);
  assert (o != nullptr);
  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      std::endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds std::map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      std::endl;
  }
#endif

  std::map<std::string, msrLengthUnitKind>::const_iterator
    it =
      gGlobalMsrLengthUnitKindsMap.find (
        theString);

  if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
    // no, score output kind is unknown in the std::map

    std::stringstream s;

    s <<
      "LPSR score output kind \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsrLengthUnitKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsrLengthUnitKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLengthUnitKindVariable (
    (*it).second);
}

void oahLengthUnitKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthUnitKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthUnitKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahLengthUnitKindAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

std::string oahLengthUnitKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

void oahLengthUnitKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahLengthUnitKindAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable);

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahLengthUnitKindAtom& elt)
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
S_oahLengthAtom oahLengthAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrLength&         lengthVariable)
{
  oahLengthAtom* o = new
    oahLengthAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lengthVariable);
  assert (o != nullptr);
  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\"" <<
      std::endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << std::endl;
  }
#endif

  if (smSize == 4) {
    // leave the low level details to the STL...
    float floatValue;
    {
      std::stringstream s;
      // concatenate the integer and decimal parts
      s << sm [ 1 ] << sm [ 2 ];
      s >> floatValue;
    }

    std::string lengthUnitName = sm [ 3 ];

    // is lengthUnitName known in the length unit names std::map?
    std::map<std::string, msrLengthUnitKind>::const_iterator
      it =
        gGlobalMsrLengthUnitKindsMap.find (
          lengthUnitName);

    if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
      // no, length unit name is unknown in the std::map

      std::stringstream s;

      s <<
        "length unit name \"" << lengthUnitName <<
        "\" is unknown" <<
        std::endl <<
        "The " <<
        gGlobalMsrLengthUnitKindsMap.size () <<
        " known length unit names are:" <<
        std::endl;

      ++gIndenter;

      s <<
        existingMsrLengthUnitKinds (K_MF_NAMES_LIST_MAX_LENGTH);

      --gIndenter;

      oahError (s.str ());
    }

    setLengthVariable (
      msrLength (
        (*it).second,
        floatValue));
  }

  else {
    std::stringstream s;

    s <<
      "length value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahLengthAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahLengthAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fLengthVariable.asString ();

  return s.str ();
}

std::string oahLengthAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fLengthVariable.asString ();

  return s.str ();
}

void oahLengthAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahLengthAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fLengthVariable.asString ();

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahLengthAtom& elt)
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
S_oahMidiTempoAtom oahMidiTempoAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrMidiTempo&      midiTempoVariable)
{
  oahMidiTempoAtom* o = new
    oahMidiTempoAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      midiTempoVariable);
  assert (o != nullptr);
  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting midi tempo atom variable to " <<
      value.asString () <<
      std::endl;
  }
#endif

  fMidiTempoVariable = value;
  fSetByAnOption = true;
}

void oahMidiTempoAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      std::endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // midiTempoDuration
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    std::stringstream s;

    s <<
      "-midiTempo argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  std::string midiTempoDuration = sm [1];

  int midiTempoPerSecond;
  {
    std::stringstream s;
    s << sm [2];
    s >> midiTempoPerSecond;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "midiTempoDuration  = " <<
      midiTempoDuration <<
      std::endl <<
      "midiTempoPerSecond = " <<
      midiTempoPerSecond <<
      std::endl;

  msrMidiTempo
    theMidiTempo (
      0, // inputLineNumber
      midiTempoDuration,
      midiTempoPerSecond);

  setMidiTempoVariable (
    theMidiTempo);
  }
#endif
}

void oahMidiTempoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMidiTempoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMidiTempoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahMidiTempoAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

std::string oahMidiTempoAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

void oahMidiTempoAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void oahMidiTempoAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fMidiTempoVariable.asString ();
  if (fSetByAnOption) {
    os <<
      ", set by an option" <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahOptionNameHelpAtom oahOptionNameHelpAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultOptionName)
{
  oahOptionNameHelpAtom* o = new
    oahOptionNameHelpAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName);
  assert (o != nullptr);
  return o;
}

oahOptionNameHelpAtom::oahOptionNameHelpAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable,
  const std::string& defaultOptionName)
  : oahStringWithDefaultValueAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName)
{
  this->setElementValueKind (
    oahElementValueKind::kElementValueOptional);

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahOptionNameHelpAtom::~oahOptionNameHelpAtom ()
{}

void oahOptionNameHelpAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionNameHelpAtom" <<
      std::endl;
  }
#endif

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahOptionNameHelpAtom::applyAtomWithDefaultValue (std::ostream& os)
{
  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      fDefaultStringValue);
}

void oahOptionNameHelpAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionNameHelpAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionNameHelpAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahOptionNameHelpAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' << fVariableName;

  return s.str ();
}

std::string oahOptionNameHelpAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' << fVariableName;

  return s.str ();
}

void oahOptionNameHelpAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionNameHelpAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahOptionNameHelpAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahOptionNameHelpAtom& elt)
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
S_oahQueryOptionNameAtom oahQueryOptionNameAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahQueryOptionNameAtom* o = new
    oahQueryOptionNameAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahQueryOptionNameAtom::oahQueryOptionNameAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpAtomExpectingAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  this->setElementValueKind (
    oahElementValueKind::kElementValueMandatory);

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahQueryOptionNameAtom::~oahQueryOptionNameAtom ()
{}

void oahQueryOptionNameAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahQueryOptionNameAtom" <<
      std::endl;
  }
#endif

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahQueryOptionNameAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahQueryOptionNameAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahQueryOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahQueryOptionNameAtom>*> (v)) {
        S_oahQueryOptionNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahQueryOptionNameAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahQueryOptionNameAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahQueryOptionNameAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahQueryOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahQueryOptionNameAtom>*> (v)) {
        S_oahQueryOptionNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahQueryOptionNameAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahQueryOptionNameAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahQueryOptionNameAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahQueryOptionNameAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

std::string oahQueryOptionNameAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahQueryOptionNameAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "QueryOptionNameAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahQueryOptionNameAtom::printAtomWithVariableOptionsValues (
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFindStringResult oahFindStringResult::create (
  const std::string&  theString,
  const S_oahElement& theElement)
{
  oahFindStringResult* o = new
    oahFindStringResult (
      theString,
      theElement);
  assert (o != nullptr);
  return o;
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
  std::stringstream s;

  s <<
    fTheString <<
    " - " <<
    fTheElement->asShortString ();

  return s.str ();
}

void oahFindStringResult::print (std::ostream& os) const
{
  os <<
    "FindStringAtom:" <<
    std::endl;

  ++gIndenter;

  gLogStream <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFindStringAtom oahFindStringAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahFindStringAtom* o = new
    oahFindStringAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahFindStringAtom::oahFindStringAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpAtomExpectingAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  this->setElementValueKind (
    oahElementValueKind::kElementValueMandatory);

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahFindStringAtom::~oahFindStringAtom ()
{}

void oahFindStringAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFindStringAtom" <<
      std::endl;
  }
#endif

  // a strings std::list to collect the results
  std::list<S_oahFindStringMatch> foundMatchesList;

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    findStringInFindableElement (
      mfStringToLowerCase (theString),
      foundMatchesList,
      os);

  //  print the found strings
  size_t foundMatchesListSize =
    foundMatchesList.size ();

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    mfSingularOrPlural (
      foundMatchesListSize, "occurrence", "occurrences") <<
    " of std::string \"" <<
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

    std::list<S_oahFindStringMatch>::const_iterator
      iBegin = foundMatchesList.begin (),
      iEnd   = foundMatchesList.end (),
      i      = iBegin;

    int counter = 0;

    for ( ; ; ) {
      oahFindStringMatch* theFindStringMatch = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        theFindStringMatch != nullptr,
        "theFindStringMatch is null");

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

void oahFindStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFindStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringAtom>*> (v)) {
        S_oahFindStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFindStringAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFindStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahFindStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringAtom>*> (v)) {
        S_oahFindStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFindStringAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFindStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahFindStringAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

std::string oahFindStringAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahFindStringAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FindStringAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahFindStringAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahFindStringAtom& elt)
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
S_oahIncludeOptionsAndArgumentsFileAtom oahIncludeOptionsAndArgumentsFileAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahIncludeOptionsAndArgumentsFileAtom* o = new
    oahIncludeOptionsAndArgumentsFileAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahIncludeOptionsAndArgumentsFileAtom::oahIncludeOptionsAndArgumentsFileAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahPureHelpAtomExpectingAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName)
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyNo; // superflous??? JMI

  this->setElementValueKind (
    oahElementValueKind::kElementValueMandatory);

  this->setMultipleOccurrencesAllowed ();
}

oahIncludeOptionsAndArgumentsFileAtom::~oahIncludeOptionsAndArgumentsFileAtom ()
{}

void oahIncludeOptionsAndArgumentsFileAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // nothing to do, this option has already be applied early JMIJMIJMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahIncludeOptionsAndArgumentsFileAtom" <<
      std::endl;
  }
#endif

  // delegate this to the handler JMIJMIJMI
//   fetchAtomUpLinkToHandler ()->
//     includeOptionsFileInHandler (
//       theString,
//       os);
}

void oahIncludeOptionsAndArgumentsFileAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*
    p =
      dynamic_cast<visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*> (v)) {
        S_oahIncludeOptionsAndArgumentsFileAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIncludeOptionsAndArgumentsFileAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIncludeOptionsAndArgumentsFileAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*
    p =
      dynamic_cast<visitor<S_oahIncludeOptionsAndArgumentsFileAtom>*> (v)) {
        S_oahIncludeOptionsAndArgumentsFileAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIncludeOptionsAndArgumentsFileAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIncludeOptionsAndArgumentsFileAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIncludeOptionsAndArgumentsFileAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahIncludeOptionsAndArgumentsFileAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

std::string oahIncludeOptionsAndArgumentsFileAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahIncludeOptionsAndArgumentsFileAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IncludeOptionsAndArgumentsFileAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahIncludeOptionsAndArgumentsFileAtom::printAtomWithVariableOptionsValues (
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
