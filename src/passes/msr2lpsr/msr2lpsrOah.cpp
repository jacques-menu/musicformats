/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

#include "mfConstants.h"
#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahWae.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2lpsrScoreOutputKindAtom msr2lpsrScoreOutputKindAtom::create (
  const std::string&   longName,
  const std::string&   shortName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
{
  msr2lpsrScoreOutputKindAtom* obj = new
    msr2lpsrScoreOutputKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrScoreOutputKindVariable);
  assert (obj != nullptr);
  return obj;
}

msr2lpsrScoreOutputKindAtom::msr2lpsrScoreOutputKindAtom (
  const std::string&   longName,
  const std::string&   shortName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lpsrScoreOutputKindVariable)
{}

msr2lpsrScoreOutputKindAtom::~msr2lpsrScoreOutputKindAtom ()
{}

void msr2lpsrScoreOutputKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msr2lpsrScoreOutputKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msr2lpsrScoreOutputKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, lpsrScoreOutputKind>::const_iterator
    it =
      gGlobalLpsrScoreOutputKindsMap.find (
        theString);

  if (it == gGlobalLpsrScoreOutputKindsMap.end ()) {
    // no, score output kind is unknown in the map

    std::stringstream ss;

    ss <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalLpsrScoreOutputKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableLpsrScoreOutputKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLpsrScoreOutputKindVariable (
    (*it).second);

  fSelected = true;
}

void msr2lpsrScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2lpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2lpsrScoreOutputKindAtom>*> (v)) {
        S_msr2lpsrScoreOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2lpsrScoreOutputKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2lpsrScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2lpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2lpsrScoreOutputKindAtom>*> (v)) {
        S_msr2lpsrScoreOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2lpsrScoreOutputKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2lpsrScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msr2lpsrScoreOutputKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return ss.str ();
}

std::string msr2lpsrScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return ss.str ();
}

void msr2lpsrScoreOutputKindAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2lpsrScoreOutputKindAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "variableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLpsrScoreOutputKindVariable" << ": \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void msr2lpsrScoreOutputKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable);
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

std::ostream& operator << (std::ostream& os, const S_msr2lpsrScoreOutputKindAtom& elt)
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

S_msr2lpsrOahGroup gGlobalMsr2lpsrOahGroup;

S_msr2lpsrOahGroup msr2lpsrOahGroup::create ()
{
  msr2lpsrOahGroup* obj = new msr2lpsrOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msr2lpsrOahGroup::msr2lpsrOahGroup ()
  : oahGroup (
      "msr2lpsr",
      "help-msr-to-lpsr", "hmsr2lpsr",
R"(These options control the way MSR data is translated to LPSR.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2lpsrOahGroup ();
}

msr2lpsrOahGroup::~msr2lpsrOahGroup ()
{}

#ifdef MF_TRACE_IS_ENABLED
void msr2lpsrOahGroup::initializMsr2lpsrTraceOptions ()
{}
#endif // MF_TRACE_IS_ENABLED

void msr2lpsrOahGroup::initializMsr2lpsrScoreOutputOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LilyPond score output",
        "help-lilypond-score-output", "hlpso",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // LPSR score output kind
  // --------------------------------------

  const lpsrScoreOutputKind
    lpsrScoreOutputKindDefaultValue =
      lpsrScoreOutputKind::kScoreOutputScoreOnly; // default value

  fScoreOutputKind = lpsrScoreOutputKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msr2lpsrScoreOutputKindAtom::create (
        "msr2lpsr-score-output-kind", "m2lpsok",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use OUTPUT_KIND to create the LPSR blocks,
as well as in the generated LilyPond code.
The NUMBER LilyPond score output kinds available are:
OUTPUT_KINDS.
'-one-file' means that LilyPond will produce a single file
containing all the scores and/or parts.
Otherwise, one file will be generated for each score and/or part.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (gGlobalLpsrScoreOutputKindsMap.size ())),
            std::regex ("OUTPUT_KINDS"),
            availableLpsrScoreOutputKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          lpsrScoreOutputKindAsString (
            lpsrScoreOutputKindDefaultValue)),
        "OUTPUT_KIND",
        "fScoreOutputKind",
        fScoreOutputKind));
}

void msr2lpsrOahGroup::initializMsr2lpsrRepeatsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "help-lpsr-repeats", "hlpsrr",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // bar lines
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-implicit-initial-repeat-barline-in-lpsr", "ciirbil",
R"(Create an implicit repeat barLine at the beginning of the staff
in the LPSR in case there is none, as is usual in scores.
By default, no such barLine is added.)",
        "createImplicitInitialRepeatBarLine",
        fCreateImplicitInitialRepeatBarLine));
}

void msr2lpsrOahGroup::initializMsr2lpsrNotesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "help-msr-to-lpsr-notes", "hmsr2lpsrn",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2lpsrOahGroup::initializMsr2lpsrLyricsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Lyrics",
      "help-lpsr-lyrics", "hlpsrlyrd",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2lpsrOahGroup::initializeMsr2lpsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2lpsrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

  // score output kind
  // --------------------------------------
  initializMsr2lpsrScoreOutputOptions ();

  // repeats
  // --------------------------------------
  initializMsr2lpsrRepeatsOptions ();

  // notes
  // --------------------------------------
  initializMsr2lpsrNotesOptions ();

  // lyrics
  // --------------------------------------
  initializMsr2lpsrLyricsOptions ();
}

//______________________________________________________________________________
void msr2lpsrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2lpsrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2lpsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lpsrOahGroup>*> (v)) {
        S_msr2lpsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2lpsrOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2lpsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lpsrOahGroup>*> (v)) {
        S_msr2lpsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2lpsrOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2lpsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2lpsrOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msr2lpsrOahGroup::displayMsr2lpsrOahValues (int valueFieldWidth)
{
  gLog <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

  // LilyPond output kind
  // --------------------------------------

  gLog <<
    "LilyPond output kind:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "scoreOutputKind" << ": " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    std::endl;

  --gIndenter;

  // repeats
  // --------------------------------------

  gLog <<
    "Repeats:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fCreateImplicitInitialRepeatBarLine" << ": " <<
    fCreateImplicitInitialRepeatBarLine <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2lpsrOahGroup& elt)
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
S_msr2lpsrOahGroup createGlobalMsr2lpsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2lpsr OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsr2lpsrOahGroup) {
    // create the global options group
    gGlobalMsr2lpsrOahGroup =
      msr2lpsrOahGroup::create ();
    assert (gGlobalMsr2lpsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2lpsrOahGroup;
}


}
