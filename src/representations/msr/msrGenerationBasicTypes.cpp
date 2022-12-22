/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "visitor.h"
#include "oahWae.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "msrGenerationBasicTypes.h"


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
    std::stringstream s;

    s <<
      "the string \"" <<
      theString <<
      "\" is no valid generation API kind";

      mfAssert (
        __FILE__, __LINE__,
      false, s.str ());
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

std::string existingGenerationAPIKinds (
  size_t namesListMaxLength)
{
  std::stringstream s;

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
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != brailleOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
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
  msrGenerationAPIKindAtom* o = new
    msrGenerationAPIKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      generationAPIKindVariable);
  assert (o != nullptr);
  return o;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'msrGenerationAPIKindAtom'" <<
      " with value \"" << theString << "\"" <<
      std::endl;
  }
#endif

  std::map<std::string, msrGenerationAPIKind>::const_iterator
    it =
      gGlobalGenerationAPIKindsMap.find (
        theString);

  if (it == gGlobalGenerationAPIKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    std::stringstream s;

    s <<
      "generation API kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalGenerationAPIKindsMap.size () - 1 <<
      " known generation API kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGenerationAPIKindVariable =
    (*it).second;
  fSetByAnOption = true;
}

void msrGenerationAPIKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrGenerationAPIKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrGenerationAPIKindAtom>*
    p =
      dynamic_cast<visitor<S_msrGenerationAPIKindAtom>*> (v)) {
        S_msrGenerationAPIKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGenerationAPIKindAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrGenerationAPIKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGenerationAPIKindAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string msrGenerationAPIKindAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return s.str ();
}

std::string msrGenerationAPIKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrGenerationAPIKindAsString (fGenerationAPIKindVariable);

  return s.str ();
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

void msrGenerationAPIKindAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName << ": " <<
    msrGenerationAPIKindAsString (
      fGenerationAPIKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrGenerationAPIKindAtom& elt)
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
