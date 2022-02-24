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
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrIgnorePartAtom msrIgnorePartAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
{
  msrIgnorePartAtom* o = new
    msrIgnorePartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (o != nullptr);
  return o;
}

msrIgnorePartAtom::msrIgnorePartAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrIgnorePartAtom::~msrIgnorePartAtom ()
{}

void msrIgnorePartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrIgnorePartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be ignored

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      " in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrIgnorePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrIgnorePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrIgnorePartAtom>*
    p =
      dynamic_cast<visitor<S_msrIgnorePartAtom>*> (v)) {
        S_msrIgnorePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrIgnorePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrIgnorePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrIgnorePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrIgnorePartAtom>*
    p =
      dynamic_cast<visitor<S_msrIgnorePartAtom>*> (v)) {
        S_msrIgnorePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrIgnorePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrIgnorePartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrIgnorePartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrIgnorePartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrIgnorePartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrIgnorePartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrIgnorePartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << endl;
}

void msrIgnorePartAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", set by user";

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_msrIgnorePartAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrKeepPartAtom msrKeepPartAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
{
  msrKeepPartAtom* o = new
    msrKeepPartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (o != nullptr);
  return o;
}

msrKeepPartAtom::msrKeepPartAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  stringSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrKeepPartAtom::~msrKeepPartAtom ()
{}

void msrKeepPartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrKeepPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be kept

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      " in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrKeepPartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrKeepPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrKeepPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrKeepPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrKeepPartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrKeepPartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrKeepPartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrKeepPartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrKeepPartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrKeepPartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "empty";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << endl;
}

void msrKeepPartAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", set by user";

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

S_msr2msrOahGroup msr2msrOahGroup::create ()
{
  msr2msrOahGroup* o = new msr2msrOahGroup ();
  assert (o != nullptr);
  return o;
}

msr2msrOahGroup::msr2msrOahGroup ()
  : oahGroup (
    "Msr2lpsr",
    "help-msr2msr-to-msr", "hmsr2msr",
R"(These options control the way MSR data is translated to another MSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2msrOahGroup ();
}

msr2msrOahGroup::~msr2msrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msr2msrOahGroup::initializMsr2msrTraceOptions ()
{
  // JMI
}
#endif

void msr2msrOahGroup::initializeBookOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Book",
      "help-msr2msr-book", "hmsrb",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // book
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "avoid-msr2msr", "am2m",
R"(Avoid the msr2msr pass, for TESTS.)",
        "fAvoidMsr2msr",
        fAvoidMsr2msr));

  // book
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "expand-to-harmony-band", "ethb",
R"(Expand the book to as many scores as needed for a harmony band.)",
        "fExpandToHarmonyBandBook",
        fExpandToHarmonyBandBook));
}

void msr2msrOahGroup::initializeBreakOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Breaks",
      "help-msr2msr-breaks", "hmb",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // page breaks
  // --------------------------------------

  fInserPageBreakAfterMeasureAtom =
    oahStringSetElementAtom::create (
      "insert-page-break-after-measure", "ipbam",
R"(Insert a page break after measure MEASURE_NUMBER
when creating a new MSR from an existing one.
There can be several occurrences of this option.)",
      "MEASURE_NUMBER",
      "fInserPageBreakAfterMeasureSet",
      fInserPageBreakAfterMeasureSet);

  subGroup->
    appendAtomToSubGroup (
      fInserPageBreakAfterMeasureAtom);

  // line breaks
  // --------------------------------------

  fInserLineBreakAfterMeasureAtom =
    oahStringSetElementAtom::create (
      "insert-line-break-after-measure", "ilbam",
R"(Insert a line break after measure MEASURE_NUMBER
when creating a new MSR from an existing one.
There can be several occurrences of this option.)",
      "MEASURE_NUMBER",
      "fInserLineBreakAfterMeasureSet",
      fInserLineBreakAfterMeasureSet);

  subGroup->
    appendAtomToSubGroup (
      fInserLineBreakAfterMeasureAtom);
}

void msr2msrOahGroup::initializeCompressOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Compress",
      "help-msr2msr-compress", "hmsrc",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // compress full measure rests
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "coalesce-empty-measures", "cem",
R"(Coalesce-empty-measures empty measures instead of creating successive empty measures
in the MSR score JMI ???.)",
        "fCoalesceEmptyMeasures",
        fCoalesceEmptyMeasures));

  // merge rests
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "merge-rests-in-msr", "mrim",
R"(Merge rests in the MSR to obtain better looking scores.
This works only when there are 2 voices per staves.
Limitations: this only handles two voices
and does not handle multi-measure/whole-measure rests.)",
        "fMergeRests",
        fMergeRests));
}

void msr2msrOahGroup::initializeRepeatsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "help-msr2msr-repeats", "hmsrr",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // barLines
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-implicit-initial-repeat-barline-in-msr", "ciirbim",
R"(Create an implicit repeat barLine at the beginning of the stave
in the MSR in case there is none, as is usual in scores.
By default, no such barLine is added.)",
        "fCreateImplicitInitialRepeatBarLine",
        fCreateImplicitInitialRepeatBarLine));
}

void msr2msrOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "help-msr2msr-to-msr-notes", "hmsr2msrn",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2msrOahGroup::initializeLyricsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Lyrics",
      "help-msr2msr-lyrics", "hmsrlyrd",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);
}

void msr2msrOahGroup::initializeMsr2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2msrTraceOptions ();
#endif

  // book
  // --------------------------------------
  initializeBookOptions ();

  // breaks
  initializeBreakOptions ();

  // compress
  // --------------------------------------
  initializeCompressOptions ();

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
void msr2msrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2msrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2msrOahGroup::printMsr2msrOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  // avoid msr2msr? (for TESTS)
  // --------------------------------------

  gLogStream <<
    "avoid msr2msr? (for TESTS):" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fAvoidMsr2msr" << " : " <<
    fAvoidMsr2msr <<
    endl;

  --gIndenter;


  // book
  // --------------------------------------

  gLogStream <<
    "Book:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fExpandToHarmonyBandBook" << " : " <<
    fExpandToHarmonyBandBook <<
    endl;

  --gIndenter;


  // compress
  // --------------------------------------

  gLogStream <<
    "Compress" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fCoalesceEmptyMeasures" << " : " <<
    fCoalesceEmptyMeasures <<
    endl <<

    setw (valueFieldWidth) <<
    "fMergeRests" << " : " <<
    fMergeRests <<
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

ostream& operator<< (ostream& os, const S_msr2msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2msrOahGroup createGlobalMsr2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2msr OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2msrOahGroup) {
    // create the global options group
    gGlobalMsr2msrOahGroup =
      msr2msrOahGroup::create ();
    assert (gGlobalMsr2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2msrOahGroup;
}


}
