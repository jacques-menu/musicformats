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
#include "mxsr2msrOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrReplaceClefAtom msrReplaceClefAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  map<msrClefKind, msrClefKind>&
                    clefKindToClefKindMapVariable)
{
  msrReplaceClefAtom* o = new
    msrReplaceClefAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      clefKindToClefKindMapVariable);
  assert (o != nullptr);
  return o;
}

msrReplaceClefAtom::msrReplaceClefAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  map<msrClefKind, msrClefKind>&
                    clefKindToClefKindMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fClefKindToClefKindMapVariable (
      clefKindToClefKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrReplaceClefAtom::~msrReplaceClefAtom ()
{}

void msrReplaceClefAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrReplaceClefAtom'" <<
      endl;
  }
#endif

  // theString contains the replace clef specification
  // decipher it to extract the old and new clef names

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  string regularExpression (
    "(.*)" // originalClefName
    "="
    "(.*)" // destinationClefName
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-marTransposePart argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    originalClefName    = sm [1],
    destinationClefName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> originalClefName = \"" << originalClefName << "\", " <<
      "--> destinationClefName = \"" << destinationClefName << "\"" <<
      endl;
  }
#endif

  // is originalClefName in the replace clef map?
  msrClefKind
    originalClefKind =
      clefKindFromString (
        K_NO_INPUT_LINE_NUMBER,
        originalClefName);

  map<msrClefKind, msrClefKind>::iterator
    it =
      fClefKindToClefKindMapVariable.find (originalClefKind);

  if (it != fClefKindToClefKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Replace clef value \"" << theString << "\" occurs more that once" <<
      "in  a " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    msrClefKind
      destinationClefKind =
        clefKindFromString (
          K_NO_INPUT_LINE_NUMBER,
          destinationClefName);

    fClefKindToClefKindMapVariable [originalClefKind] =
      destinationClefKind;
  }
}

void msrReplaceClefAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrReplaceClefAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrReplaceClefAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrReplaceClefAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrReplaceClefAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrReplaceClefAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrReplaceClefAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fClefKindToClefKindMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        msrClefKindAsString ((*i).first) <<
        "=" <<
        msrClefKindAsString ((*i).second);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrReplaceClefAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fClefKindToClefKindMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        msrClefKindAsString ((*i).first) <<
        "=" <<
        msrClefKindAsString ((*i).second);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrReplaceClefAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrReplaceClefAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fClefKindToClefKindMapVariable" << " : " <<
    endl;

  if (! fClefKindToClefKindMapVariable.size ()) {
    os << "empty";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << msrClefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << msrClefKindAsString ((*i).second) << "\"";
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void msrReplaceClefAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fClefKindToClefKindMapVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << msrClefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << msrClefKindAsString ((*i).second) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_mxsr2msrOahGroup gGlobalMxsr2msrOahGroup;

S_mxsr2msrOahGroup mxsr2msrOahGroup::create (
  S_oahHandler handler)
{
  mxsr2msrOahGroup* o = new mxsr2msrOahGroup (
    handler);
  assert (o != nullptr);
  return o;
}

mxsr2msrOahGroup::mxsr2msrOahGroup (
  S_oahHandler handler)
  : oahGroup (
    "mxsr2msr",
    "help-mxsr-to-msr", "hmxsr2msr",
R"(These options control the way MXSRs are translated to MSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  createTheMxsr2msrPrefixes (handler);

  initializeMxsr2msrOahGroup ();
}

mxsr2msrOahGroup::~mxsr2msrOahGroup ()
{}

void mxsr2msrOahGroup::createTheMxsr2msrPrefixes (
                            S_oahHandler handler)
{
  // the 'ignore-redundant' prefixes
  // --------------------------------------

  fShortIgnoreRedundantPrefix =
    oahPrefix::create (
      "ir", "ir",
      "'-ir=abc,yz' is equivalent to '-irabc, -iryz'");
  handler->
    registerPrefixInHandler (
      fShortIgnoreRedundantPrefix);

  fLongIgnoreRedundantPrefix =
    oahPrefix::create (
      "ignore-redundant", "ignore-redundant-",
      "'-ignore-redundant=abc,yz' is equivalent to '-ignore-redundant-abc, -ignore-redundant-yz'");
  handler->
    registerPrefixInHandler (
      fLongIgnoreRedundantPrefix);

  // the 'delay-rests' prefixes
  // --------------------------------------

  fShortDelayRestsPrefix =
    oahPrefix::create (
      "dr", "dr",
      "'-dr=abc,yz' is equivalent to '-drabc, -dryz'");
  handler->
    registerPrefixInHandler (
      fShortDelayRestsPrefix);

  fLongDelayRestsPrefix =
    oahPrefix::create (
      "delay-rests", "delay-rests-",
      "'-delay-rests=abc,yz' is equivalent to '-delay-rests-abc, -delay-rests-yz'");
  handler->
    registerPrefixInHandler (
      fLongDelayRestsPrefix);
}

void mxsr2msrOahGroup::initializeMxsr2msrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxsr2msrTraceOptions ();
#endif

  // parts
  // --------------------------------------
  initializePartsOptions ();

  // clefs, keys, time signatures
  // --------------------------------------
  initializeClefsKeysTimesOptions ();

  // page breaks
  // --------------------------------------
  initializePageBreaksOptions ();

  // measures
  // --------------------------------------
  initializeMeasuresOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // dynamics and wedges
  // --------------------------------------
  initializeDynamicsAndWedgesOptions ();

  // words
  // --------------------------------------
  initializeWordsOptions ();

  // combined options
  // --------------------------------------
  initializeCombinedOptionsOptions ();
}

#ifdef TRACING_IS_ENABLED
void mxsr2msrOahGroup::initializeMxsr2msrTraceOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "mxsr2msr Trace",
        "help-mxsr-to-oah-trace", "hmxsr2ot",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);
}
#endif

void mxsr2msrOahGroup::initializePartsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "help-msr-to-lpsr-parts", "hm2p",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML part ID
  // --------------------------------------

  fIgnorePartIDAtom =
    oahStringSetElementAtom::create (
      "ignore-musicxml-part-id", "mompi",
R"(Ignore the part with ID PART_ID, which is a string.
There can be several occurrences of this option.
All the parts not ignored are kept.
This option is incompatible with '-mkpi, -msr-keep-musicxml-part-id'.)",
      "PART_ID",
      "fPartsIgnoreIDSet",
      fPartsIgnoreIDSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnorePartIDAtom);

  // ignore MusicXML part name
  // --------------------------------------

  fIgnorePartNameAtom =
    oahStringSetElementAtom::create (
      "ignore-musicxml-part-name", "mompn",
R"(Ignore the part named PART_NAME, which is a string.
There can be several occurrences of this option.
All the parts not ignored are kept.
This option is incompatible with '-mkpn, -msr-keep-musicxml-part-name'.)",
      "PART_NAME",
      "fPartsIgnoreNameSet",
      fPartsIgnoreNameSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnorePartNameAtom);

  // keep MusicXML part ID
  // --------------------------------------

  fKeepPartIDAtom =
    oahStringSetElementAtom::create (
      "keep-musicxml-part-id", "mkmpi",
R"(Keep the part with ID PART_ID, which is a string.
There can be several occurrences of this option.
All the parts not kept are ignored.
This option is incompatible with '-mopi, -msr-ignore-musicxml-part-id'.)",
      "PART_ID",
      "fPartsKeepIDSet",
      fPartsKeepIDSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartIDAtom);

  // keep MusicXML part name
  // --------------------------------------

  fKeepPartNameAtom =
    oahStringSetElementAtom::create (
      "keep-musicxml-part-name", "mkmpn",
R"(Keep the part named PART_NAME, which is a string.
There can be several occurrences of this option.
All the parts not kept are ignored.
This option is incompatible with '-mopn, -msr-ignore-musicxml-part-name'.)",
      "PART_NAME",
      "fPartsKeepNameSet",
      fPartsKeepNameSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartNameAtom);
}

void mxsr2msrOahGroup::initializeClefsKeysTimesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "clefs, keys, time signatures",
        "help-musicxml-clefs-keys-times", "hmxmlckt",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // replace clef
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      msrReplaceClefAtom::create (
        "msr-replace-clef", "mrc",
        regex_replace (
          regex_replace (
R"(Raplace clef ORIGINAL_CLEF by NEW_CLEF.
REPLACE_CLEF_SPEC can be:
'ORIGINAL_CLEF = NEW_CLEF'
or
"ORIGINAL_CLEF = NEW_CLEF"
The single or double quotes are used to allow spaces in the clef names
and around the '=' sign, otherwise they can be dispensed with.
The NUMBER clefs available are:
CLEFS_NAMES.
There can be several occurrences of this option.)",
            regex ("NUMBER"),
            to_string (gGlobalClefKindsMap.size ())),
          regex ("CLEFS_NAMES"),
          gIndenter.indentMultiLineString (
            existingClefKindsNames (K_NAMES_LIST_MAX_LENGTH))),
        "REPLACE_CLEF_SPEC",
        "fReplaceClefKindToClefKindMapVariable",
        fReplaceClefKindToClefKindMapVariable));

  // the 'ignore redundant' multiplex booleans atom
  // --------------------------------------

  fIgnoreRedundantMultiplexBooleansAtom =
    oahCommonPrefixBooleansAtom::create (
      "ignore-same-elements", "ism",
      "Ignore ELEMENTS that are the same as the current one.",
      "ELEMENTS",
      "ELEMENTS",
      fShortIgnoreRedundantPrefix,
      fLongIgnoreRedundantPrefix);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantMultiplexBooleansAtom);

  // redundant clefs
  // --------------------------------------

  fIgnoreRedundantClefsAtom =
    oahBooleanAtom::create (
      "ignore-redundant-clefs", "irclefs",
R"(Ignore clefs that are the same as the current one.)",
      "fIgnoreRedundantClefs",
      fIgnoreRedundantClefs);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantClefsAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  // redundant keys
  // --------------------------------------

  fIgnoreRedundantKeysAtom =
    oahBooleanAtom::create (
      "ignore-redundant-keys", "irkeys",
R"(Ignore keys that are the same as the current one.)",
      "fIgnoreRedundantKeys",
      fIgnoreRedundantKeys);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantKeysAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  // redundant times
  // --------------------------------------

  fIgnoreRedundantTimesAtom =
    oahBooleanAtom::create (
      "ignore-redundant-times", "irtimes",
R"(Ignore times that are the same as the current one.)",
      "fIgnoreRedundantTimes",
      fIgnoreRedundantTimes);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantTimesAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void mxsr2msrOahGroup::initializePageBreaksOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page breaks",
        "help-musicxml-page-breaks", "hmxmlpb",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML page breaks
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-musicxml-page-breaks", "imxmlpb",
R"(Ignore page breaks in MusicXML data.)",
        "fIgnorePageBreaksInMusicXML",
        fIgnorePageBreaksInMusicXML));
}

void mxsr2msrOahGroup::initializeMeasuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "help-musicxml-measures", "hmxmlm",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // replicate MSR measure
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapElementAtom::create (
        "replicate-msr-measure", "rmmeas",
R"###(Replicate an MSR mesure according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER:NUMBER_OF_MEASURES_TO_ADD',
where MEASURE_NUMBER is a string, and NUMBER_OF_MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and NUMBER_OF_MEASURES_TO_ADD should be at least 1, , such as '17:3'.
There can be spaces around the ':'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score,
but for other needs too.
This option can be used any number of times.)###",
        "MEASURE_NUMBER",
        "fMeasuresToBeReplicatedStringToIntMap",
        fMeasuresToBeReplicatedStringToIntMap));

  // add empty MSR measures
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapElementAtom::create (
        "add-empty-msr-measures", "aemmeas",
R"###(Add empty MSR mesures according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER:NUMBER_OF_MEASURES_TO_ADD',
where MEASURE_NUMBER is a string, and NUMBER_OF_MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and NUMBER_OF_MEASURES_TO_ADD should be at least 1, , such as '17:3'.
There can be spaces around the ':'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "fAddEmptyMeasuresStringToIntMap",
        fAddEmptyMeasuresStringToIntMap));
}

void mxsr2msrOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "help-msr-notes", "hmsrn",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore articulations
  // --------------------------------------

  S_oahBooleanAtom
    ignoreArticulationsAtom =
      oahBooleanAtom::create (
        "ignore-articulations", "iarts",
R"(Ignore articulations in MusicXML data.)",
        "fIgnoreArticulations",
        fIgnoreArticulations);
  subGroup->
    appendAtomToSubGroup (
      ignoreArticulationsAtom);

  // ignore ornaments
  // --------------------------------------

  S_oahBooleanAtom
    ignoreOrnamentsAtom =
      oahBooleanAtom::create (
        "ignore-ornaments", "iorns",
R"(Ignore ornaments in MusicXML data.)",
        "fIgnoreOrnaments",
        fIgnoreOrnaments);
  subGroup->
    appendAtomToSubGroup (
      ignoreOrnamentsAtom);

  // ignore ties
  // --------------------------------------

  S_oahBooleanAtom
    ignoreTiesAtom =
      oahBooleanAtom::create (
        "ignore-ties", "ities",
R"(Ignore ties in MusicXML data)",
        "fIgnoreTies",
        fIgnoreTies);
  subGroup->
    appendAtomToSubGroup (
      ignoreTiesAtom);

  // ignore dynamics
  // --------------------------------------

  S_oahBooleanAtom
    ignoreDynamicsAtom =
      oahBooleanAtom::create (
        "ignore-dynamics", "idyns",
R"(Ignore dynamics in MusicXML data)",
        "fIgnoreDynamics",
        fIgnoreDynamics);
  subGroup->
    appendAtomToSubGroup (
      ignoreDynamicsAtom);

  // ignore slurs
  // --------------------------------------

  S_oahBooleanAtom
    ignoreSlursAtom =
      oahBooleanAtom::create (
        "ignore-slurs", "islurs",
R"(Ignore slurs in MusicXML data.)",
        "fIgnoreSlurs",
        fIgnoreSlurs);
  subGroup->
    appendAtomToSubGroup (
      ignoreSlursAtom);

  // ignore wedges
  // --------------------------------------

  S_oahBooleanAtom
    ignoreWedgesAtom =
      oahBooleanAtom::create (
        "ignore-wedges", "iwedges",
R"(Ignore '<wedge/>' in MusicXML data.)",
        "fIgnoreWedges",
        fIgnoreWedges);
  subGroup->
    appendAtomToSubGroup (
      ignoreWedgesAtom);

  // ignore lyrics
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-musicxml-lyrics", "imlyrics",
R"(Ignore lyrics in MusicXML data.)",
        "fIgnoreLyrics",
        fIgnoreLyrics));


  // ignore harmonies
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-harmonies", "iharms",
R"(Ignore harmonies in MusicXML data.)",
        "fIgnoreHarmonies",
        fIgnoreHarmonies));


  // ignore figured bass elements
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-figured-bass", "ifigbass",
R"(Ignore figured bass elements in MusicXML data.)",
        "fIgnoreFiguredBassElements",
        fIgnoreFiguredBassElements));

  // slash all grace notes
  // --------------------------------------

  S_oahBooleanAtom
    slashAllGraceNotesAtom =
      oahBooleanAtom::create (
        "slash-all-grace-notes", "slashagn",
R"(Add a slash to all grace notes)",
        "fSlashAllGraceNotes",
        fSlashAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slashAllGraceNotesAtom);

  // slur all grace notes
  // --------------------------------------

  S_oahBooleanAtom
    slurAllGraceNotesAtom =
      oahBooleanAtom::create (
        "slur-all-grace-notes", "sluragn",
R"(Add a slur to all grace notes)",
        "fSlurAllGraceNotes",
        fSlurAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slurAllGraceNotesAtom);

  // beam all grace notes
  // --------------------------------------

  S_oahBooleanAtom
    beamAllGraceNotesAtom =
      oahBooleanAtom::create (
        "beam-all-grace-notes", "beamagn",
R"(Add a beam to all grace notes)",
        "fBeamAllGraceNotes",
        fBeamAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      beamAllGraceNotesAtom);

  // the 'delay rests' multiplex booleans atom
  // --------------------------------------

  S_oahCommonPrefixBooleansAtom
    delayRestsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "delay-to-next-note", "dtnn",
        "Delay SHORT_NAME/LONG_NAME that occur on a rest until the next note.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortDelayRestsPrefix,
        fLongDelayRestsPrefix);

  subGroup->
    appendAtomToSubGroup (
      delayRestsMultiplexBooleansAtom);

  // delay rests dynamics
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsDynamicsAtom =
      oahBooleanAtom::create (
        "delay-rests-dynamics", "drdyns",
R"()",
        "fDelayRestsDynamics",
        fDelayRestsDynamics);
  subGroup->
    appendAtomToSubGroup (
      delayRestsDynamicsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsDynamicsAtom);

  // delay rests words
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsWordsAtom =
      oahBooleanAtom::create (
        "delay-rests-words", "drwords",
R"()",
        "fDelayRestsWords",
        fDelayRestsWords);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWordsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWordsAtom);

  // delay rests beams
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsBeamsAtom =
      oahBooleanAtom::create (
        "delay-rests-beams", "drbeams",
R"()",
        "fDelayRestsBeams",
        fDelayRestsBeams);
  subGroup->
    appendAtomToSubGroup (
      delayRestsBeamsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsBeamsAtom);

  // delay rests slurs
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsSlursAtom =
      oahBooleanAtom::create (
        "delay-rests-slurs", "drslurs",
R"()",
        "fDelayRestsSlurs",
        fDelayRestsSlurs);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlursAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsSlursAtom);

  // delay rests ligatures
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsLigaturesAtom =
      oahBooleanAtom::create (
        "delay-rests-ligatures", "drligs",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
        "fDelayRestsLigatures",
        fDelayRestsLigatures);
  subGroup->
    appendAtomToSubGroup (
      delayRestsLigaturesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsLigaturesAtom);

  // delay rests pedals
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsPedalsAtom =
      oahBooleanAtom::create (
        "delay-rests-pedals", "drpeds",
R"()",
        "fDelayRestsPedals",
        fDelayRestsPedals);
  subGroup->
    appendAtomToSubGroup (
      delayRestsPedalsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsPedalsAtom);

  // delay rests slashes
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsSlashesAtom =
      oahBooleanAtom::create (
        "delay-rests-slashes", "drslashes",
R"('<slash/>' in MusicXML)",
        "fDelayRestsSlashes",
        fDelayRestsSlashes);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlashesAtom);

  // delay rests wedges
  // --------------------------------------

  S_oahBooleanAtom
    delayRestsWedgesAtom =
      oahBooleanAtom::create (
        "delay-rests-wedges", "drwedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "fDelayRestsWedges",
        fDelayRestsWedges);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWedgesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWedgesAtom);
}

void mxsr2msrOahGroup::initializeDynamicsAndWedgesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics and wedges",
        "help-musicxml-dynamics-and-wedges", "hmxmldaw",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // dynamics
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "all-dynamics-below", "adb",
R"(Ignore dynamics placement and set it to 'below'.)",
        "fAllDynamicsBelow",
        fAllDynamicsBelow));

  // wedges
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "all-wedges-below", "awb",
R"(Ignore wedges placement and set it to 'below'.)",
        "fAllWedgesBelow",
        fAllWedgesBelow));
}

void mxsr2msrOahGroup::initializeWordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "help-musicxml-words", "hmxmlw",
R"(Ignore words in MusicXML data.)",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML words

  S_oahBooleanAtom
    ignoreMusicXMLWordsAtom =
      oahBooleanAtom::create (
        "ignore-musicxml-words", "imwords",
R"(Ignore '<words />' in MusicXML data.)",
        "fIgnoreMusicXMLWords",
        fIgnoreMusicXMLWords);
  subGroup->
    appendAtomToSubGroup (
      ignoreMusicXMLWordsAtom);

  // convert words to bold

  fBoldWordsAtom =
    oahStringSetElementAtom::create (
      "convert-musicxml-words-to-bold", "bold-words",
R"(Convert MusicXML words STRING to bold.
There can be several occurrences of this option.)",
      "STRING",
      "fBoldWordsSet",
      fBoldWordsSet);

  subGroup->
    appendAtomToSubGroup (
      fBoldWordsAtom);

  // convert words to italic

  fItalicWordsAtom =
    oahStringSetElementAtom::create (
      "convert-musicxml-words-to-italic", "italic-words",
R"(Convert MusicXML words STRING to italic.
There can be several occurrences of this option.)",
      "STRING",
      "fItalicWordsSet",
      fItalicWordsSet);

  subGroup->
    appendAtomToSubGroup (
      fItalicWordsAtom);

  // place words above

  fWordsToBePlacedAboveAtom =
    oahStringSetElementAtom::create (
      "place-musicxml-words-above", "words-above",
R"(Place MusicXML words STRING above the staff.
There can be several occurrences of this option.)",
      "STRING",
      "fWordsToBePlacedAboveSet",
      fWordsToBePlacedAboveSet);

  subGroup->
    appendAtomToSubGroup (
      fWordsToBePlacedAboveAtom);

  // place words below

  fWordsToBePlacedBelowAtom =
    oahStringSetElementAtom::create (
      "place-musicxml-words-below", "words-below",
R"(Place MusicXML words STRING below the staff.
There can be several occurrences of this option.)",
      "STRING",
      "fWordsToBePlacedBelowSet",
      fWordsToBePlacedBelowSet);

  subGroup->
    appendAtomToSubGroup (
      fWordsToBePlacedBelowAtom);

  // convert MusicXML words to MSR tempos

  fWordsToTempoAtom =
    oahStringSetElementAtom::create (
      "convert-musicxml-words-to-msr-tempo", "words-to-tempo",
R"(Convert MusicXML words STRING to an MSR tempo.
There can be several occurrences of this option.)",
      "STRING",
      "fWordsToTemposSet",
      fWordsToTemposSet);

  subGroup->
    appendAtomToSubGroup (
      fWordsToTempoAtom);

  // convert MusicXML words to MSR rehearsal marks

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-rehearsal-mark", "words-to-rehearsalMark",
  R"(Convert MusicXML words STRING to an MSR rehearsal mark.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToRehearsalMarkSet",
        fWordsToRehearsalMarkSet));

  // convert MusicXML words to MSR segnos

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-segno", "words-to-segno",
  R"(Convert MusicXML words STRING to an MSR segno.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToSegnoSet",
        fWordsToSegnoSet));

  // convert MusicXML words to MSR dal segnos

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-dal-segno", "words-to-dal-segno",
  R"(Convert MusicXML words STRING to an MSR dal segno.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToDalSegnoSet",
        fWordsToDalSegnoSet));

  // convert MusicXML words to MSR dal segno al fine

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-dal_segno-al-fine", "words-to-dal_segno-al-fine",
  R"(Convert MusicXML words STRING to an MSR dal segno al fine.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToDalSegnoAlFineSet",
        fWordsToDalSegnoAlFineSet));

  // convert MusicXML words to MSR dal segno al coda

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-dal_segno-al-coda", "words-to-dal_segno-al-coda",
  R"(Convert MusicXML words STRING to an MSR dal segno al coda.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToDalSegnoAlCodaSet",
        fWordsToDalSegnoAlCodaSet));

  // convert MusicXML words to MSR coda

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-coda-first", "words-to-coda-first",
  R"(Convert MusicXML words STRING to an MSR coda first.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToCodaFirstSet",
        fWordsToCodaFirstSet));

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-coda-second", "words-to-coda-second",
  R"(Convert MusicXML words STRING to an MSR coda second.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToCodaSecondSet",
        fWordsToCodaSecondSet));

  // convert MusicXML words to cresc

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-cresc", "words-to-cresc",
  R"(Convert MusicXML words STRING to an MSR cresc.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToCrescSet",
        fWordsToCrescSet));

  // convert MusicXML words to decresc

  subGroup->
    appendAtomToSubGroup (
      oahStringSetElementAtom::create (
        "convert-musicxml-words-to-msr-decresc", "words-to-decresc",
  R"(Convert MusicXML words STRING to an MSR decresc.
  There can be several occurrences of this option.)",
        "STRING",
        "fWordsToDecrescSet",
        fWordsToDecrescSet));

  // add words from the MusicXML lyrics

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "add-msr-words-from-the-msr-lyrics", "awftl",
R"(Add MSR words with the MSR lyrics contents, keeping the latter untouched.
This may come in handy when MusicXML data has been obtained from scanned images.)",
        "fAddMsrWordsFromTheMusicXMLLyrics",
        fAddMsrWordsFromTheMusicXMLLyrics));
}

void mxsr2msrOahGroup::initializeCombinedOptionsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "help-mxsr-combined-options", "hmxsrco",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // cubase
  // --------------------------------------

  // create the 'cubase' combined atom
  S_oahCombinedBooleansAtom
    cubaseCombinedBooleansAtom =
      oahCombinedBooleansAtom::create (
        "cubase", "",
  R"(Useful settings for MusicXML data exported from Cubase.)");

  subGroup->
    appendAtomToSubGroup (
      cubaseCombinedBooleansAtom);

  // set the '-cubase' option by default
  cubaseCombinedBooleansAtom->
    setCombinedBooleanVariables (fCubase);

  // populate the 'cubase' combined atom
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

//______________________________________________________________________________
void mxsr2msrOahGroup::enforceGroupQuietness ()
{
  // specific trace JMI
  // --------------------------------------
}

//______________________________________________________________________________
void mxsr2msrOahGroup::checkGroupOptionsConsistency ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Checking the consistency of mxsr2msrOahGroup group \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  // JMI and if mixed ID and name options are used?

  if (fPartsIgnoreIDSet.size () > 0 && fPartsKeepIDSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fIgnorePartIDAtom->fetchNames () <<
      "' and '" <<
      fKeepPartIDAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }

  if (fPartsIgnoreNameSet.size () > 0 && fPartsKeepNameSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fIgnorePartNameAtom->fetchNames () <<
      "' and '" <<
      fKeepPartNameAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }
}

//______________________________________________________________________________
Bool mxsr2msrOahGroup::wordsIsToBeConvertedToBold (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fBoldWordsSet.size ()) {
    set<string>::const_iterator
      it =
        fBoldWordsSet.find (wordsValue);

    if (it != fBoldWordsSet.end ()) {
      // register wordsValue as to be converted
// JMI      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToItalic (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fItalicWordsSet.size ()) {
    set<string>::const_iterator
      it =
        fItalicWordsSet.find (wordsValue);

    if (it != fItalicWordsSet.end ()) {
      // register wordsValue as to be converted
// JMI      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBePlacedAbove (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToBePlacedAboveSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToBePlacedAboveSet.find (wordsValue);

    if (it != fWordsToBePlacedAboveSet.end ()) {
      // register wordsValue as to be converted
// JMI      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBePlacedBelow (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToBePlacedBelowSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToBePlacedBelowSet.find (wordsValue);

    if (it != fWordsToBePlacedBelowSet.end ()) {
      // register wordsValue as to be converted
// JMI      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

//______________________________________________________________________________
Bool mxsr2msrOahGroup::wordsIsToBeConvertedToTempo (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToTemposSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToTemposSet.find (wordsValue);

    if (it != fWordsToTemposSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToRehearsalMark (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToRehearsalMarkSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToRehearsalMarkSet.find (wordsValue);

    if (it != fWordsToRehearsalMarkSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToSegno (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToSegnoSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToSegnoSet.find (wordsValue);

    if (it != fWordsToSegnoSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToDalSegno (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToDalSegnoSet.find (wordsValue);

    if (it != fWordsToDalSegnoSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToDalSegnoAlFine (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoAlFineSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToDalSegnoAlFineSet.find (wordsValue);

    if (it != fWordsToDalSegnoAlFineSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToDalSegnoAlCoda (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoAlCodaSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToDalSegnoAlCodaSet.find (wordsValue);

    if (it != fWordsToDalSegnoAlCodaSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToCodaFirst (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCodaFirstSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToCodaFirstSet.find (wordsValue);

    if (it != fWordsToCodaFirstSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToCodaSecond (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCodaSecondSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToCodaSecondSet.find (wordsValue);

    if (it != fWordsToCodaSecondSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToCresc (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCrescSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToCrescSet.find (wordsValue);

    if (it != fWordsToCrescSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

Bool mxsr2msrOahGroup::wordsIsToBeConvertedToDecresc (
  const string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDecrescSet.size ()) {
    set<string>::const_iterator
      it =
        fWordsToDecrescSet.find (wordsValue);

    if (it != fWordsToDecrescSet.end ()) {
      // register wordsValue as to be converted
      fWordsToBeConvertedSet.insert (wordsValue);

      result = true;
    }
  }

  return result;
}

//______________________________________________________________________________
void mxsr2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsr2msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsr2msrOahGroup>*> (v)) {
        S_mxsr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsr2msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxsr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsr2msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsr2msrOahGroup>*> (v)) {
        S_mxsr2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsr2msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxsr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsr2msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrOahGroup::printMxsr2msrValues (int valueFieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  // parts
  // --------------------------------------

  gLogStream <<
    "Parts:" <<
    endl;

  ++gIndenter;

  // parts ignored IDs

  gLogStream << left <<
    setw (valueFieldWidth) << "parts ignored IDs" << " : ";

  if (! fPartsIgnoreIDSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsIgnoreIDSet.begin ();
      i != fPartsIgnoreIDSet.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept IDs

  gLogStream << left <<
    setw (valueFieldWidth) << "parts kept IDs" << " : ";

  if (! fPartsKeepIDSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepIDSet.begin ();
      i != fPartsKeepIDSet.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts ignored names

  gLogStream << left <<
    setw (valueFieldWidth) << "parts ignored names" << " : ";

  if (! fPartsIgnoreNameSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsIgnoreNameSet.begin ();
      i != fPartsIgnoreNameSet.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept names

  gLogStream << left <<
    setw (valueFieldWidth) << "parts kept names" << " : ";

  if (! fPartsKeepNameSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepNameSet.begin ();
      i != fPartsKeepNameSet.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  gLogStream << endl;

  --gIndenter;

  // clefs, keys, time signatures
  // --------------------------------------

  gLogStream <<
    "clefs, keys, time signatures:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreRedundantClefs" << " : " <<
    fIgnoreRedundantClefs <<
    endl <<

    setw (valueFieldWidth) << "fIgnoreRedundantKeys" << " : " <<
    fIgnoreRedundantKeys <<
    endl <<

    setw (valueFieldWidth) << "fIgnoreRedundantTimes" << " : " <<
    fIgnoreRedundantTimes <<
    endl;

  --gIndenter;

  // page breaks
  // --------------------------------------

  gLogStream <<
    "Page breaks:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnorePageBreaksInMusicXML" << " : " <<
    fIgnorePageBreaksInMusicXML <<
    endl <<

  --gIndenter;

  // measures
  // --------------------------------------

  gLogStream <<
    "Measures:" <<
    endl;

  ++gIndenter;

//     map<string,int>       fAddEmptyMeasuresStringToIntMap;

/* JMI
  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreRedundantClefs" << " : " <<
    fIgnoreRedundantClefs <<
    endl <<

    setw (valueFieldWidth) << "fIgnoreRedundantKeys" << " : " <<
    fIgnoreRedundantKeys <<
    endl <<

    setw (valueFieldWidth) << "fIgnoreRedundantTimes" << " : " <<
    fIgnoreRedundantTimes <<
    endl;
*/
  --gIndenter;

  // notes
  // --------------------------------------

  gLogStream <<
     "Notes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fDelayRestsDynamics" << " : " <<
    fDelayRestsDynamics <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsWords" << " : " <<
    fDelayRestsWords <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsSlurs" << " : " <<
    fDelayRestsSlurs <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsLigatures" << " : " <<
    fDelayRestsLigatures <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsPedals" << " : " <<
    fDelayRestsPedals <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsSlashes" << " : " <<
    fDelayRestsSlashes <<
    endl <<

    setw (valueFieldWidth) << "fDelayRestsWedges" << " : " <<
    fDelayRestsWedges <<
    endl <<

    setw (valueFieldWidth) << "fSlashAllGraceNotes" << " : " <<
    fSlashAllGraceNotes <<
    endl <<
    setw (valueFieldWidth) << "fSlurAllGraceNotes" << " : " <<
    fSlurAllGraceNotes <<
    endl <<
    setw (valueFieldWidth) << "fBeamAllGraceNotes" << " : " <<
    fBeamAllGraceNotes <<
    endl;

  --gIndenter;

  // articulations
  // --------------------------------------

  gLogStream <<
     "Articulations:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreArticulations" << " : " <<
    fIgnoreArticulations <<
    endl;

  --gIndenter;

  // ornaments
  // --------------------------------------

  gLogStream <<
     "Ornaments:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreOrnaments" << " : " <<
    fIgnoreOrnaments <<
    endl;

  --gIndenter;

  // words
  // --------------------------------------

  gLogStream <<
     "Words:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreMusicXMLWords" << " : " <<
    fIgnoreMusicXMLWords <<
    endl <<

    setw (valueFieldWidth) <<
    "fAddMsrWordsFromTheMusicXMLLyrics" << " : " <<
    fAddMsrWordsFromTheMusicXMLLyrics <<
    endl;

  --gIndenter;

  // ties
  // --------------------------------------

  gLogStream <<
     "Ties:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreTies" << " : " <<
    fIgnoreTies <<
    endl;

  --gIndenter;

  // dynamics
  // --------------------------------------

  gLogStream <<
     "Dynamics:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreDynamics" << " : " <<
    fIgnoreDynamics <<
    endl;

  --gIndenter;

  // slurs
  // --------------------------------------

  gLogStream <<
     "Slurs:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreSlurs" << " : " <<
    fIgnoreSlurs <<
    endl;

  --gIndenter;

  // wedges
  // --------------------------------------

  gLogStream <<
     "Wedges:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreWedges" << " : " <<
    fIgnoreWedges <<
    endl;

  --gIndenter;

  // lyrics
  // --------------------------------------

  gLogStream <<
    "Lyrics:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (valueFieldWidth) << "fIgnoreLyrics" << " : " <<
    fIgnoreLyrics <<
    endl;

  --gIndenter;

  // harmonies
  // --------------------------------------

  gLogStream <<
    "Harmonies:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreHarmonies" << " : " <<
    fIgnoreHarmonies <<
    endl;

  --gIndenter;

  // figured bass
  // --------------------------------------

  gLogStream <<
    "Figured bass:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fIgnoreFiguredBassElements" << " : " <<
    fIgnoreFiguredBassElements <<
    endl;

  --gIndenter;

  // dynamics and wedges
  // --------------------------------------

  gLogStream <<
    "Dynamics and wedges:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fAllDynamicsBelow" << " : " <<
    fAllDynamicsBelow <<
    endl <<
    setw (valueFieldWidth) << "fAllWedgesBelow" << " : " <<
    fAllWedgesBelow <<
    endl;

  --gIndenter;

  // combined options, cubase
  // --------------------------------------

  gLogStream <<
    "Cubase:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fCubase" << " : " <<
    fCubase <<
    endl;

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  // specific trace
  // --------------------------------------

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mxsr2msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxsr2msrOahGroup createGlobalMxsr2msrOahGroup (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mxsr2msr OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMxsr2msrOahGroup) {
    // create the global options group
    gGlobalMxsr2msrOahGroup =
      mxsr2msrOahGroup::create (
        handler);
    assert (gGlobalMxsr2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMxsr2msrOahGroup;
}


}
