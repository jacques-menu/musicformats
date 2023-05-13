/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "visitor.h"
#include "oahWae.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "msrGenerationBasicTypes.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrGenerationAPIKindAsString (
  msrGenerationAPIKind generationAPIKind)
{
  std::string result;

  switch (generationAPIKind) {
    case msrGenerationAPIKind::kMsrFunctionsAPIKind:
      result = "functions";
      break;
    case msrGenerationAPIKind::kMsrStringsAPIKind:
      result = "strings";
      break;
  } // switch

  return result;
}

EXP msrGenerationAPIKind msrGenerationAPIKindFromString (
  const std::string& theString)
{
  msrGenerationAPIKind result = msrGenerationAPIKind::kMsrFunctionsAPIKind; // default value

  if      (theString == "functions") {
    result = msrGenerationAPIKind::kMsrFunctionsAPIKind;
  }
  else if (theString == "strings") {
    result = msrGenerationAPIKind::kMsrStringsAPIKind;
  }
  else {
    std::stringstream ss;

    ss <<
      "the string \"" <<
      theString <<
      "\" is no valid generation API kind";

      mfAssert (
        __FILE__, __LINE__,
      false, ss.str ());
  }

  return result;
}

std::map<std::string, msrGenerationAPIKind>
  gGlobalGenerationAPIKindsMap;

void initializeGenerationAPIKindsMap ()
{
  gGlobalGenerationAPIKindsMap ["functions"] = msrGenerationAPIKind::kMsrFunctionsAPIKind;
  gGlobalGenerationAPIKindsMap ["strings"]   = msrGenerationAPIKind::kMsrStringsAPIKind;
}

std::string availableGenerationAPIKinds (
  size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t brailleOutputKindsMapSize =
    gGlobalGenerationAPIKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    size_t nextToLast =
      brailleOutputKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrGenerationAPIKind>::const_iterator i =
        gGlobalGenerationAPIKindsMap.begin ();
      i != gGlobalGenerationAPIKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != brailleOutputKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
S_msrGenerationAPIKindAtom msrGenerationAPIKindAtom::create (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  msrGenerationAPIKind& generationAPIKindVariable)
{
  msrGenerationAPIKindAtom* obj = new
    msrGenerationAPIKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      generationAPIKindVariable);
  assert (obj != nullptr);
  return obj;
}

msrGenerationAPIKindAtom::msrGenerationAPIKindAtom (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  msrGenerationAPIKind& generationAPIKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fGenerationAPIKindVariable (
      generationAPIKindVariable)
{}

msrGenerationAPIKindAtom::~msrGenerationAPIKindAtom ()
{}

void msrGenerationAPIKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> handling atom '" << fetchNames () << "; which is of type 'msrGenerationAPIKindAtom'" <<
      " with value \"" << theString << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map<std::string, msrGenerationAPIKind>::const_iterator
    it =
      gGlobalGenerationAPIKindsMap.find (
        theString);

  if (it == gGlobalGenerationAPIKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    std::stringstream ss;

    ss <<
      "generation API kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalGenerationAPIKindsMap.size () - 1 <<
      " known generation API kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  fGenerationAPIKindVariable =
    (*it).second;

  fSelected = true;
}

void msrGenerationAPIKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrGenerationAPIKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrGenerationAPIKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGenerationAPIKindAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrGenerationAPIKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return ss.str ();
}

std::string msrGenerationAPIKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return ss.str ();
}

void msrGenerationAPIKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "GenerationAPIKindAtom:" <<
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
    "fGenerationAPIKindVariable" << ": " <<
    msrGenerationAPIKindAsString (
      fGenerationAPIKindVariable) <<
    std::endl;

  --gIndenter;
}

void msrGenerationAPIKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName << ": " <<
    msrGenerationAPIKindAsString (
      fGenerationAPIKindVariable);
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

std::ostream& operator << (std::ostream& os, const S_msrGenerationAPIKindAtom& elt)
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
void initializeMsrGenerationAPI ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    initializeGenerationAPIKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}
