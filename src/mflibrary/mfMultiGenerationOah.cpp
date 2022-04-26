/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msrBasicTypes.h"        // for " K_NAMES_LIST_MAX_LENGTH

#include "mfMultiGenerationOah.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
// constants
const string K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME = "lilypond";
const string K_GENERATED_OUTPUT_KIND_BRAILLE_NAME  = "braille";
const string K_GENERATED_OUTPUT_KIND_MUSICXML_NAME = "musicxml";
const string K_GENERATED_OUTPUT_KIND_GUIDO_NAME    = "guido";
const string K_GENERATED_OUTPUT_KIND_MIDI_NAME     = "midi";

//______________________________________________________________________________
string mfMultiGenerationOutputKindAsString (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  // no CamelCase here, these strings are used in the command line options
  string result;

//   switch (multiGenerationOutputKind) {
//     case mfMultiGenerationOutputKind::k_NoGeneration:
//       result = "*noGenerateOutputKind*";
//       break;
//     case mfMultiGenerationOutputKind::kGenerationLilypond:
//       result = "LilyPond output";
//       break;
//     case mfMultiGenerationOutputKind::kGenerationBraille:
//       result = "braille output";
//       break;
//     case mfMultiGenerationOutputKind::kGenerationMusicXML:
//       result = "MusicXML output";
//       break;
//     case mfMultiGenerationOutputKind::kGenerationGuido:
//       result = "Guido output";
//       break;
//     case mfMultiGenerationOutputKind::kGenerationMidi:
//       result = "MIDI output";
//       break;
//   } // switch

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
      result = "*k_NoOutpu*t";
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

EXP mfMultiGenerationOutputKind mfMultiGenerationOutputKindFromString (
  const string& theString)
{
  mfMultiGenerationOutputKind
    result =
      mfMultiGenerationOutputKind::k_NoGeneration;

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

map<string, mfMultiGenerationOutputKind>
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

string existingMultiGenerationOutputKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t brailleOutputKindsMapSize =
    gGlobalMultiGenerationOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    size_t nextToLast =
      brailleOutputKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, mfMultiGenerationOutputKind>::const_iterator i =
        gGlobalMultiGenerationOutputKindsMap.begin ();
      i != gGlobalMultiGenerationOutputKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
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
EXP mfMultiGenerationOutputKind fetchGeneratedOutputKindFromRunData ()
{
  mfMultiGenerationOutputKind
    result =
      mfMultiGenerationOutputKind::k_NoGeneration;
//
//   // analyze the options
//   const vector<oahOption>&
//     optionsVector =
//       gGlobalServiceRunData->
//         getOptionsAndArguments ().getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//       string optionName =
//         optionsVector [i].getOptionName ();
//       string optionValue =
//         optionsVector [i].getOptionValue ();
//
//       string optionNameWithoutDash = optionName.substr (1);
//
// //     gLogStream << JMI
// //       "optionName: " << optionName <<
// //       ", optionValue: " << optionValue <<
// //       ", optionNameWithoutDash: " << optionNameWithoutDash <<
// //       endl;
//
//       // is optionNameWithoutDash one of the multi generators output kinds?
//       mfMultiGenerationOutputKind
//         newMultiGenerationOutputKind =
//           mfMultiGenerationOutputKindFromString (
//             optionNameWithoutDash);
//
//       if (newMultiGenerationOutputKind != mfMultiGenerationOutputKind::k_NoGeneration) {
//         // yes, optionNameWithoutDash is a multi generators output kind
//         if (result != mfMultiGenerationOutputKind::k_NoGeneration) {
//           stringstream s;
//
//           s <<
//             "mfMultiGenerationOutputKindAtom generated output kind specified more that once" <<
//             endl;
//
//           oahError (s.str ());
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
  gLogStream <<  // JMIJMIJMI
    "result: " <<
    mfMultiGenerationOutputKindAsString (result) <<
    endl;

  return result;
}

//______________________________________________________________________________
S_mfMultiGenerationOutputKindAtom mfMultiGenerationOutputKindAtom::create (
  const string&   longName,
  const string&   shortName,
  const string&   description,
  const string&   variableName,
  mfMultiGenerationOutputKind&
                  multiGenerationOutputKindVariable,
  mfMultiGenerationOutputKind
                  multiGenerationOutputKindValue)
{
  mfMultiGenerationOutputKindAtom* o = new
    mfMultiGenerationOutputKindAtom (
      longName,
      shortName,
      description,
      variableName,
      multiGenerationOutputKindVariable,
      multiGenerationOutputKindValue);
  assert (o != nullptr);
  return o;
}

mfMultiGenerationOutputKindAtom::mfMultiGenerationOutputKindAtom (
  const string&   longName,
  const string&   shortName,
  const string&   description,
  const string&   variableName,
  mfMultiGenerationOutputKind&
                  multiGenerationOutputKindVariable,
  mfMultiGenerationOutputKind
                  multiGenerationOutputKindValue)
  : oahAtomImplicitlyStoringAValue (
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
    mfMultiGenerationOutputKind::k_NoGeneration;
}

mfMultiGenerationOutputKindAtom::~mfMultiGenerationOutputKindAtom ()
{}

void mfMultiGenerationOutputKindAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" <<
      fetchNames () <<
      "; which is of type 'mfMultiGenerationOutputKindAtom'" <<
      endl;
  }
#endif

  if (fSetByAnOption) {
    stringstream s;

    s <<
      "mfMultiGenerationOutputKindAtom generated output kind" << // JMI ???
      " set more than once" <<
      endl;

    oahError (s.str ());
  }
  else {
    fMultiGenerationOutputKindVariable = fMultiGenerationOutputKindValue;
    fSetByAnOption = true;
  }
}

void mfMultiGenerationOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mfMultiGenerationOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_mfMultiGenerationOutputKindAtom>*> (v)) {
        S_mfMultiGenerationOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfMultiGenerationOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfMultiGenerationOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mfMultiGenerationOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_mfMultiGenerationOutputKindAtom>*> (v)) {
        S_mfMultiGenerationOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfMultiGenerationOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfMultiGenerationOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string mfMultiGenerationOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable);

  return s.str ();
}

string mfMultiGenerationOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable);

  return s.str ();
}

void mfMultiGenerationOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiGenerationOutputKindAtom:" <<
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
    "fMultiGenerationOutputKindVariable" << " : " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable) <<
    endl <<
    setw (fieldWidth) <<
    "fMultiGenerationOutputKindValue" << " : " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindValue) <<
    endl;

  --gIndenter;
}

void mfMultiGenerationOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_mfMultiGenerationOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_multiGenerationOahGroup gGlobalMultiGenerationOahGroup;

S_multiGenerationOahGroup mfMultiGenerationOahGroup::create ()
{
  mfMultiGenerationOahGroup* o = new mfMultiGenerationOahGroup ();
  assert (o != nullptr);

  return o;
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
  fMultiGenerationOutputKindAtom = mfMultiGenerationOutputKind::k_NoGeneration;
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGenerationOahGroup>*> (v)) {
        S_multiGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfMultiGenerationOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfMultiGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_multiGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_multiGenerationOahGroup>*> (v)) {
        S_multiGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfMultiGenerationOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfMultiGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfMultiGenerationOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mfMultiGenerationOahGroup::printMultiGenerationOahValues (int fieldWidth)
{
  gLogStream <<
    "The MultiGeneration options are:" <<
    endl;

  ++gIndenter;

  // Output kind
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Multigenerator output kind:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (fieldWidth) << "fMultiGenerationOutputKind" << " : " <<
    mfMultiGenerationOutputKindAsString (
      fMultiGenerationOutputKind) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_multiGenerationOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_multiGenerationOahGroup createGlobalMultiGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating multi generator OAH group" <<
      endl;
  }
#endif

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
