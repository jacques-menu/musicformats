/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "oahHarmoniesExtraOah.h"

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED


#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

#include "visitor.h"

#include "oahWae.h"

#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahOah.h"

#include "msrHarmonies.h"

#include "lpsrOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_extraShowAllHarmoniesStructuresAtom extraShowAllHarmoniesStructuresAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description)
{
  extraShowAllHarmoniesStructuresAtom* o = new
    extraShowAllHarmoniesStructuresAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

extraShowAllHarmoniesStructuresAtom::extraShowAllHarmoniesStructuresAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      "valueSpecification JMI",
      "fVariableName JMI")
{}

extraShowAllHarmoniesStructuresAtom::~extraShowAllHarmoniesStructuresAtom ()
{}

/* JMI
void extraShowAllHarmoniesStructuresAtom::applyElement (std::ostream& os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a extraShowAllHarmoniesStructuresAtom" <<
      std::endl;
  }
#endif

  printAllHarmoniesStructures (os);
}
*/

void extraShowAllHarmoniesStructuresAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a extraShowAllHarmoniesStructuresAtom" <<
      std::endl;
  }
#endif

  printAllHarmoniesStructures (os);
}

void extraShowAllHarmoniesStructuresAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraShowAllHarmoniesStructuresAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowAllHarmoniesStructuresAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraShowAllHarmoniesStructuresAtom::printAllHarmoniesStructures (std::ostream& os) const
{
  msrHarmonyStructure::printAllHarmoniesStructures (os);
}

void extraShowAllHarmoniesStructuresAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt)
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
S_extraShowAllHarmoniesContentsAtom extraShowAllHarmoniesContentsAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraShowAllHarmoniesContentsAtom* o = new
    extraShowAllHarmoniesContentsAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraShowAllHarmoniesContentsAtom::extraShowAllHarmoniesContentsAtom (
  const std::string& shortName,
  const std::string& longName,
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
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;
}

extraShowAllHarmoniesContentsAtom::~extraShowAllHarmoniesContentsAtom ()
{}

void extraShowAllHarmoniesContentsAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowAllHarmoniesContentsAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowAllHarmoniesContentsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      std::endl;
  }
#endif

  // fetch the semitones pitch from theString
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        theString);

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      {
        std::stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
          "'" <<
          std::endl;

        oahError (s.str ());
      }
      break;

    case msrSemiTonesPitchKind::kSTP_A_Flat:
    case msrSemiTonesPitchKind::kSTP_A_Natural:
    case msrSemiTonesPitchKind::kSTP_A_Sharp:

    case msrSemiTonesPitchKind::kSTP_B_Flat:
    case msrSemiTonesPitchKind::kSTP_B_Natural:
    case msrSemiTonesPitchKind::kSTP_B_Sharp:

    case msrSemiTonesPitchKind::kSTP_C_Flat:
    case msrSemiTonesPitchKind::kSTP_C_Natural:
    case msrSemiTonesPitchKind::kSTP_C_Sharp:

    case msrSemiTonesPitchKind::kSTP_D_Flat:
    case msrSemiTonesPitchKind::kSTP_D_Natural:
    case msrSemiTonesPitchKind::kSTP_D_Sharp:

    case msrSemiTonesPitchKind::kSTP_E_Flat:
    case msrSemiTonesPitchKind::kSTP_E_Natural:
    case msrSemiTonesPitchKind::kSTP_E_Sharp:

    case msrSemiTonesPitchKind::kSTP_F_Flat:
    case msrSemiTonesPitchKind::kSTP_F_Natural:
    case msrSemiTonesPitchKind::kSTP_F_Sharp:

    case msrSemiTonesPitchKind::kSTP_G_Flat:
    case msrSemiTonesPitchKind::kSTP_G_Natural:
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      break;

    default:
      {
        std::stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          std::endl;

        oahError (s.str ());
      }
  } // switch

  // print all the harmonies notes
  printAllHarmoniesContents (
    gLogStream,
    semiTonesPitchKind);
}

void extraShowAllHarmoniesContentsAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraShowAllHarmoniesContentsAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowAllHarmoniesContentsAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraShowAllHarmoniesContentsAtom::printAllHarmoniesContents (
  std::ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{
  msrHarmonyContents::printAllHarmoniesContents (
    os,
    semiTonesPitchKind);
}

void extraShowAllHarmoniesContentsAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt)
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
S_extraShowHarmonyDetailsAtom extraShowHarmonyDetailsAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraShowHarmonyDetailsAtom* o = new
    extraShowHarmonyDetailsAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraShowHarmonyDetailsAtom::extraShowHarmonyDetailsAtom (
  const std::string& shortName,
  const std::string& longName,
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
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;
}

extraShowHarmonyDetailsAtom::~extraShowHarmonyDetailsAtom ()
{}

void extraShowHarmonyDetailsAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyDetailsAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyDetailsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      std::endl;
  }
#endif

  // decipher theString with a regular expression
  std::string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*");

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Harmony details string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'" <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      smSize << " elements: ";
    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for
    gLogStream << std::endl;
  }
#endif

  if (smSize == 3) { // JMI ???
  }

  else {
    std::stringstream s;

    s <<
      "-Harmony details argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  std::string
    rootName    = sm [1],
    harmonyName = sm [2];

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      std::endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      {
        std::stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
          "'" <<
          std::endl;

        oahError (s.str ());
      }
      break;

    case msrSemiTonesPitchKind::kSTP_A_Flat:
    case msrSemiTonesPitchKind::kSTP_A_Natural:
    case msrSemiTonesPitchKind::kSTP_A_Sharp:

    case msrSemiTonesPitchKind::kSTP_B_Flat:
    case msrSemiTonesPitchKind::kSTP_B_Natural:
    case msrSemiTonesPitchKind::kSTP_B_Sharp:

    case msrSemiTonesPitchKind::kSTP_C_Flat:
    case msrSemiTonesPitchKind::kSTP_C_Natural:
    case msrSemiTonesPitchKind::kSTP_C_Sharp:

    case msrSemiTonesPitchKind::kSTP_D_Flat:
    case msrSemiTonesPitchKind::kSTP_D_Natural:
    case msrSemiTonesPitchKind::kSTP_D_Sharp:

    case msrSemiTonesPitchKind::kSTP_E_Flat:
    case msrSemiTonesPitchKind::kSTP_E_Natural:
    case msrSemiTonesPitchKind::kSTP_E_Sharp:

    case msrSemiTonesPitchKind::kSTP_F_Flat:
    case msrSemiTonesPitchKind::kSTP_F_Natural:
    case msrSemiTonesPitchKind::kSTP_F_Sharp:

    case msrSemiTonesPitchKind::kSTP_G_Flat:
    case msrSemiTonesPitchKind::kSTP_G_Natural:
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      break;

    default:
      {
        std::stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          std::endl;

        oahError (s.str ());
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  // print the Harmony details
  printHarmonyDetails (
    gLogStream,
    semiTonesPitchKind,
    harmonyKind);
}

void extraShowHarmonyDetailsAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyDetailsAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyDetailsAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraShowHarmonyDetailsAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyDetailsAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraShowHarmonyDetailsAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

//______________________________________________________________________________
S_extraShowHarmonyAnalysisAtom extraShowHarmonyAnalysisAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraShowHarmonyAnalysisAtom* o = new
    extraShowHarmonyAnalysisAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraShowHarmonyAnalysisAtom::extraShowHarmonyAnalysisAtom (
  const std::string& shortName,
  const std::string& longName,
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
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;
}

extraShowHarmonyAnalysisAtom::~extraShowHarmonyAnalysisAtom ()
{}

void extraShowHarmonyAnalysisAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyAnalysisAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyAnalysisAtom'" <<
      ", theString = \"" << theString << "\"" <<
      std::endl;
  }
#endif

  // decipher theString with a regular expression
  std::string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]+"
    "([[:alnum:]]+)"
    "[[:space:]]+"
    "([[:digit:]]+)"
    "[[:space:]]*");

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Harmony analysis string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'" <<
      std::endl;
  }
#endif

  if (smSize == 4) {
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        smSize << " elements: ";
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for
      gLogStream << std::endl;
    }
#endif
  }

  else {
    std::stringstream s;

    s <<
      "Harmony analysis argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());

/* JMI ??? should work...
    printSubGroupSpecificHelp (
      gLogStream,
      showHarmonyAnalysisAtom->
        getUpLinkToSubGroup ());
*/
  }

  std::string
    rootName          = sm [1],
    harmonyName       = sm [2],
    inversionAsString = sm [3];

  int inversion;

  {
    std::stringstream s;

    s << inversionAsString;

    s >> inversion;
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      "--> inversion: " << inversion <<
      std::endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      {
        std::stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
          "'" <<
          std::endl;

        oahError (s.str ());
      }
      break;

    case msrSemiTonesPitchKind::kSTP_A_Flat:
    case msrSemiTonesPitchKind::kSTP_A_Natural:
    case msrSemiTonesPitchKind::kSTP_A_Sharp:

    case msrSemiTonesPitchKind::kSTP_B_Flat:
    case msrSemiTonesPitchKind::kSTP_B_Natural:
    case msrSemiTonesPitchKind::kSTP_B_Sharp:

    case msrSemiTonesPitchKind::kSTP_C_Flat:
    case msrSemiTonesPitchKind::kSTP_C_Natural:
    case msrSemiTonesPitchKind::kSTP_C_Sharp:

    case msrSemiTonesPitchKind::kSTP_D_Flat:
    case msrSemiTonesPitchKind::kSTP_D_Natural:
    case msrSemiTonesPitchKind::kSTP_D_Sharp:

    case msrSemiTonesPitchKind::kSTP_E_Flat:
    case msrSemiTonesPitchKind::kSTP_E_Natural:
    case msrSemiTonesPitchKind::kSTP_E_Sharp:

    case msrSemiTonesPitchKind::kSTP_F_Flat:
    case msrSemiTonesPitchKind::kSTP_F_Natural:
    case msrSemiTonesPitchKind::kSTP_F_Sharp:

    case msrSemiTonesPitchKind::kSTP_G_Flat:
    case msrSemiTonesPitchKind::kSTP_G_Natural:
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      break;

    default:
      {
        std::stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          std::endl;

        oahError (s.str ());
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  if (harmonyKind == msrHarmonyKind::kHarmony_UNKNOWN) {
    std::stringstream s;

    s <<
      "'" << harmonyName <<
      "' is no harmony name, valid names are:" <<
      std::endl <<
      "maj, min,aug, dim, dom, maj7, min7, dim7, aug7, halfdim, minmaj7, maj6, min6, dom9, maj9, min9, dom11, maj11, min11, dom13, maj13, min13, sus2, sus4, neapolitan, italian, french, german, pedal, power, tristan, minmaj9, domsus4, domaug5, dommin9, domaug9dim5, domaug9aug5, domaug11 and maj7aug11" <<
      std::endl;

    oahError (s.str ());
  }

  // print the Harmony analysis
  printHarmonyAnalysis (
    gLogStream,
    semiTonesPitchKind,
    harmonyKind,
    inversion);
}

void extraShowHarmonyAnalysisAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyAnalysisAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyAnalysisAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraShowHarmonyAnalysisAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyAnalysisAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraShowHarmonyAnalysisAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

//_______________________________________________________________________________

S_harmoniesExtraOahGroup gGlobalHarmoniesExtraOahGroup;

S_harmoniesExtraOahGroup harmoniesExtraOahGroup::create ()
{
  harmoniesExtraOahGroup* o = new harmoniesExtraOahGroup ();
  assert (o != nullptr);
  return o;
}

harmoniesExtraOahGroup::harmoniesExtraOahGroup ()
  : oahGroup (
    "Extra",
    "help-extra", "he",
R"(These extra provide features not related to conversion from MusicXML to other formats.
In the text below:
  - ROOT_DIATONIC_PITCH should belong to the names available in
    the selected MSR pitches language, "nederlands" by default;
  - other languages can be chosen with the '-mpl, -msr-pitches-language' option;
  - HARMONY_NAME should be one of:
      MusicXML harmonies:
        "maj", "min", "aug", "dim", "dom",
        "maj7", "min7", "dim7", "aug7", "halfdim", "minmaj7",
        "maj6", "min6", "dom9", "maj9", "min9", "dom11", "maj11", "min11",
        "dom13", "maj13", "min13", "sus2", "sus4",
        "neapolitan", "italian", "french", "german"
      Jazz-specific harmonies:
        "pedal", "power", "tristan", "minmaj9", "domsus4", "domaug5",
        "dommin9", "domaug9dim5", "domaug9aug5", "domaug11", "maj7aug11"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeHarmoniesExtraOahGroup ();
}

harmoniesExtraOahGroup::~harmoniesExtraOahGroup ()
{}

void harmoniesExtraOahGroup::initializeExtraShowAllHarmoniesStructuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies structures",
        "help-extra-harmonies-structures", "hecs",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowAllHarmoniesStructuresAtom::create (
        "show-harmonies-structures", "scs",
R"(Write all known harmonies structures to standard output.)"));
}

void harmoniesExtraOahGroup::initializeExtraShowAllHarmoniesContentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies contents",
        "help-extra-harmonies-contents", "hecc",
//          regex_replace (
//            std::regex ("HARMONIES_KINDS"),
//            existingHarmonyKindsNames ()
//    HARMONIES_KINDS.
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowAllHarmoniesContentsAtom::create (
        "show-all-harmonies-contents", "sacc",
R"(Write all harmonies contents for the given diatonic (semitones) PITCH,
supplied in the current language to standard output.)",
        "PITCH",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void harmoniesExtraOahGroup::initializeExtraShowHarmonyDetailsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmony details",
        "help-extra-harmony-details", "hecd",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowHarmonyDetailsAtom::create (
        "show-harmony-details", "scd",
        regex_replace (
R"(Write the details of the harmony for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.
HARMONY_SPEC should be of the form ROOT_DIATONIC_PITCH:HARMONY_NAME .
There can be spaces around the ':', in which case quoting is needed.")",
         std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "HARMONY_SPEC",
        "fHarmoniesRootAsString",
        fHarmoniesRootAsString));
}

void harmoniesExtraOahGroup::initializeExtraShowHarmonyAnalysisOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmony analysis",
        "help-extra-harmony-analysis", "heca",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowHarmonyAnalysisAtom::create (
        "show-harmony-analysis", "sca", // -sca "c dommin9 0"
        regex_replace (
R"(Write an analysis of the harmony for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.
HARMONY_SPEC should be of the form ROOT_DIATONIC_PITCH:HARMONY_NAME INVERSION .")",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "HARMONY_SPEC",
        "fHarmoniesRootAsString",
        fHarmoniesRootAsString));
}

void harmoniesExtraOahGroup::initializeHarmoniesExtraOahGroup ()
{
  // show all harmonies structures
  // --------------------------------------
  initializeExtraShowAllHarmoniesStructuresOptions ();

  // show all harmonies contents
  // --------------------------------------
  initializeExtraShowAllHarmoniesContentsOptions ();

  // show harmony details
  // --------------------------------------
  initializeExtraShowHarmonyDetailsOptions ();

  // show harmony analysis
  // --------------------------------------
  initializeExtraShowHarmonyAnalysisOptions ();
}

//______________________________________________________________________________
void harmoniesExtraOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void harmoniesExtraOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void harmoniesExtraOahGroup::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> harmoniesExtraOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_harmoniesExtraOahGroup>*
    p =
      dynamic_cast<visitor<S_harmoniesExtraOahGroup>*> (v)) {
        S_harmoniesExtraOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching harmoniesExtraOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void harmoniesExtraOahGroup::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> harmoniesExtraOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_harmoniesExtraOahGroup>*
    p =
      dynamic_cast<visitor<S_harmoniesExtraOahGroup>*> (v)) {
        S_harmoniesExtraOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching harmoniesExtraOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void harmoniesExtraOahGroup::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> harmoniesExtraOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void harmoniesExtraOahGroup::printHarmoniesExtraOahValues (int fieldWidth)
{
  gLogStream <<
    "The extra extra are:" << // JMI
    std::endl;

  ++gIndenter;

  // harmony intervals
  // --------------------------------------

  // harmony notes
  // --------------------------------------

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_harmoniesExtraOahGroup& elt)
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
S_harmoniesExtraOahGroup createGlobalHarmoniesExtraOahGroup ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global extra OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalHarmoniesExtraOahGroup) {
    // create the global OAH group
    gGlobalHarmoniesExtraOahGroup =
      harmoniesExtraOahGroup::create ();
    assert (gGlobalHarmoniesExtraOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalHarmoniesExtraOahGroup;
}


}


#endif
