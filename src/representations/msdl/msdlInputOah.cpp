/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...
#include <string>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "msdlInputOah.h"
#include "msdl2msrOah.h"

#include "oahEarlyOptions.h"

#include "msdlTokens.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msdlKeywordsLanguageAtom msdlKeywordsLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlKeywordsLanguageKind&
                    msdlKeywordsLanguageKindVariable)
{
  msdlKeywordsLanguageAtom* o = new
    msdlKeywordsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlKeywordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlKeywordsLanguageAtom::msdlKeywordsLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlKeywordsLanguageKind&
                    msdlKeywordsLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsdlKeywordsLanguageKindVariable (
      msdlKeywordsLanguageKindVariable)
{}

msdlKeywordsLanguageAtom::~msdlKeywordsLanguageAtom ()
{}

void msdlKeywordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlKeywordsLanguageKindVariable (
    (*it).second);
}

void msdlKeywordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlKeywordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlKeywordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlKeywordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

string msdlKeywordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

void msdlKeywordsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlKeywordsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdlKeywordsLanguageKindVariable" << " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlKeywordsLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlKeywordsLanguageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_oahDisplayMsdlKeywordsInLanguageAtom oahDisplayMsdlKeywordsInLanguageAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
{
  oahDisplayMsdlKeywordsInLanguageAtom* o = new
    oahDisplayMsdlKeywordsInLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahDisplayMsdlKeywordsInLanguageAtom::oahDisplayMsdlKeywordsInLanguageAtom (
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

oahDisplayMsdlKeywordsInLanguageAtom::~oahDisplayMsdlKeywordsInLanguageAtom ()
{}

void oahDisplayMsdlKeywordsInLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing keywords names
  gLogStream <<
    "The keywords in language '" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "' are: " <<
    endl;

  ++gIndenter;

  gLogStream <<
    gIndenter.indentMultiLineString (
      existingKeywordsInLanguage (
        keywordsLanguageKind,
        K_NAMES_LIST_MAX_LENGTH)) <<
    endl;

  --gIndenter;
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDisplayMsdlKeywordsInLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

string oahDisplayMsdlKeywordsInLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahDisplayMsdlKeywordsInLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlKeywordsInLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahDisplayMsdlKeywordsInLanguageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_oahDisplayMsdlTokensInLanguageAtom oahDisplayMsdlTokensInLanguageAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
{
  oahDisplayMsdlTokensInLanguageAtom* o = new
    oahDisplayMsdlTokensInLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (o != nullptr);
  return o;
}

oahDisplayMsdlTokensInLanguageAtom::oahDisplayMsdlTokensInLanguageAtom (
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

oahDisplayMsdlTokensInLanguageAtom::~oahDisplayMsdlTokensInLanguageAtom ()
{}

void oahDisplayMsdlTokensInLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing tokens
  gLogStream <<
    "The tokens in language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "\" are (<...> denotes a class of tokens): " <<
    endl;

  ++gIndenter;

  gLogStream <<
    gIndenter.indentMultiLineString (
      existingTokensInLanguage (
        keywordsLanguageKind,
        K_NAMES_LIST_MAX_LENGTH)) <<
    endl;

  --gIndenter;
}

void oahDisplayMsdlTokensInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahDisplayMsdlTokensInLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

string oahDisplayMsdlTokensInLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahDisplayMsdlTokensInLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlTokensInLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahDisplayMsdlTokensInLanguageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msdlCommentsTypeAtom msdlCommentsTypeAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlCommentsTypeKind&
                    msdlCommentsTypeKindVariable)
{
  msdlCommentsTypeAtom* o = new
    msdlCommentsTypeAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlCommentsTypeKindVariable);
  assert (o != nullptr);
  return o;
}

msdlCommentsTypeAtom::msdlCommentsTypeAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlCommentsTypeKind&
                    msdlCommentsTypeKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsdlCommentsTypeKindVariable (
      msdlCommentsTypeKindVariable)
{}

msdlCommentsTypeAtom::~msdlCommentsTypeAtom ()
{}

void msdlCommentsTypeAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      endl;
  }
#endif

  map<string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlCommentsTypeKindVariable (
    (*it).second);
}

void msdlCommentsTypeAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlCommentsTypeAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlCommentsTypeAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlCommentsTypeAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

string msdlCommentsTypeAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

void msdlCommentsTypeAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlCommentsTypeAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fMsdlCommentsTypeKindVariable" << " : \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlCommentsTypeAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlCommentsTypeAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msdlUserLanguageAtom msdlUserLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlUserLanguageKind&
                    msdlUserLanguageKindVariable)
{
  msdlUserLanguageAtom* o = new
    msdlUserLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlUserLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlUserLanguageAtom::msdlUserLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msdlUserLanguageKind&
                    msdlUserLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsdlUserLanguageVariable (
      msdlUserLanguageKindVariable)
{}

msdlUserLanguageAtom::~msdlUserLanguageAtom ()
{}

void msdlUserLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the User languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSDR User language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () <<
      " known MSDR user languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlUserLanguageKindVariable (
    (*it).second);
}

void msdlUserLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlUserLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlUserLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlUserLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

string msdlUserLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

void msdlUserLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlUserLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fMsdlUserLanguageVariable" << " : \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msdlUserLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlUserLanguageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msdlPitchesLanguageAtom msdlPitchesLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    msdlPitchesLanguageKindVariable)
{
  msdlPitchesLanguageAtom* o = new
    msdlPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

msdlPitchesLanguageAtom::msdlPitchesLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    msdlPitchesLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      msdlPitchesLanguageKindVariable)
{}

msdlPitchesLanguageAtom::~msdlPitchesLanguageAtom ()
{}

void msdlPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (
        theString);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSDR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msdlPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msdlPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msdlPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msdlPitchesLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlPitchesLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fMsrQuarterTonesPitchesLanguageKindVariable" << " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    endl;

  --gIndenter;
}

void msdlPitchesLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msdlPitchesLanguageAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//_______________________________________________________________________________
S_msdlInputOahGroup gGlobalMsdlInputOahGroup;

S_msdlInputOahGroup msdlInputOahGroup::create (
  const string& serviceName)
{
  msdlInputOahGroup* o = new msdlInputOahGroup (
    serviceName);
  assert (o != nullptr);
  return o;
}

msdlInputOahGroup::msdlInputOahGroup (
  const string& serviceName)
  : oahGroup (
    "MSDR",
    "help-msdl", "hmsdl",
R"(These options control the way MSDR data is handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdlInputOahGroup (
    serviceName);
}

msdlInputOahGroup::~msdlInputOahGroup ()
{}

void msdlInputOahGroup::initializeMsdlDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "help-msdl-display", "hmsdld",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display MSDR

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msdl", "dmsdl",
R"(Write the contents of the MSDR data to standard error.)",
        "displayMsdl",
        fDisplayMsdl));

  // display MSDR short

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msdl-short", "dmsdls",
R"(Write the contents of the MSDR data, short version, to standard error.)",
        "displayMsdlShort",
        fDisplayMsdlShort));
}

void msdlInputOahGroup::initializeMsdlLanguagesOptions (
  const string& serviceName)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "help-msdl-languages_JMI", "hmsdll_JMI",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // msdl pitches language

  if (! setMsdlQuarterTonesPitchesLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // MSDR pitches language

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdlQuarterTonesPitchesLanguageKind;

  fMsdlQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlPitchesLanguageAtom::create (
        "msdl-pitches-language_JMI", "msdlpl_JMI",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input note pitches.
The NUMBER MSDR pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            regex ("PITCHES_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlPitchesLanguage",
        fMsdlQuarterTonesPitchesLanguageKind));

  // MSDR keywords display

  const msdlKeywordsLanguageKind
    msdlKeywordsLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

  fMsdlKeywordsLanguageKind =
    msdlKeywordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahDisplayMsdlKeywordsInLanguageAtom::create (
        "display-keywords-in-language", "dkil",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Display the keywords in language LANGUAGE.
The NUMBER MSDR keywords languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlKeywordsLanguageKindDefaultValue)),
        "LANGUAGE",
        serviceName));

  // MSDR tokens display

  const msdlKeywordsLanguageKind
    msdlTokensLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

  fMsdlKeywordsLanguageKind =
    msdlTokensLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahDisplayMsdlTokensInLanguageAtom::create (
        "display-tokens-in-language", "dtil",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Display the tokens in language LANGUAGE.
The NUMBER MSDR tokens languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlTokensLanguageKindDefaultValue)),
        "LANGUAGE",
        serviceName));
}

void msdlInputOahGroup::initializeMsdlInputOahGroup (
  const string& serviceName)
{
  // display
  // --------------------------------------
  initializeMsdlDisplayOptions ();

  // languages
  // --------------------------------------
  initializeMsdlLanguagesOptions (
    serviceName);
}

//______________________________________________________________________________
Bool msdlInputOahGroup::setMsdlKeywordsLanguage (string language)
{
  // is language in the keywords languages map?
  map<string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (language);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlKeywordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
Bool msdlInputOahGroup::setMsdlQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (language);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msdlInputOahGroup::enforceGroupQuietness ()
{
  fDisplayMsdl = false;
  fDisplayMsdlShort = false;
}

//______________________________________________________________________________
void msdlInputOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdlInputOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlInputOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlInputOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlInputOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlInputOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdlInputOahGroup::printMsdlInputOahValues (int fieldWidth)
{
  gLogStream <<
    "The MSDR options are:" <<
    endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDisplayMsdl" << " : " <<
    fDisplayMsdl <<
    endl <<
    setw (fieldWidth) << "fDisplayMsdlShort" << " : " <<
    fDisplayMsdlShort <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "msdlQuarterTonesPitchesLanguage" << " : \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsdlQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) << "msdlKeywordsLanguage" << " : \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKind) <<
    "\"" <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdlInputOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msdlInputOahGroup createGlobalMsdlInputOahGroup (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global MSDR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdlInputOahGroup) {
    // create the global options group
    gGlobalMsdlInputOahGroup =
      msdlInputOahGroup::create (
        serviceName);
    assert (gGlobalMsdlInputOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdlInputOahGroup;
}


}
