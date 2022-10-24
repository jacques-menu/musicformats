/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msr2lpsrScoreOutputKindAtom msr2lpsrScoreOutputKindAtom::create (
  const string&        longName,
  const string&        shortName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
{
  msr2lpsrScoreOutputKindAtom* o = new
    msr2lpsrScoreOutputKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrScoreOutputKindVariable);
  assert (o != nullptr);
  return o;
}

msr2lpsrScoreOutputKindAtom::msr2lpsrScoreOutputKindAtom (
  const string&        longName,
  const string&        shortName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msr2lpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msr2lpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrScoreOutputKind>::const_iterator
    it =
      gGlobalLpsrScoreOutputKindsMap.find (
        theString);

  if (it == gGlobalLpsrScoreOutputKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrScoreOutputKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrScoreOutputKindVariable (
    (*it).second);
}

void msr2lpsrScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2lpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2lpsrScoreOutputKindAtom>*> (v)) {
        S_msr2lpsrScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lpsrScoreOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2lpsrScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2lpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2lpsrScoreOutputKindAtom>*> (v)) {
        S_msr2lpsrScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lpsrScoreOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2lpsrScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrScoreOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string msr2lpsrScoreOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

string msr2lpsrScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

void msr2lpsrScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2lpsrScoreOutputKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void msr2lpsrScoreOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_msr2lpsrScoreOutputKindAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________

S_msr2lpsrOahGroup gGlobalMsr2lpsrOahGroup;

S_msr2lpsrOahGroup msr2lpsrOahGroup::create ()
{
  msr2lpsrOahGroup* o = new msr2lpsrOahGroup ();
  assert (o != nullptr);
  return o;
}

msr2lpsrOahGroup::msr2lpsrOahGroup ()
  : oahGroup (
    "Msr2lpsr",
    "help-msr-to-lpsr", "hmsr2lpsr",
R"(These options control the way MSR data is translated to LPSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2lpsrOahGroup ();
}

msr2lpsrOahGroup::~msr2lpsrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msr2lpsrOahGroup::initializMsr2lpsrTraceOptions ()
{}
#endif

void msr2lpsrOahGroup::initializeScoreOutputOptions ()
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
              regex ("NUMBER"),
              to_string (gGlobalLpsrScoreOutputKindsMap.size ())),
            regex ("OUTPUT_KINDS"),
            existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrScoreOutputKindAsString (
            lpsrScoreOutputKindDefaultValue)),
        "OUTPUT_KIND",
        "fScoreOutputKind",
        fScoreOutputKind));
}

void msr2lpsrOahGroup::initializeRepeatsOptions ()
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
R"(Create an implicit repeat barLine at the beginning of the stave
in the LPSR in case there is none, as is usual in scores.
By default, no such barLine is added.)",
        "createImplicitInitialRepeatBarLine",
        fCreateImplicitInitialRepeatBarLine));
}

void msr2lpsrOahGroup::initializeNotesOptions ()
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

void msr2lpsrOahGroup::initializeLyricsOptions ()
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
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2lpsrTraceOptions ();
#endif

  // score output kind
  // --------------------------------------
  initializeScoreOutputOptions ();

  // repeats
  // --------------------------------------
  initializeRepeatsOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // lyrics
  // --------------------------------------
  initializeLyricsOptions ();
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lpsrOahGroup>*> (v)) {
        S_msr2lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lpsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2lpsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lpsrOahGroup>*> (v)) {
        S_msr2lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lpsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2lpsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lpsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2lpsrOahGroup::printMsr2lpsrOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  // LilyPond output kind
  // --------------------------------------

  gLogStream <<
    "LilyPond output kind:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "scoreOutputKind" << " : " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    endl;

  --gIndenter;

  // repeats
  // --------------------------------------

  gLogStream <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fCreateImplicitInitialRepeatBarLine" << " : " <<
    fCreateImplicitInitialRepeatBarLine <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msr2lpsrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msr2lpsrOahGroup createGlobalMsr2lpsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2lpsr OAH group" <<
      endl;
  }
#endif

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
