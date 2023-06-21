/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "mfConstants.h"

#include "mfMultiGenerationOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
// constants
const std::string K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME = "lilypond";
const std::string K_GENERATED_OUTPUT_KIND_BRAILLE_NAME  = "braille";
const std::string K_GENERATED_OUTPUT_KIND_MUSICXML_NAME = "musicxml";
const std::string K_GENERATED_OUTPUT_KIND_GUIDO_NAME    = "guido";
const std::string K_GENERATED_OUTPUT_KIND_MIDI_NAME     = "midi";

//______________________________________________________________________________
std::string mfMultiGenerationOutputKindAsString (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  // no CamelCase here, these strings are used in the command line options
  std::string result;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      result = "*kGeneration_UNKNOWN_*";
      break;
    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result = "LilyPond";
      break;
    case mfMultiGenerationOutputKind::kGenerationBraille:
      result = "braille";
      break;
    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result = "MusicXML";
      break;
    case mfMultiGenerationOutputKind::kGenerationGuido:
      result = "Guido";
      break;
    case mfMultiGenerationOutputKind::kGenerationMidi:
      result = "MIDI";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfMultiGenerationOutputKind& elt)
{
  os << mfMultiGenerationOutputKindAsString (elt);
  return os;
}

EXP mfMultiGenerationOutputKind mfMultiGenerationOutputKindFromString (
  const std::string& theString)
{
  mfMultiGenerationOutputKind
    result =
      mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;

  if      (theString == K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME) {
    result = mfMultiGenerationOutputKind::kGenerationLilypond;
  }
  else if (theString == K_GENERATED_OUTPUT_KIND_BRAILLE_NAME) {
    result = mfMultiGenerationOutputKind::kGenerationBraille;
  }
  else if (theString == K_GENERATED_OUTPUT_KIND_MUSICXML_NAME) {
    result = mfMultiGenerationOutputKind::kGenerationMusicXML;
  }
  else if (theString == K_GENERATED_OUTPUT_KIND_GUIDO_NAME) {
    result = mfMultiGenerationOutputKind::kGenerationGuido;
  }
  else if (theString == K_GENERATED_OUTPUT_KIND_MIDI_NAME) {
    result = mfMultiGenerationOutputKind::kGenerationMidi;
  }
  else {
    // theString is no generated output kind
  }

  return result;
}

std::map<std::string, mfMultiGenerationOutputKind>
  gGlobalMultiGenerationOutputKindsMap;

void initializeMultiGenerationOutputKindsMap ()
{
  gGlobalMultiGenerationOutputKindsMap [K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME] =
    mfMultiGenerationOutputKind::kGenerationLilypond;
  gGlobalMultiGenerationOutputKindsMap [K_GENERATED_OUTPUT_KIND_BRAILLE_NAME]  =
    mfMultiGenerationOutputKind::kGenerationBraille;
  gGlobalMultiGenerationOutputKindsMap [K_GENERATED_OUTPUT_KIND_MUSICXML_NAME] =
    mfMultiGenerationOutputKind::kGenerationMusicXML;
  gGlobalMultiGenerationOutputKindsMap [K_GENERATED_OUTPUT_KIND_GUIDO_NAME]    =
    mfMultiGenerationOutputKind::kGenerationGuido;
  gGlobalMultiGenerationOutputKindsMap [K_GENERATED_OUTPUT_KIND_MIDI_NAME]     =
    mfMultiGenerationOutputKind::kGenerationMidi;
}

std::string availableMultiGenerationOutputKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t brailleOutputKindsMapSize =
    gGlobalMultiGenerationOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    size_t nextToLast =
      brailleOutputKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, mfMultiGenerationOutputKind>::const_iterator i =
        gGlobalMultiGenerationOutputKindsMap.begin ();
      i != gGlobalMultiGenerationOutputKindsMap.end ();
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
EXP mfMultiGenerationOutputKind fetchGeneratedOutputKindFromRunData ()
{
  mfMultiGenerationOutputKind
    result =
      mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;
//
//   // analyze the options
//   const std::vector<oahOption>&
//     optionsVector =
//       gServiceRunData->
//         getOptionsAndArguments ().getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//       std::string optionName =
//         optionsVector [i].getOptionName ();
//       std::string optionValue =
//         optionsVector [i].getOptionValue ();
//
//       std::string optionNameWithoutDash = optionName.substr (1);
//
// //     gLog << JMI
// //       "optionName: " << optionName <<
// //       ", optionValue: " << optionValue <<
// //       ", optionNameWithoutDash: " << optionNameWithoutDash <<
// //       std::endl;
//
//       // is optionNameWithoutDash one of the multi generators output kinds?
//       mfMultiGenerationOutputKind
//         newMultiGenerationOutputKind =
//           mfMultiGenerationOutputKindFromString (
//             optionNameWithoutDash);
//
//       if (newMultiGenerationOutputKind != mfMultiGenerationOutputKind::kGeneration_UNKNOWN_) {
//         // yes, optionNameWithoutDash is a multi generators output kind
//         if (result != mfMultiGenerationOutputKind::kGeneration_UNKNOWN_) {
//           std::stringstream ss;
//
//           ss <<
//             "mfMultiGenerationOutputKindAtom generated output kind specified more that once" <<
//             std::endl;
//
//           oahError (ss.str ());
//         }
//         else {
//           result = newMultiGenerationOutputKind;
//         }
//       }
//
//       else {
//         // no, , optionNameWithoutDash is NO multi generators output kind
//       }
//     } //for
//   }
//
  gLog <<  // JMIJMIJMI
    "result: " <<
    mfMultiGenerationOutputKindAsString (result) <<
    std::endl;

  return result;
}

//______________________________________________________________________________
S_mfMultiGenerationOutputKindAtom mfMultiGenerationOutputKindAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  mfMultiGenerationOutputKind&
                     multiGenerationOutputKindVariable,
  mfMultiGenerationOutputKind
                     multiGenerationOutputKindValue)
{
  mfMultiGenerationOutputKindAtom* obj = new
    mfMultiGenerationOutputKindAtom (
      longName,
      shortName,
      description,
      variableName,
      multiGenerationOutputKindVariable,
      multiGenerationOutputKindValue);
  assert (obj != nullptr);
  return obj;
}

mfMultiGenerationOutputKindAtom::mfMultiGenerationOutputKindAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName,
  mfMultiGenerationOutputKind&
                     multiGenerationOutputKindVariable,
  mfMultiGenerationOutputKind
                     multiGenerationOutputKindValue)
  : oahAtomImplicitlySettingAVariable (
      longName,
      shortName,
      description,
      variableName),
    fMultiGenerationOutputKindVariable (
      multiGenerationOutputKindVariable),
    fMultiGenerationOutputKindValue ( // this is where the value is supplied
      multiGenerationOutputKindValue)
{
  fMultiGenerationOutputKindVariable =
    mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;
}

mfMultiGenerationOutputKindAtom::~mfMultiGenerationOutputKindAtom ()
{}

void mfMultiGenerationOutputKindAtom::setImplicitVariable (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> handling atom '" <<
      fetchNames () <<
      "; which is of type 'mfMultiGenerationOutputKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fSelected) {
    std::stringstream ss;

    ss <<
      "mfMultiGenerationOutputKindAtom generated output kind" << // JMI ???
      " set more than once" <<
      std::endl;

    oahError (ss.str ());
  }
  else {
    fMultiGenerationOutputKindVariable = fMultiGenerationOutputKindValue;
  }

  fSelected = true;
}

void mfMultiGenerationOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfMultiGenerationOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_mfMultiGenerationOutputKindAtom>*> (v)) {
        S_mfMultiGenerationOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfMultiGenerationOutputKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mfMultiGenerationOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfMultiGenerationOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_mfMultiGenerationOutputKindAtom>*> (v)) {
        S_mfMultiGenerationOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfMultiGenerationOutputKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mfMultiGenerationOutputKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string mfMultiGenerationOutputKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable);

  return ss.str ();
}

std::string mfMultiGenerationOutputKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable);

  return ss.str ();
}

void mfMultiGenerationOutputKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiGenerationOutputKindAtom:" <<
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
    "fMultiGenerationOutputKindVariable" << ": " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMultiGenerationOutputKindValue" << ": " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindValue) <<
    std::endl;

  --gIndenter;
}

void mfMultiGenerationOutputKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fMultiGenerationOutputKindVariable;
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

std::ostream& operator << (std::ostream& os, const S_mfMultiGenerationOutputKindAtom& elt)
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
S_multiGenerationOahGroup gGlobalMultiGenerationOahGroup;

S_multiGenerationOahGroup mfMultiGenerationOahGroup::create ()
{
  mfMultiGenerationOahGroup* obj = new mfMultiGenerationOahGroup ();
  assert (obj != nullptr);
  return obj;
}

mfMultiGenerationOahGroup::mfMultiGenerationOahGroup ()
  : oahGroup (
      "Multi generator",
      "help-multi-generator", "hmg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMultiGenerationOahGroup ();
}

mfMultiGenerationOahGroup::~mfMultiGenerationOahGroup ()
{}

void mfMultiGenerationOahGroup::initializeMultiGenerationOahGroup ()
{
  initializeMultiGenerationOutputKindsMap ();

  // generator output kind
  // --------------------------------------

  initializeMultiGenerationOutputKindOptions ();
}

void mfMultiGenerationOahGroup::initializeMultiGenerationOutputKindOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "help-generated-output", "hgc",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lilypond output

  subGroup->
    appendAtomToSubGroup (
      mfMultiGenerationOutputKindAtom::create (
        K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME, "",
R"(Generate LilyPond code as output (default value).)",
        "fMultiGenerationOutputKind",
        fMultiGenerationOutputKind,
        mfMultiGenerationOutputKind::kGenerationLilypond));

  // braille output

  subGroup->
    appendAtomToSubGroup (
      mfMultiGenerationOutputKindAtom::create (
        K_GENERATED_OUTPUT_KIND_BRAILLE_NAME, "",
R"(Generate braille code as output.)",
        "fMultiGenerationOutputKind",
        fMultiGenerationOutputKind,
        mfMultiGenerationOutputKind::kGenerationBraille));

  // musicxml output

  subGroup->
    appendAtomToSubGroup (
      mfMultiGenerationOutputKindAtom::create (
        K_GENERATED_OUTPUT_KIND_MUSICXML_NAME, "",
R"(Generate MusicXML code as output.)",
        "fMultiGenerationOutputKind",
        fMultiGenerationOutputKind,
        mfMultiGenerationOutputKind::kGenerationMusicXML));

  // guido output

  subGroup->
    appendAtomToSubGroup (
      mfMultiGenerationOutputKindAtom::create (
        K_GENERATED_OUTPUT_KIND_GUIDO_NAME, "",
R"(Generate Guido code as output.)",
        "fMultiGenerationOutputKind",
        fMultiGenerationOutputKind,
        mfMultiGenerationOutputKind::kGenerationGuido));

  // MIDI output

//   subGroup->
//     appendAtomToSubGroup (
//       mfMultiGenerationOutputKindAtom::create (
//         K_GENERATED_OUTPUT_KIND_MIDI_NAME, "",
// R"(Generate MIDI code as output.)",
//         "fMultiGenerationOutputKind",
//         fMultiGenerationOutputKind,
//         mfMultiGenerationOutputKind::kGenerationMidi));
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::enforceGroupQuietness ()
{
  fMultiGenerationOutputKindAtom = mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_multiGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGenerationOahGroup>*> (v)) {
        S_multiGenerationOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfMultiGenerationOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mfMultiGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_multiGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGenerationOahGroup>*> (v)) {
        S_multiGenerationOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfMultiGenerationOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mfMultiGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mfMultiGenerationOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::displayMultiGenerationOahValues (int fieldWidth)
{
  gLog <<
    "The MultiGeneration options are:" <<
    std::endl;

  ++gIndenter;

  // Output kind
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Multigenerator output kind:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    std::setw (fieldWidth) << "fMultiGenerationOutputKind" << ": " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKind) <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_multiGenerationOahGroup& elt)
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
S_multiGenerationOahGroup createGlobalMultiGenerationOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating multi generator OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMultiGenerationOahGroup) {
    // create the multi generator options group
    gGlobalMultiGenerationOahGroup =
      mfMultiGenerationOahGroup::create ();
    assert (gGlobalMultiGenerationOahGroup != 0);
  }

  // return the multi generator OAH group
  return gGlobalMultiGenerationOahGroup;
}


}
