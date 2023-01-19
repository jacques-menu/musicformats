/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...
#include <string>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "msdlInputOah.h"
#include "msdl2msrOah.h"

#include "oahEarlyOptions.h"

#include "msdlTokens.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdlKeywordsLanguageAtom msdlKeywordsLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      std::endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'" <<
      std::endl;
  }
#endif

  std::map<std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlKeywordsLanguageKindVariable (
    (*it).second);
}

void msdlKeywordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlKeywordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlKeywordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlKeywordsLanguageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string msdlKeywordsLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

std::string msdlKeywordsLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return s.str ();
}

void msdlKeywordsLanguageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlKeywordsLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fMsdlKeywordsLanguageKindVariable" << ": \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void msdlKeywordsLanguageAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKindVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlKeywordsLanguageAtom& elt)
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
S_oahDisplayMsdlKeywordsInLanguageAtom oahDisplayMsdlKeywordsInLanguageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
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

oahDisplayMsdlKeywordsInLanguageAtom::~oahDisplayMsdlKeywordsInLanguageAtom ()
{}

void oahDisplayMsdlKeywordsInLanguageAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  std::map<std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

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
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    existingKeywordsInLanguage (
      keywordsLanguageKind,
      K_MF_NAMES_LIST_MAX_LENGTH),
    gLogStream);

  --gIndenter;
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahDisplayMsdlKeywordsInLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

std::string oahDisplayMsdlKeywordsInLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahDisplayMsdlKeywordsInLanguageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlKeywordsInLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahDisplayMsdlKeywordsInLanguageAtom& elt)
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
S_oahDisplayMsdlTokensInLanguageAtom oahDisplayMsdlTokensInLanguageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
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

oahDisplayMsdlTokensInLanguageAtom::~oahDisplayMsdlTokensInLanguageAtom ()
{}

void oahDisplayMsdlTokensInLanguageAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the language name:
  // is it in the keywords languages map?

  std::map<std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream s;

    s <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

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
    "\" are (<...> denotes a class   of tokens): " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    existingTokensInLanguage (
      keywordsLanguageKind,
      K_MF_NAMES_LIST_MAX_LENGTH),
    gLogStream);

  --gIndenter;
}

void oahDisplayMsdlTokensInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahDisplayMsdlTokensInLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName;

  return s.str ();
}

std::string oahDisplayMsdlTokensInLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName;

  return s.str ();
}

void oahDisplayMsdlTokensInLanguageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlTokensInLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahDisplayMsdlTokensInLanguageAtom& elt)
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
S_msdlCommentsTypeAtom msdlCommentsTypeAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      std::endl;
  }
#endif

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'" <<
      std::endl;
  }
#endif

  std::map<std::string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream s;

    s <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlCommentsTypeKindVariable (
    (*it).second);
}

void msdlCommentsTypeAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlCommentsTypeAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlCommentsTypeAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlCommentsTypeAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string msdlCommentsTypeAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

std::string msdlCommentsTypeAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return s.str ();
}

void msdlCommentsTypeAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlCommentsTypeAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fMsdlCommentsTypeKindVariable" << ": \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void msdlCommentsTypeAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    msdlCommentsTypeKindAsString (
      fMsdlCommentsTypeKindVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlCommentsTypeAtom& elt)
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
S_msdlUserLanguageAtom msdlUserLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      std::endl;
  }
#endif

  // theString contains the language name:
  // is it in the User languages map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'" <<
      std::endl;
  }
#endif

  std::map<std::string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    std::stringstream s;

    s <<
      "MSDR User language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () <<
      " known MSDR user languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsdlUserLanguageKindVariable (
    (*it).second);
}

void msdlUserLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlUserLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlUserLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlUserLanguageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string msdlUserLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

std::string msdlUserLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return s.str ();
}

void msdlUserLanguageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlUserLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMsdlUserLanguageVariable" << ": \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void msdlUserLanguageAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlUserLanguageAtom& elt)
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
S_msdlPitchesLanguageAtom msdlPitchesLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      std::endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'" <<
      std::endl;
  }
#endif

  std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (
        theString);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map

    std::stringstream s;

    s <<
      "MSDR pitches language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msdlPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlPitchesLanguageAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string msdlPitchesLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

std::string msdlPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msdlPitchesLanguageAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdlPitchesLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMsrQuarterTonesPitchesLanguageKindVariable" << ": " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    std::endl;

  --gIndenter;
}

void msdlPitchesLanguageAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlPitchesLanguageAtom& elt)
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
S_msdlInputOahGroup gGlobalMsdlInputOahGroup;

S_msdlInputOahGroup msdlInputOahGroup::create (
  const std::string& serviceName)
{
  msdlInputOahGroup* o = new msdlInputOahGroup (
    serviceName);
  assert (o != nullptr);
  return o;
}

msdlInputOahGroup::msdlInputOahGroup (
  const std::string& serviceName)
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
        "fDisplayMsdl",
        fDisplayMsdl));

  // display MSDR short

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msdl-full", "dmsdlfull",
R"(Write the contents of the MSDR data, full version, to standard error.)",
        "fDisplayMsdlFull",
        fDisplayMsdlFull));
}

void msdlInputOahGroup::initializeMsdlLanguagesOptions (
  const std::string& serviceName)
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
    std::stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'nederlands' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

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
              std::regex ("NUMBER"),
              std::to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            std::regex ("PITCHES_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlPitchesLanguage",
        fMsdlQuarterTonesPitchesLanguageKind));

  // MSDR keywords display

  const msdlKeywordsLanguageKind
    msdlKeywordsLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsLanguageEnglish; // MSDL default

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
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            std::regex ("KEYWORDS_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            existingMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlKeywordsLanguageKindDefaultValue)),
        "LANGUAGE",
        serviceName));

  // MSDR tokens display

  const msdlKeywordsLanguageKind
    msdlTokensLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsLanguageEnglish; // MSDL default

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
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            std::regex ("KEYWORDS_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            existingMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlTokensLanguageKindDefaultValue)),
        "LANGUAGE",
        serviceName));
}

void msdlInputOahGroup::initializeMsdlInputOahGroup (
  const std::string& serviceName)
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
Bool msdlInputOahGroup::setMsdlKeywordsLanguage (std::string language)
{
  // is language in the keywords languages map?
  std::map<std::string, msdlKeywordsLanguageKind>::const_iterator
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
Bool msdlInputOahGroup::setMsdlQuarterTonesPitchesLanguage (std::string language)
{
  // is language in the note names languages map?
  std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
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
{}

//______________________________________________________________________________
void msdlInputOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdlInputOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlInputOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlInputOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlInputOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlInputOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlInputOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void msdlInputOahGroup::printMsdlInputOahValues (int fieldWidth)
{
  gLogStream <<
    "The MSDR options are:" <<
    std::endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fDisplayMsdl" << ": " <<
    fDisplayMsdl <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplayMsdlFull" << ": " <<
    fDisplayMsdlFull <<
    std::endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "msdlQuarterTonesPitchesLanguage" << ": \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsdlQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) << "msdlKeywordsLanguage" << ": \"" <<
    msdlKeywordsLanguageKindAsString (
      fMsdlKeywordsLanguageKind) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdlInputOahGroup& elt)
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
S_msdlInputOahGroup createGlobalMsdlInputOahGroup (
  const std::string& serviceName)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global MSDR OAH group" <<
      std::endl;
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
