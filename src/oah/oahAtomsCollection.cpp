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

#include "mfStringsHandling.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
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
  const string&    shortName,
  const string&    longName,
  const string&    description,
  S_oahAtom originalOahAtom)
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
  const string&    shortName,
  const string&    longName,
  const string&    description,
  S_oahAtom originalOahAtom)
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

void oahAtomAlias::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAtomAlias" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  if (fOriginalOahAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalOahAtom);
  }
}

void oahAtomAlias::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomAlias::printShort (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomAlias: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fOriginalOahAtom->fetchNames () <<
    endl;
}

void oahAtomAlias::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  fOriginalOahAtom->
    printAtomWithVariableOptionsValues (
      os,
      valueFieldWidth);
}

ostream& operator << (ostream& os, const S_oahAtomAlias& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahMacroAtom oahMacroAtom::create (
  const string& longName,
  const string& shortName,
  const string& description)
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
  const string& longName,
  const string& shortName,
  const string& description)
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

void oahMacroAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahMacroAtom" <<
      endl;
  }
#endif

  for (
    list<S_oahAtom>::const_iterator i =
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  for (
    list<S_oahAtom>::const_iterator i =
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

void oahMacroAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  for (
    list<S_oahAtom>::const_iterator i =
      fMacroAtomsList.begin ();
    i != fMacroAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    os <<
      "atom:" <<
      endl;
    ++gIndenter;
    os <<
      atom;
    --gIndenter;
    os << endl;
  } // for

  --gIndenter;
}

void oahMacroAtom::printShort (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MacroAtom: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  list<S_oahAtom>::const_iterator
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

void oahMacroAtom::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    ":" <<
    endl;

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
      endl;
  }

  else {
    os << endl;

    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fMacroAtomsList.begin (),
      iEnd   = fMacroAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom atom = (*i);

      os <<
        atom-> fetchNames () <<
        ":" <<
        endl;

      ++gIndenter;

			gIndenter.indentMultiLineString (
				atom-> getDescription (),
				os);

      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for

    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahMacroAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
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

ostream& operator << (ostream& os, const S_oahMacroAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahOptionsUsageAtom oahOptionsUsageAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahOptionsUsageAtom::~oahOptionsUsageAtom ()
{}

void oahOptionsUsageAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionsUsageAtom" <<
      endl;
  }
#endif

  printOptionsUsage (os);
}

void oahOptionsUsageAtom::printOptionsUsage (ostream& os) const
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    fHelpAtomWithoutAValueServiceName <<
    " options usage:" <<
    endl;

  ++gIndenter;

// 	gIndenter.indentMultiLineString (
// 		foundString,
// 		os);
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
				regex ("OPTION_NAME_HELP_NAMES"),
				gGlobalOahOahGroup->
					getOptionNameHelpAtom ()->
						fetchNamesBetweenQuotes ()
				),
			regex ("EXECUTABLE_NAME"),
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahOptionsUsageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUsageAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator << (ostream& os, const S_oahOptionsUsageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahHelpAtom oahHelpAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpAtom::~oahHelpAtom ()
{}

void oahHelpAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahHelpAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahHelpAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpAtom::printOptionsSummary (ostream& os) const
{
  os <<
    gGlobalOahOahGroup->getOahOahGroupServiceName () <<
    endl;
}

ostream& operator << (ostream& os, const S_oahHelpAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahHelpSummaryAtom oahHelpSummaryAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHelpSummaryAtom::~oahHelpSummaryAtom ()
{}

void oahHelpSummaryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpSummaryAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahHelpSummaryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsSummaryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpSummaryAtom::printOptionsSummary (ostream& os) const
{
  os <<
    gGlobalOahOahGroup->getOahOahGroupServiceName () <<
    endl;
}

ostream& operator << (ostream& os, const S_oahHelpSummaryAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahAboutAtom oahAboutAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahAboutAtom::~oahAboutAtom ()
{}

void oahAboutAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAboutAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahAboutAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAboutAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAboutAtom::printAbout (ostream& os) const
{
  os <<
    fetchAtomUpLinkToHandler ()->
      handlerServiceAboutInformation () <<
    endl;
}

ostream& operator << (ostream& os, const S_oahAboutAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
string oahVersionKindAsString (
  oahVersionKind versionKind)
{
  string result;

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

ostream& operator << (ostream& os, const oahVersionKind& elt)
{
  os << oahVersionKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_oahVersionAtom oahVersionAtom::create (
  const string&  longName,
  const string&  shortName,
  const string&  description,
  const string&  serviceName,
  oahVersionKind versionKind)
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
  const string&  longName,
  const string&  shortName,
  const string&  description,
  const string&  serviceName,
  oahVersionKind versionKind)
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

void oahVersionAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahVersionAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahVersionAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahVersionAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahVersionAtom::printVersionShort (ostream& os) const
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

void oahVersionAtom::printVersionFull (ostream& os) const
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

ostream& operator << (ostream& os, const S_oahVersionAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahLibraryVersionAtom oahLibraryVersionAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryVersionAtom::~oahLibraryVersionAtom ()
{}

void oahLibraryVersionAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahLibraryVersionAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahLibraryVersionAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahLibraryVersionAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahLibraryVersionAtom::printVersion (ostream& os) const
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

ostream& operator << (ostream& os, const S_oahLibraryVersionAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahHistoryAtom oahHistoryAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahHistoryAtom::~oahHistoryAtom ()
{}

void oahHistoryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahHistoryAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahHistoryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahHistoryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHistoryAtom::printHistory (ostream& os) const
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

ostream& operator << (ostream& os, const S_oahHistoryAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahLibraryHistoryAtom oahLibraryHistoryAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}

oahLibraryHistoryAtom::~oahLibraryHistoryAtom ()
{}

void oahLibraryHistoryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahLibraryHistoryAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahLibraryHistoryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahLibraryHistoryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahLibraryHistoryAtom::printHistory (ostream& os) const
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

ostream& operator << (ostream& os, const S_oahLibraryHistoryAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahContactAtom oahContactAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahContactAtom::~oahContactAtom ()
{}

void oahContactAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahContactAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahContactAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahContactAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahContactAtom::printContact (ostream& os) const
{
	gIndenter.indentMultiLineString (
    regex_replace (
R"(To contact the maintainers of EXECUTABLE_NAME:
  Create an issue at https://github.com/jacques-menu/musicformats,
  describing the problem and any error messages you get if relevant.
  You should sign up for GitHub for that.)",
        regex ("EXECUTABLE_NAME"),
        fHelpAtomWithoutAValueServiceName),
		os);
}

ostream& operator << (ostream& os, const S_oahContactAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahDisplayPrefixes oahDisplayPrefixes::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplayPrefixes::~oahDisplayPrefixes ()
{}

void oahDisplayPrefixes::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahDisplayPrefixes" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahDisplayPrefixes::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayPrefixes:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahDisplayPrefixes::printPrefixes (ostream& os) const
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

  handler->
    printKnownPrefixes (os);
}

ostream& operator << (ostream& os, const S_oahDisplayPrefixes& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahDisplaySingleCharacterOptions oahDisplaySingleCharacterOptions::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
  : oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName)
{}


oahDisplaySingleCharacterOptions::~oahDisplaySingleCharacterOptions ()
{}

void oahDisplaySingleCharacterOptions::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahDisplaySingleCharacterOptions" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahDisplaySingleCharacterOptions::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplaySingleCharacterOptions:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahDisplaySingleCharacterOptions::printSingleCharacterOptions (ostream& os) const
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomUpLinkToHandler ();

  handler->
    printKnownSingleCharacterOptions (os);
}

ostream& operator << (ostream& os, const S_oahDisplaySingleCharacterOptions& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahOnOffAtom oahOnOffAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  mfOnOffKind& onOffKindVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  mfOnOffKind& onOffKindVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOnOffAtom" <<
      endl;
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
    stringstream s;

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
        endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahOnOffAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OnOffAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOnOffKindVariable" << " : " <<
    fOnOffKindVariable <<
    endl <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<

    setw (fieldWidth) <<
    "fSetByAnOption" << " : " <<
    fSetByAnOption <<
    endl;

  --gIndenter;
}

void oahOnOffAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fOnOffKindVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahOnOffAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahBooleanAtom oahBooleanAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& variableName,
  Bool&         booleanVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& variableName,
  Bool&         booleanVariable)
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

void oahBooleanAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahBooleanAtom" <<
      endl;
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
        endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahBooleanAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "BooleanAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    fBooleanVariable <<
    endl <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<

    setw (fieldWidth) <<
    "fSetByAnOption" << " : " <<
    fSetByAnOption <<
    endl;

  --gIndenter;
}

void oahBooleanAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahBooleanAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  const string&    longName,
  const string&    shortName,
  const string&    description,
  const string&    variableName,
  Bool&            booleanVariable,
  S_oahBooleanAtom secondBooleanAtom)
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
  const string&    longName,
  const string&    shortName,
  const string&    description,
  const string&    variableName,
  Bool&            booleanVariable,
  S_oahBooleanAtom secondBooleanAtom)
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

void oahTwoBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahTwoBooleansAtom" <<
      endl;
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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahTwoBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "TwoBooleansAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    endl;

	++gIndenter;

	gIndenter.indentMultiLineString (
		fDescription,
		os);

	--gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "booleanVariable" << " : " <<
    fBooleanVariable <<
    endl <<
    setw (fieldWidth) <<
    "secondBooleanAtom" << " : " <<
    fSecondBooleanAtom <<
    endl;

  --gIndenter;
}

void oahTwoBooleansAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fBooleanVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahTwoBooleansAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  const string&    longName,
  const string&    shortName,
  const string&    description,
  const string&    variableName,
  Bool&            booleanVariable,
  S_oahBooleanAtom secondBooleanAtom,
  S_oahBooleanAtom thirdBooleanAtom)
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
  const string&    longName,
  const string&    shortName,
  const string&    description,
  const string&    variableName,
  Bool&            booleanVariable,
  S_oahBooleanAtom secondBooleanAtom,
  S_oahBooleanAtom thirdBooleanAtom)
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

void oahThreeBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahThreeBooleansAtom" <<
      endl;
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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahThreeBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "ThreeBooleansAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    endl;

  ++gIndenter;

	gIndenter.indentMultiLineString (
		fDescription,
		os);

	--gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "fBooleanVariable" << " : " <<
    fBooleanVariable <<
    endl <<
    setw (fieldWidth) <<
    "fSecondBooleanAtom" << " : " <<
    fSecondBooleanAtom <<
    endl <<
    setw (fieldWidth) <<
    "fThirdBooleanAtom" << " : " <<
    fThirdBooleanAtom <<
    endl;

  --gIndenter;
}

void oahThreeBooleansAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fBooleanVariable;

  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_oahThreeBooleansAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  const string& longName,
  const string& shortName,
  const string& description)
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
  const string& longName,
  const string& shortName,
  const string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::addBooleanAtom (
  S_oahBooleanAtom booleanAtom)
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
  const string& name)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' combined boolean variables to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  // set the value of the atoms in the list
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i =
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

void oahCombinedBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCombinedBooleansAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
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
  ostream& os,
  int fieldWidth) const
{
/* JMI
  printAtomWithVariableNameEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl;
    */
}

void oahCombinedBooleansAtom::printCombinedBooleansEssentialsShort (
  ostream& os,
  int fieldWidth) const
{
/* JMI
  printAtomWithVariableNameEssentialsShort (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl;
    */
}

void oahCombinedBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "CombinedBooleansAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    os << endl;

    ++gIndenter;

    os << "'";

    list<S_oahBooleanAtom>::const_iterator
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

  os << endl;
}

void oahCombinedBooleansAtom::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    ":" <<
    endl;

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
      endl;
  }

  else {
    os << endl;

    ++gIndenter;

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahBooleanAtom booleanAtom = (*i);

      os <<
        booleanAtom-> fetchNames () <<
        ":" <<
        endl;

      ++gIndenter;

			gIndenter.indentMultiLineString (
				booleanAtom-> getDescription (),
				os);

      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for

    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahCombinedBooleansAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  int fieldWidth =
    valueFieldWidth - gIndenter.getIndentation () + 1;

  ++gIndenter; // only now

  if (! fBooleanAtomsList.size ()) {
    os <<
      "[NONE]" << // JMI
      endl;
  }

  else {
    list<S_oahBooleanAtom>::const_iterator
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

  // JMI    os << endl;
    } // for
  }

  --gIndenter;

}

ostream& operator << (ostream& os, const S_oahCombinedBooleansAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahCommonPrefixBooleansAtom oahCommonPrefixBooleansAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& shortSuffixDescriptor,
  const string& longSuffixDescriptor,
  S_oahPrefix   shortNamesPrefix,
  S_oahPrefix   longNamesPrefix)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& shortSuffixDescriptor,
  const string& longSuffixDescriptor,
  S_oahPrefix   shortNamesPrefix,
  S_oahPrefix   longNamesPrefix)
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
  S_oahBooleanAtom booleanAtom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    booleanAtom != nullptr,
    "booleanAtom is null");

  // long name consistency check
  {
    string booleanAtomLongName =
      booleanAtom->getLongName ();

    if (booleanAtomLongName.size ()) {
      std::size_t found =
        booleanAtomLongName.find (fLongNamesPrefixName);

      if (found == string::npos) {
        stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" is different than the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          endl;

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else if (found != 0) {
        stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" doesn't start by the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"" <<
          endl;

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else {
        string booleanAtomLongNameSuffix =
          booleanAtomLongName.substr (
            fLongNamesPrefixName.size ());

        if (booleanAtomLongNameSuffix.size () == 0) {
          stringstream s;

          s <<
            "Option long name \"" <<
            booleanAtomLongName <<
            "\" is nothing more than the long names prefix name \"" <<
            fLongNamesPrefixName <<
            "\"" <<
          endl;

          booleanAtom->print (s);

          oahError (s.str ());
        }
        else {
          // register this boolean atom's suffix in the list
          fLongNamesSuffixes.push_back (booleanAtomLongNameSuffix);
        }
      }
    }

    else {
      stringstream s;

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
    string booleanAtomShortName =
      booleanAtom->getShortName ();

    std::size_t found =
      booleanAtomShortName.find (fShortNamesPrefixName);

    if (found == string::npos) {
      stringstream s;

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
      stringstream s;

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
      string booleanAtomShortNameSuffix =
        booleanAtomShortName.substr (
          fShortNamesPrefixName.size ());

      if (booleanAtomShortNameSuffix.size () == 0) {
        stringstream s;

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
  const string& name)
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

void oahCommonPrefixBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCommonPrefixBooleansAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
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
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
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

void oahCommonPrefixBooleansAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiplexBooleansAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    "shortSuffixDescriptor" << " : " <<
    fShortSuffixDescriptor <<
    endl <<
    "longSuffixDescriptor" << " : " <<
    fLongSuffixDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "shortNamesPrefix" << " : ";
  if (fShortNamesPrefix) {
    os << fShortNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "longNamesPrefix" << " : ";
  if (fLongNamesPrefix) {
    os << fLongNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os << "[EMPTY]";
  }
  else {
    os << endl;

    ++gIndenter;

    list<S_oahBooleanAtom>::const_iterator
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
  os << endl;

  --gIndenter;
}

void oahCommonPrefixBooleansAtom::printHelp (ostream& os) const
{
  os <<
    '-' << fShortNamesPrefixName << "<" << fShortSuffixDescriptor << ">" <<
    ", " <<
    '-' << fLongNamesPrefixName << "-<" << fLongSuffixDescriptor << ">" <<
    endl;

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
    os << endl;
    ++gIndenter;

    list<string>::const_iterator
      iBegin = fShortNamesSuffixes.begin (),
      iEnd   = fShortNamesSuffixes.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
        os << endl;
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

    os << "." << endl;
    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      endl;
  }

  if (fLongSuffixDescriptor != fShortSuffixDescriptor) {
    int longNamesSuffixesCount = 0;

    {
      list<string>::const_iterator
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
      os << endl;
      ++gIndenter;

      list<string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      int cumulatedLength = 0;

      for ( ; ; ) {
        string suffix = (*i);

        cumulatedLength += suffix.size ();
        if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
          os << endl;
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

      os << "." << endl;
      --gIndenter;
    }
    else {
      os <<
        "[NONE]" <<
        endl;
    }
  }

  if (fDescription.size ()) { // ??? JMI
// JMI    g_Indenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahCommonPrefixBooleansAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the boolean atoms in the list
}

ostream& operator << (ostream& os, const S_oahCommonPrefixBooleansAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahIntegerAtom oahIntegerAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  int&          integerVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  int&          integerVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the integer value

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << theString;
      s >> integerValue;
    }

    setIntegerVariable (integerValue);
  }

  else {
    stringstream s;

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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fIntegerVariable;

  return s.str ();
}

string oahIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fIntegerVariable;

  return s.str ();
}

void oahIntegerAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  --gIndenter;
}

void oahIntegerAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fIntegerVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahIntegerAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahTwoIntegersAtom oahTwoIntegersAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  int&          integerVariable,
  int&          integerSecondaryVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  int&          integerVariable,
  int&          integerSecondaryVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the two integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // integer secondary value
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 1 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;

    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerSecondaryVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    stringstream s;

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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

string oahTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName <<
    " \"" <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

void oahTwoIntegersAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl <<
    setw (fieldWidth) <<
    "integerSecondaryVariable" << " : " <<
    fIntegerSecondaryVariable <<
    endl;

  --gIndenter;
}

void oahTwoIntegersAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fIntegerVariable <<
    ' ' <<
    fIntegerSecondaryVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahTwoIntegersAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFloatAtom oahFloatAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  float&        floatVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  float&        floatVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the float value

  // check whether it is well-formed
  string regularExpression (
    "([+|-]?[[:digit:]]+)(.[[:digit:]]*)?"
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for float string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;

      s << sm [ 0 ];
      s >> floatValue;
    }

    setFloatVariable (floatValue);
  }

  else {
    stringstream s;

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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahFloatAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fFloatVariable;

  return s.str ();
}

string oahFloatAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fFloatVariable;

  return s.str ();
}

void oahFloatAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FloatAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "floatVariable" << " : " <<
    fFloatVariable <<
    endl;

  --gIndenter;
}

void oahFloatAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fFloatVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahFloatAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringAtom oahStringAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable)
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
  const string& theString,
  ostream&      os)
{
  setStringVariable (theString);
}

void oahStringAtom::setStringVariable (const string& value)
  {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << " '" << fStringVariable << "'";

  return s.str ();
}

string oahStringAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << " '" << fStringVariable << "'";

  return s.str ();
}

void oahStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl;

  --gIndenter;
}

void oahStringAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    '[' << fStringVariable << ']';

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahStringAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFactorizedStringAtom oahFactorizedStringAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& atomNameDescriptor,
  const string& stringValueDescriptor)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& atomNameDescriptor,
  const string& stringValueDescriptor)
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
  S_oahStringAtom stringAtom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stringAtom != nullptr,
    "stringAtom is null");

  // atom long name consistency check
  string stringAtomLongName =
    stringAtom->getLongName ();

  if (stringAtomLongName.size () == 0) {
    stringstream s;

    s <<
      "option long name \"" << stringAtomLongName <<
      "\" is empty";

    stringAtom->print (s);

    oahError (s.str ());
  }

  // atom short name consistency check
  string stringAtomShortName =
    stringAtom->getShortName ();

  if (stringAtomShortName.size () == 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyOahVerboseMode ()) {
      stringstream s;

      s <<
        "Option short name \"" << stringAtomShortName << "\"" <<
        " for long name \"" << stringAtomLongName << "\"" <<
        " is empty";

      oahWarning (s.str ());
    }
#endif
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
  const string& name)
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

void oahFactorizedStringAtom::applyElement (ostream& os) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying monoplex string atom '" <<
      fetchNames () <<
      "' which is a oahFactorizedStringAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  // browse the string atoms
  if (fStringAtomsList.size ()) {
    for (
      list<S_oahStringAtom>::const_iterator i = fStringAtomsList.begin ();
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

void oahFactorizedStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FactorizedStringAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    "atomNameDescriptor" << " : " <<
    fAtomNameDescriptor <<
    endl <<
    "stringValueDescriptor" << " : " <<
    fStringValueDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fStringAtomsList" << " : ";

  if (! fStringAtomsList.size ()) {
    os << "[EMPTY]";
  }

  else {
    os << endl;

    ++gIndenter;

    os << "'";

    list<S_oahStringAtom>::const_iterator
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

  os << endl;
}

void oahFactorizedStringAtom::printHelp (ostream& os) const
{
  os <<
    '-' << fAtomNameDescriptor << ' ' << fStringValueDescriptor <<
    endl;

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
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    list<string>::const_iterator
      iBegin = fAtomNamesList.begin (),
      iEnd   = fAtomNamesList.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
        os << endl;
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

    os << "." << endl;
    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahFactorizedStringAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the string atoms in the list
}

ostream& operator << (ostream& os, const S_oahFactorizedStringAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringWithDefaultValueAtom oahStringWithDefaultValueAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& defaultStringValue)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& defaultStringValue)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahStringWithDefaultValueAtom" <<
      endl;
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
      endl;
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
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithDefaultValueAtom::setStringVariable (const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringWithDefaultValueAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fStringVariable;

  return s.str ();
}

string oahStringWithDefaultValueAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fStringVariable;

  return s.str ();
}

void oahStringWithDefaultValueAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fDefaultStringValue" << " : " <<
    "\"" << fDefaultStringValue << "\"" <<
    endl;

  --gIndenter;
}

void oahStringWithDefaultValueAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fStringVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahStringWithDefaultValueAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringWithRegexAtom oahStringWithRegexAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& regexString)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& regexString)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahStringWithRegexAtom" <<
      endl;
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
      endl;
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
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithRegexAtom::setStringVariable (const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringWithRegexAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fStringVariable;

  return s.str ();
}

string oahStringWithRegexAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fStringVariable;

  return s.str ();
}

void oahStringWithRegexAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fRegexString" << " : " <<
    "\"" << fRegexString << "\"" <<
    endl;

  --gIndenter;
}

void oahStringWithRegexAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fStringVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahStringWithRegexAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    endl;
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahRationalAtom oahRationalAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  Rational&     rationalVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  Rational&     rationalVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the fraction:
  // decipher it to extract numerator and denominator values

  string regularExpression (
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // numerator
    "/"
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // denominator
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Rational string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) { // JMI ???
    int
      numerator,
      denominator;

    {
      stringstream s;
      s << sm [1];
      s >> numerator;
    }
    {
      stringstream s;
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
        endl;
    }
#endif

    setRationalVariable (rationalValue);
  }

  else {
    stringstream s;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahRationalAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fRationalVariable;

  return s.str ();
}

string oahRationalAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fRationalVariable;

  return s.str ();
}

void oahRationalAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RationalAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "rationalVariable" << " : " <<
    fRationalVariable <<
    endl;

  --gIndenter;
}

void oahRationalAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRationalVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahRationalAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahNaturalNumbersSetElementAtom oahNaturalNumbersSetElementAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<int>&     naturalNumbersSetVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<int>&     naturalNumbersSetVariable)
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
  const string& theString,
  ostream&      os)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahNaturalNumbersSetElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    '[';

  set<int>::const_iterator
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

string oahNaturalNumbersSetElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    '[';

  set<int>::const_iterator
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

void oahNaturalNumbersSetElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "NaturalNumbersSetElementAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fNaturalNumbersSetVariable" << " : " <<
    endl;

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
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

  os << endl;

  --gIndenter;
}

void oahNaturalNumbersSetElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "[EMPTY]";
  }

  else {
    if (fSetByAnOption) {
      os <<
        "set by an option" <<
        endl;
    }

    set<int>::const_iterator
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

ostream& operator << (ostream& os, const S_oahNaturalNumbersSetElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahRGBColorAtom oahRGBColorAtom::create (
  const string&       shortName,
  const string&       longName,
  const string&       description,
  const string&       valueSpecification,
  const string&       variableName,
  msrRGBColor& RGBColorVariable)
{
  oahRGBColorAtom* o = new
    oahRGBColorAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      RGBColorVariable);
  assert (o != nullptr);
  return o;
}

oahRGBColorAtom::oahRGBColorAtom (
  const string&       shortName,
  const string&       longName,
  const string&       description,
  const string&       valueSpecification,
  const string&       variableName,
  msrRGBColor& RGBColorVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fRGBColorVariable (
      RGBColorVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahRGBColorAtom::~oahRGBColorAtom ()
{}

void oahRGBColorAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  msrRGBColor theRGBColor (theString);

  setRGBColorVariable (theRGBColor);
}

void oahRGBColorAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRGBColorAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRGBColorAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahRGBColorAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    '[' <<
    fRGBColorVariable.asString () <<
    ']';

  return s.str ();
}

string oahRGBColorAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    '[' <<
    fRGBColorVariable.asString () <<
    ']';

  return s.str ();
}

void oahRGBColorAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RGBColorAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "RGBColorVariable" << " : " <<
    fRGBColorVariable.asString () <<
    endl;

  --gIndenter;
}

void oahRGBColorAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRGBColorVariable.asString ();
  if (fSetByAnOption) {
    os <<
      ", set by an option" <<
      endl;
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_oahRGBColorAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahIntSetElementAtom oahIntSetElementAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<int>&     intSetVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<int>&     intSetVariable)
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
  const string& theString,
  ostream&      os)
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
      endl;
  }
#endif

  string regularExpression (
    "([[:digit:]]+)"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 2) {
    stringstream s;

    s <<
      "-boxAroundBarNumber argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [1];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahIntSetElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fIntSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    set<int>::const_iterator
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

string oahIntSetElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fIntSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    set<int>::const_iterator
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

void oahIntSetElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntSetElementAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "intSetVariable" << " : '" <<
    endl;

  if (! fIntSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  --gIndenter;
}

void oahIntSetElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fIntSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "set by an option" <<
      endl;

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_oahIntSetElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringSetElementAtom oahStringSetElementAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringSetElementAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be ignored

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringSetElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    set<string>::const_iterator
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

string oahStringSetElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringSetVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    set<string>::const_iterator
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

void oahStringSetElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringSetElementAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void oahStringSetElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os <<
      "set by an option" <<
      endl;

    ++gIndenter;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_oahStringSetElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringToIntMapElementAtom oahStringToIntMapElementAtom::create (
  const string&     shortName,
  const string&     longName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  map<string, int>& stringToIntMapVariable)
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
  const string&     shortName,
  const string&     longName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  map<string, int>& stringToIntMapVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'" <<
      endl;
  }
#endif

  // theString contains the string int map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapElementAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"     // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:digit:]]+)" // int
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string musicxmlMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "musicxmlMeasureNumber = " <<
      musicxmlMeasureNumber <<
      endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringToIntMapElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, int>::const_iterator
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

string oahStringToIntMapElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToIntMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, int>::const_iterator
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

void oahStringToIntMapElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToIntMapAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringToIntMapVariable" << " : " <<
    endl;

  if (! fStringToIntMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  --gIndenter;
}

void oahStringToIntMapElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToIntMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, int>::const_iterator
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
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "set by an option" <<
      endl;

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_oahStringToIntMapElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringToStringMapElementAtom oahStringToStringMapElementAtom::create (
    const string&         longName,
    const string&         shortName,
    const string&         description,
    const string&         valueSpecification,
    const string&         variableName,
    map<string, string>&  stringToStringMapVariable)
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
    const string&         longName,
    const string&         shortName,
    const string&         description,
    const string&         valueSpecification,
    const string&         variableName,
    map<string, string>&  stringToStringMapVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'" <<
      endl;
  }
#endif

  // theString contains the string string map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMapElementAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    key   = sm [1],
    value = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "key = " <<
      key <<
      endl <<
      "value = " <<
      value <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringToStringMapElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, string>::const_iterator
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

string oahStringToStringMapElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, string>::const_iterator
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

void oahStringToStringMapElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToStringMapAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringToStringMapVariable" << " : ";

  if (! fStringToStringMapVariable.size ()) {
    os << "[EMPTY]" << endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
//       os << endl; // JMI
    } // for

    --gIndenter;
  }

  os << endl;
  --gIndenter;
}

void oahStringToStringMapElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToStringMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, string>::const_iterator
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
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "set by an option" <<
      endl;

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_oahStringToStringMapElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringToStringMultiMapElementAtom oahStringToStringMultiMapElementAtom::create (
    const string&         longName,
    const string&         shortName,
    const string&         description,
    const string&         valueSpecification,
    const string&         variableName,
    multimap<string, string>&  stringToStringMultiMapVariable)
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
    const string&         longName,
    const string&         shortName,
    const string&         description,
    const string&         valueSpecification,
    const string&         variableName,
    multimap<string, string>&  stringToStringMultiMapVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'" <<
      endl;
  }
#endif

  // theString contains the string string multimap specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToStringMultiMapElementAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    ":"
    "[[:space:]]*"
    "([[:w:]]+)"      // string
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for " <<
      fetchNamesBetweenQuotes () <<
      " string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      fetchNamesBetweenQuotes () <<
      " argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    key   = sm [1],
    value = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "key = " <<
      key <<
      endl <<
      "value = " <<
      value <<
      endl;
  }
#endif

  fStringToStringMultiMapVariable.insert (
    pair<string, string> (key, value));
  fSetByAnOption = true;
}

void oahStringToStringMultiMapElementAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToStringMultiMapElementAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringToStringMultiMapElementAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    multimap<string, string>::const_iterator
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

string oahStringToStringMultiMapElementAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToStringMultiMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    multimap<string, string>::const_iterator
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

void oahStringToStringMultiMapElementAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringToStringMultiMapAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fStringToStringMultiMapVariable" << " : ";

  if (! fStringToStringMultiMapVariable.size ()) {
    os << "[EMPTY]" << endl;
  }
  else {
    os << endl;
    ++gIndenter;

    multimap<string, string>::const_iterator
      iBegin = fStringToStringMultiMapVariable.begin (),
      iEnd   = fStringToStringMultiMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
//       os << endl; // JMI
    } // for

    --gIndenter;
  }

  os << endl;
  --gIndenter;
}

void oahStringToStringMultiMapElementAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToStringMultiMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    multimap<string, string>::const_iterator
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
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      "set by an option" <<
      endl;

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_oahStringToStringMultiMapElementAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringAndIntegerAtom oahStringAndIntegerAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& stringVariableName,
  string&       stringVariable,
  const string& integerVariableName,
  int&          integerVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& stringVariableName,
  string&       stringVariable,
  const string& integerVariableName,
  int&          integerVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "(.+)"           // string
    "[[:space:]]+"
    ":"
    "[[:space:]]+"
    "([[:digit:]]+)" // int
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }

    fIntegerVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    stringstream s;

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
      endl;
  }
#endif

  fIntegerVariable = value;
  fSetByAnOption = true;
}

void oahStringAndIntegerAtom::setStringVariable (const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
      value <<
      "\"" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringAndIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

string oahStringAndIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

void oahStringAndIntegerAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariableName" << " : " <<
    "\"" << fStringVariableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "integerVariableName" << " : " <<
    fIntegerVariableName <<
    endl <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  --gIndenter;
}

void oahStringAndIntegerAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fStringVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (valueFieldWidth) <<
    fIntegerVariableName <<
    " : " <<
    "\"" << fIntegerVariable << "\"";

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahStringAndIntegerAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahStringAndTwoIntegersAtom oahStringAndTwoIntegersAtom::create ( // UNUSED JMI
  const string&  shortName,
  const string&  longName,
  const string&  description,
  const string&  valueSpecification,
  const string&  variableName,
  string&        stringVariable,
  int&           primaryIntegerVariable,
  int&           secondaryIntegerVariable)
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
  const string&  shortName,
  const string&  longName,
  const string&  description,
  const string&  valueSpecification,
  const string&  variableName,
  string&        stringVariable,
  int&           primaryIntegerVariable,
  int&           secondaryIntegerVariable)
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
  const string& theString,
  ostream&      os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:alpha:]]+)" // string
    "[[:space:]]+"
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // secondary integer value
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 4) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fPrimaryIntegerVariable = integerValue; // JMI

    {
      stringstream s;
      s << sm [ 3 ];
      s >> integerValue;
    }
    fSecondaryIntegerVariable = integerValue;

    fSetByAnOption = true;
  }

  else {
    stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndTwoIntegersAtom::setStringVariable (const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to '" <<
      value <<
      "'" <<
      endl;
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
      endl;
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
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahStringAndTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

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

string oahStringAndTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

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

void oahStringAndTwoIntegersAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    "primaryIntegerVariable" << " : " <<
    fPrimaryIntegerVariable <<
    "secondaryIntegerVariable" << " : " <<
    fSecondaryIntegerVariable <<
    endl;

  --gIndenter;
}

void oahStringAndTwoIntegersAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "primaryIntegerVariable" <<
    " : " <<
    fPrimaryIntegerVariable <<
    endl <<
    setw (valueFieldWidth) <<
    "secondaryIntegerVariable" <<
    " : " <<
    fSecondaryIntegerVariable;

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahStringAndTwoIntegersAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahLengthUnitKindAtom oahLengthUnitKindAtom::create (
  const string&      shortName,
  const string&      longName,
  const string&      description,
  const string&      valueSpecification,
  const string&      variableName,
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
  const string&      shortName,
  const string&      longName,
  const string&      description,
  const string&      valueSpecification,
  const string&      variableName,
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  map<string, msrLengthUnitKind>::const_iterator
    it =
      gGlobalMsrLengthUnitKindsMap.find (
        theString);

  if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsrLengthUnitKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahLengthUnitKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

string oahLengthUnitKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

void oahLengthUnitKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthUnitKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "optionsLengthUnitKindVariable" << " : " <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    endl;

  --gIndenter;
}

void oahLengthUnitKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable);

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahLengthUnitKindAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahLengthAtom oahLengthAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  msrLength&    lengthVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  msrLength&    lengthVariable)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:

  // check whether it is well-formed
   // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
  string regularExpression (
    "([[:digit:]]+)"     // integer part
    "(.[[:digit:]]*)"    // decimal part
    "?([[:alpha:]]{2,})" // length unit
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 4) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;
      // concatenate the integer and decimal parts
      s << sm [ 1 ] << sm [ 2 ];
      s >> floatValue;
    }

    string lengthUnitName = sm [ 3 ];

    // is lengthUnitName known in the length unit names map?
    map<string, msrLengthUnitKind>::const_iterator
      it =
        gGlobalMsrLengthUnitKindsMap.find (
          lengthUnitName);

    if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
      // no, length unit name is unknown in the map

      stringstream s;

      s <<
        "length unit name \"" << lengthUnitName <<
        "\" is unknown" <<
        endl <<
        "The " <<
        gGlobalMsrLengthUnitKindsMap.size () <<
        " known length unit names are:" <<
        endl;

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
    stringstream s;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahLengthAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fLengthVariable.asString ();

  return s.str ();
}

string oahLengthAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fLengthVariable.asString ();

  return s.str ();
}

void oahLengthAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "optionsLengthVariable" << " : " <<
    fLengthVariable.asString () <<
    endl;

  --gIndenter;
}

void oahLengthAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fLengthVariable.asString ();

  if (fSetByAnOption) { // JMIJMIJMI
    os <<
      ", set by an option";
  }

  os << endl;
}

ostream& operator << (ostream& os, const S_oahLengthAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahMidiTempoAtom oahMidiTempoAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  msrMidiTempo& midiTempoVariable)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  msrMidiTempo& midiTempoVariable)
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
      endl;
  }
#endif

  fMidiTempoVariable = value;
  fSetByAnOption = true;
}

void oahMidiTempoAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // midiTempoDuration
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // midiTempoPerSecond
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-midiTempo argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string midiTempoDuration = sm [1];

  int midiTempoPerSecond;
  {
    stringstream s;
    s << sm [2];
    s >> midiTempoPerSecond;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "midiTempoDuration  = " <<
      midiTempoDuration <<
      endl <<
      "midiTempoPerSecond = " <<
      midiTempoPerSecond <<
      endl;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahMidiTempoAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

string oahMidiTempoAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

void oahMidiTempoAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MidiTempoAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "midiTempoVariable" << " : \"" <<
    fMidiTempoVariable.asString () <<
    "\"" <<
    endl;

  --gIndenter;
}

void oahMidiTempoAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fMidiTempoVariable.asString ();
  if (fSetByAnOption) {
    os <<
      ", set by an option" <<
      endl;
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_oahMidiTempoAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahOptionNameHelpAtom oahOptionNameHelpAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& defaultOptionName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  string&       stringVariable,
  const string& defaultOptionName)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionNameHelpAtom" <<
      endl;
  }
#endif

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahOptionNameHelpAtom::applyAtomWithDefaultValue (ostream& os)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahOptionNameHelpAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' << fVariableName;

  return s.str ();
}

string oahOptionNameHelpAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' << fVariableName;

  return s.str ();
}

void oahOptionNameHelpAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionNameHelpAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahOptionNameHelpAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator << (ostream& os, const S_oahOptionNameHelpAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahQueryOptionNameAtom oahQueryOptionNameAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahQueryOptionNameAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahQueryOptionNameAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

string oahQueryOptionNameAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahQueryOptionNameAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "QueryOptionNameAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahQueryOptionNameAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator << (ostream& os, const S_oahQueryOptionNameAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFindStringResult oahFindStringResult::create (
  const string& theString,
  S_oahElement  theElement)
{
  oahFindStringResult* o = new
    oahFindStringResult (
      theString,
      theElement);
  assert (o != nullptr);
  return o;
}

oahFindStringResult::oahFindStringResult (
  const string& theString,
  S_oahElement  theElement)
{
  fTheString  = theString;
  fTheElement = theElement;
}

oahFindStringResult::~oahFindStringResult ()
{}

string oahFindStringResult::asString () const
{
  stringstream s;

  s <<
    fTheString <<
    " - " <<
    fTheElement->asShortString ();

  return s.str ();
}

void oahFindStringResult::print (ostream& os) const
{
  os <<
    "FindStringAtom:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    fTheString <<
    endl <<
    fTheElement->asShortString () <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const oahFindStringResult& elt)
{
  elt.print (os);
  return os;
}

ostream& operator << (ostream& os, const S_oahFindStringResult& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahFindStringAtom oahFindStringAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFindStringAtom" <<
      endl;
  }
#endif

  // a strings list to collect the results
  list<S_oahFindStringMatch> foundMatchesList;

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
    " of string \"" <<
    theString <<
    "\" " <<
    mfSingularOrPluralWithoutNumber (
      foundMatchesListSize, "has", "have") <<
    " been found";

  if (foundMatchesListSize) {
    os <<
      ":" <<
      endl;

    ++gIndenter;

    list<S_oahFindStringMatch>::const_iterator
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

      string
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

      os << right <<
        setw (2) << counter << ": " << containingFindableElementInfo <<
        endl;

      ++gIndenter;

      os << elementName << endl;

      // indent a bit more for readability
      gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

			gIndenter.indentMultiLineString (
				foundString,
				os);

      // unindent a bit more for readability
      gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
      --gIndenter;

      if (++i == iEnd) break;

      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << endl;
  }

  gIndenter.setIndentation (saveIndent);
}

void oahFindStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahFindStringAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

string oahFindStringAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahFindStringAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FindStringAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahFindStringAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator << (ostream& os, const S_oahFindStringAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahIncludeOptionsAndArgumentsFileAtom oahIncludeOptionsAndArgumentsFileAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& theString,
  ostream&      os)
{
  // nothing to do, this option has already be applied early JMIJMIJMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahIncludeOptionsAndArgumentsFileAtom" <<
      endl;
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahIncludeOptionsAndArgumentsFileAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

string oahIncludeOptionsAndArgumentsFileAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahIncludeOptionsAndArgumentsFileAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IncludeOptionsAndArgumentsFileAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahIncludeOptionsAndArgumentsFileAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator << (ostream& os, const S_oahIncludeOptionsAndArgumentsFileAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
