/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "mfConstants.h"
#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "msdlInputOah.h"
#include "msdl2msrOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "msdlTokens.h"

#include "waeHandlers.h"


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
  msdlKeywordsLanguageAtom* obj = new
    msdlKeywordsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlKeywordsLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlKeywordsLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream ss;

    ss <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setMsdlKeywordsLanguageKindVariable (
    (*it).second);

  fSelected = true;
}

void msdlKeywordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlKeywordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlKeywordsLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlKeywordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlKeywordsLanguageAtom>*> (v)) {
        S_msdlKeywordsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlKeywordsLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlKeywordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlKeywordsLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msdlKeywordsLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return ss.str ();
}

std::string msdlKeywordsLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    msdlKeywordsLanguageKindAsString (fMsdlKeywordsLanguageKindVariable);

  return ss.str ();
}

void msdlKeywordsLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

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

void msdlKeywordsLanguageAtom::displayAtomWithVariableOptionsValues (
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

std::ostream& operator << (std::ostream& os, const S_msdlKeywordsLanguageAtom& elt)
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
S_oahDisplayMsdlKeywordsInLanguageAtom oahDisplayMsdlKeywordsInLanguageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahDisplayMsdlKeywordsInLanguageAtom* obj = new
    oahDisplayMsdlKeywordsInLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
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

  std::map <std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream ss;

    ss <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing keywords names
  gLog <<
    "The keywords in language '" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "' are: " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    availableKeywordsInLanguage (
      keywordsLanguageKind,
      K_MF_NAMES_LIST_MAX_LENGTH),
    gLog);

  --gIndenter;

  fSelected = true;
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlKeywordsInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlKeywordsInLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayMsdlKeywordsInLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlKeywordsInLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlKeywordsInLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahDisplayMsdlKeywordsInLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahDisplayMsdlKeywordsInLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahDisplayMsdlKeywordsInLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlKeywordsInLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  displayAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahDisplayMsdlKeywordsInLanguageAtom& elt)
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
S_oahDisplayMsdlTokensInLanguageAtom oahDisplayMsdlTokensInLanguageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahDisplayMsdlTokensInLanguageAtom* obj = new
    oahDisplayMsdlTokensInLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      serviceName);
  assert (obj != nullptr);
  return obj;
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

  std::map <std::string, msdlKeywordsLanguageKind>::const_iterator
    it =
      gGlobalMsdlKeywordsLanguageKindsMap.find (theString);

  if (it == gGlobalMsdlKeywordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream ss;

    ss <<
      "MSDR keywords display language \"" << theString <<
      "\" is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlKeywordsLanguageKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  // get the keywords language kind
  msdlKeywordsLanguageKind
    keywordsLanguageKind =
      (*it).second;

  // write the existing tokens
  gLog <<
    "The tokens in language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsLanguageKind) <<
    "\" are (<...> denotes a class   of tokens): " <<
    std::endl;

  ++gIndenter;

  gIndenter.indentMultiLineString (
    existingTokensInLanguage (
      keywordsLanguageKind,
      K_MF_NAMES_LIST_MAX_LENGTH),
    gLog);

  --gIndenter;

  fSelected = true;
}

void oahDisplayMsdlTokensInLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahDisplayMsdlTokensInLanguageAtom>*
    p =
      dynamic_cast<visitor<S_oahDisplayMsdlTokensInLanguageAtom>*> (v)) {
        S_oahDisplayMsdlTokensInLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahDisplayMsdlTokensInLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahDisplayMsdlTokensInLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahDisplayMsdlTokensInLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahDisplayMsdlTokensInLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName;

  return ss.str ();
}

std::string oahDisplayMsdlTokensInLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName;

  return ss.str ();
}

void oahDisplayMsdlTokensInLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahDisplayMsdlTokensInLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  displayAtomWithVariableOptionsValues (
    os, fieldWidth);

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahDisplayMsdlTokensInLanguageAtom& elt)
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
S_msdlCommentsTypeAtom msdlCommentsTypeAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  msdlCommentsTypeKind&
                    msdlCommentsTypeKindVariable)
{
  msdlCommentsTypeAtom* obj = new
    msdlCommentsTypeAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlCommentsTypeKindVariable);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the keywords languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlCommentsTypeAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream ss;

    ss <<
      "MSDR keywords language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDR keywords languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlCommentsTypeKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setMsdlCommentsTypeKindVariable (
    (*it).second);

  fSelected = true;
}

void msdlCommentsTypeAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlCommentsTypeAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlCommentsTypeAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlCommentsTypeAtom>*
    p =
      dynamic_cast<visitor<S_msdlCommentsTypeAtom>*> (v)) {
        S_msdlCommentsTypeAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlCommentsTypeAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlCommentsTypeAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlCommentsTypeAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msdlCommentsTypeAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return ss.str ();
}

std::string msdlCommentsTypeAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    msdlCommentsTypeKindAsString (fMsdlCommentsTypeKindVariable);

  return ss.str ();
}

void msdlCommentsTypeAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

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

void msdlCommentsTypeAtom::displayAtomWithVariableOptionsValues (
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

std::ostream& operator << (std::ostream& os, const S_msdlCommentsTypeAtom& elt)
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
S_msdlUserLanguageAtom msdlUserLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  msdlUserLanguageKind&
                    msdlUserLanguageKindVariable)
{
  msdlUserLanguageAtom* obj = new
    msdlUserLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlUserLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the User languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlUserLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, language is unknown in the map

    std::stringstream ss;

    ss <<
      "MSDR User language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () <<
      " known MSDR user languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlUserLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setMsdlUserLanguageKindVariable (
    (*it).second);

  fSelected = true;
}

void msdlUserLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlUserLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlUserLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlUserLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlUserLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlUserLanguageAtom>*> (v)) {
        S_msdlUserLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlUserLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlUserLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlUserLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msdlUserLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return ss.str ();
}

std::string msdlUserLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    msdlUserLanguageKindAsString (
      fMsdlUserLanguageVariable);

  return ss.str ();
}

void msdlUserLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

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

void msdlUserLanguageAtom::displayAtomWithVariableOptionsValues (
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

std::ostream& operator << (std::ostream& os, const S_msdlUserLanguageAtom& elt)
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
S_msdlPitchesLanguageAtom msdlPitchesLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    msdlPitchesLanguageKindVariable)
{
  msdlPitchesLanguageAtom* obj = new
    msdlPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      msdlPitchesLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msdlPitchesLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (
        theString);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map

    std::stringstream ss;

    ss <<
      "MSDR pitches language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);

  fSelected = true;
}

void msdlPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlPitchesLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msdlPitchesLanguageAtom>*> (v)) {
        S_msdlPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlPitchesLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlPitchesLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msdlPitchesLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

std::string msdlPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

void msdlPitchesLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

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
    fMsrQuarterTonesPitchesLanguageKindVariable <<
    std::endl;

  --gIndenter;
}

void msdlPitchesLanguageAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fMsrQuarterTonesPitchesLanguageKindVariable;
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

std::ostream& operator << (std::ostream& os, const S_msdlPitchesLanguageAtom& elt)
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
S_msdlInputOahGroup gGlobalMsdlInputOahGroup;

S_msdlInputOahGroup msdlInputOahGroup::create (
  const std::string& serviceName)
{
  msdlInputOahGroup* obj = new msdlInputOahGroup (
    serviceName);
  assert (obj != nullptr);
  return obj;
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
    std::stringstream ss;

    ss <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'nederlands' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
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
            availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
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
            availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
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
            availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
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
  std::map <std::string, msdlKeywordsLanguageKind>::const_iterator
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
  std::map <std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlInputOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlInputOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlInputOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlInputOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlInputOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlInputOahGroup>*> (v)) {
        S_msdlInputOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlInputOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlInputOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlInputOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdlInputOahGroup::displayMsdlInputOahValues (int fieldWidth)
{
  gLog <<
    "The MSDR options are:" <<
    std::endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLog <<
    "Display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fDisplayMsdl" << ": " <<
    fDisplayMsdl <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplayMsdlFull" << ": " <<
    fDisplayMsdlFull <<
    std::endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLog <<
    "Languages:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "msdlQuarterTonesPitchesLanguage" << ": \"" <<
    fMsdlQuarterTonesPitchesLanguageKind <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msdlInputOahGroup createGlobalMsdlInputOahGroup (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global MSDR OAH group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
