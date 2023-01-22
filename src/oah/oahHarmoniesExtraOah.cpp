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


#include "oahOah.h"

#include "msrHarmonies.h"

#include "lpsrOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_extraDisplayAllHarmoniesStructuresAtom extraDisplayAllHarmoniesStructuresAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description)
{
  extraDisplayAllHarmoniesStructuresAtom* o = new
    extraDisplayAllHarmoniesStructuresAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

extraDisplayAllHarmoniesStructuresAtom::extraDisplayAllHarmoniesStructuresAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{}

extraDisplayAllHarmoniesStructuresAtom::~extraDisplayAllHarmoniesStructuresAtom ()
{}

void extraDisplayAllHarmoniesStructuresAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a extraDisplayAllHarmoniesStructuresAtom" <<
      std::endl;
  }
#endif

  printAllHarmoniesStructures (os);

	fSelected = true;
}
//
// void extraDisplayAllHarmoniesStructuresAtom::applyAtomWithValue (
//   const std::string& theString,
//   std::ostream&      os)
// {
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     gLogStream <<
//       "==> option '" << fetchNames () << "' is a extraDisplayAllHarmoniesStructuresAtom" <<
//       std::endl;
//   }
// #endif
//
//   printAllHarmoniesStructures (os);
// }

void extraDisplayAllHarmoniesStructuresAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesStructuresAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayAllHarmoniesStructuresAtom>*> (v)) {
        S_extraDisplayAllHarmoniesStructuresAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayAllHarmoniesStructuresAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraDisplayAllHarmoniesStructuresAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesStructuresAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayAllHarmoniesStructuresAtom>*> (v)) {
        S_extraDisplayAllHarmoniesStructuresAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayAllHarmoniesStructuresAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraDisplayAllHarmoniesStructuresAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesStructuresAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraDisplayAllHarmoniesStructuresAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraDisplayAllHarmoniesStructuresAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraDisplayAllHarmoniesStructuresAtom::printAllHarmoniesStructures (std::ostream& os) const
{
  msrHarmonyStructure::printAllHarmoniesStructures (os);
}

void extraDisplayAllHarmoniesStructuresAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_extraDisplayAllHarmoniesStructuresAtom& elt)
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
S_extraDisplayAllHarmoniesContentsAtom extraDisplayAllHarmoniesContentsAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraDisplayAllHarmoniesContentsAtom* o = new
    extraDisplayAllHarmoniesContentsAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraDisplayAllHarmoniesContentsAtom::extraDisplayAllHarmoniesContentsAtom (
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

extraDisplayAllHarmoniesContentsAtom::~extraDisplayAllHarmoniesContentsAtom ()
{}

void extraDisplayAllHarmoniesContentsAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayAllHarmoniesContentsAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayAllHarmoniesContentsAtom'" <<
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

void extraDisplayAllHarmoniesContentsAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesContentsAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayAllHarmoniesContentsAtom>*> (v)) {
        S_extraDisplayAllHarmoniesContentsAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayAllHarmoniesContentsAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraDisplayAllHarmoniesContentsAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesContentsAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayAllHarmoniesContentsAtom>*> (v)) {
        S_extraDisplayAllHarmoniesContentsAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayAllHarmoniesContentsAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraDisplayAllHarmoniesContentsAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayAllHarmoniesContentsAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraDisplayAllHarmoniesContentsAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraDisplayAllHarmoniesContentsAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraDisplayAllHarmoniesContentsAtom::printAllHarmoniesContents (
  std::ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{
  msrHarmonyContents::printAllHarmoniesContents (
    os,
    semiTonesPitchKind);
}

void extraDisplayAllHarmoniesContentsAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_extraDisplayAllHarmoniesContentsAtom& elt)
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
S_extraDisplayHarmonyDetailsAtom extraDisplayHarmonyDetailsAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraDisplayHarmonyDetailsAtom* o = new
    extraDisplayHarmonyDetailsAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraDisplayHarmonyDetailsAtom::extraDisplayHarmonyDetailsAtom (
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

extraDisplayHarmonyDetailsAtom::~extraDisplayHarmonyDetailsAtom ()
{}

void extraDisplayHarmonyDetailsAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayHarmonyDetailsAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayHarmonyDetailsAtom'" <<
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Harmony details string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'" <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
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

#ifdef MF_TRACING_IS_ENABLED
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

void extraDisplayHarmonyDetailsAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyDetailsAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayHarmonyDetailsAtom>*> (v)) {
        S_extraDisplayHarmonyDetailsAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayHarmonyDetailsAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraDisplayHarmonyDetailsAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyDetailsAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayHarmonyDetailsAtom>*> (v)) {
        S_extraDisplayHarmonyDetailsAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayHarmonyDetailsAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraDisplayHarmonyDetailsAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyDetailsAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraDisplayHarmonyDetailsAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraDisplayHarmonyDetailsAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraDisplayHarmonyDetailsAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

//______________________________________________________________________________
S_extraDisplayHarmonyAnalysisAtom extraDisplayHarmonyAnalysisAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::string&       stringVariable)
{
  extraDisplayHarmonyAnalysisAtom* o = new
    extraDisplayHarmonyAnalysisAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o != nullptr);
  return o;
}

extraDisplayHarmonyAnalysisAtom::extraDisplayHarmonyAnalysisAtom (
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

extraDisplayHarmonyAnalysisAtom::~extraDisplayHarmonyAnalysisAtom ()
{}

void extraDisplayHarmonyAnalysisAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayHarmonyAnalysisAtom'" <<
      std::endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraDisplayHarmonyAnalysisAtom'" <<
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

#ifdef MF_TRACING_IS_ENABLED
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
#ifdef MF_TRACING_IS_ENABLED
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

#ifdef MF_TRACING_IS_ENABLED
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

void extraDisplayHarmonyAnalysisAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyAnalysisAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayHarmonyAnalysisAtom>*> (v)) {
        S_extraDisplayHarmonyAnalysisAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayHarmonyAnalysisAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraDisplayHarmonyAnalysisAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyAnalysisAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_extraDisplayHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraDisplayHarmonyAnalysisAtom>*> (v)) {
        S_extraDisplayHarmonyAnalysisAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraDisplayHarmonyAnalysisAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraDisplayHarmonyAnalysisAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraDisplayHarmonyAnalysisAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void extraDisplayHarmonyAnalysisAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraDisplayHarmonyAnalysisAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void extraDisplayHarmonyAnalysisAtom::displayAtomWithVariableOptionsValues (
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
  - other languages can be selected with the '-mpl, -msr-pitches-language' option;
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

void harmoniesExtraOahGroup::initializeExtraDisplayAllHarmoniesStructuresOptions ()
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
      extraDisplayAllHarmoniesStructuresAtom::create (
        "display-harmonies-structures", "dhs",
R"(Write all known harmonies structures to standard output.)"));
}

void harmoniesExtraOahGroup::initializeExtraDisplayAllHarmoniesContentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies contents",
        "help-extra-harmonies-contents", "hecc",
//          regex_replace (
//            std::regex ("HARMONIES_KINDS"),
//            availableHarmonyKindsNames ()
//    HARMONIES_KINDS.
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraDisplayAllHarmoniesContentsAtom::create (
        "display-all-harmonies-contents", "dahc",
R"(Write all harmonies contents for the given diatonic (semitones) PITCH,
supplied in the current language to standard output.)",
        "PITCH",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void harmoniesExtraOahGroup::initializeExtraDisplayHarmonyDetailsOptions ()
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
      extraDisplayHarmonyDetailsAtom::create (
        "display-harmony-details", "dhd",
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

void harmoniesExtraOahGroup::initializeExtraDisplayHarmonyAnalysisOptions ()
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
      extraDisplayHarmonyAnalysisAtom::create (
        "display-harmony-analysis", "dha", // -sca "c dommin9 0"
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
  initializeExtraDisplayAllHarmoniesStructuresOptions ();

  // show all harmonies contents
  // --------------------------------------
  initializeExtraDisplayAllHarmoniesContentsOptions ();

  // show harmony details
  // --------------------------------------
  initializeExtraDisplayHarmonyDetailsOptions ();

  // show harmony analysis
  // --------------------------------------
  initializeExtraDisplayHarmonyAnalysisOptions ();
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
#ifdef MF_TRACING_IS_ENABLED
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

#ifdef MF_TRACING_IS_ENABLED
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
#ifdef MF_TRACING_IS_ENABLED
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

#ifdef MF_TRACING_IS_ENABLED
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> harmoniesExtraOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void harmoniesExtraOahGroup::displayHarmoniesExtraOahValues (int fieldWidth)
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
#ifdef MF_TRACING_IS_ENABLED
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
