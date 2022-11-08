/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...
#include <sstream>

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string K_MSR_GENERATION_API_KIND_LONG_NAME  = "msr-generation-api-kind";
const string K_MSR_GENERATION_API_KIND_SHORT_NAME = "msr-gen-api";

S_msrPitchesLanguageAtom msrPitchesLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                     quarterTonesPitchesLanguageKind)
{
  msrPitchesLanguageAtom* o = new
    msrPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      quarterTonesPitchesLanguageKind);
  assert (o != nullptr);
  return o;
}

msrPitchesLanguageAtom::msrPitchesLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                     quarterTonesPitchesLanguageKind)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      quarterTonesPitchesLanguageKind)
{}

msrPitchesLanguageAtom::~msrPitchesLanguageAtom ()
{}

void msrPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (
        theString);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map

    stringstream s;

    s <<
      "MSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void msrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string msrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void msrPitchesLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrPitchesLanguageAtom:" <<
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
    "fMsrQuarterTonesPitchesLanguageKindVariable" << " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable) <<
    endl;

  --gIndenter;
}

void msrPitchesLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_msrPitchesLanguageAtom& elt)
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
S_msrRenamePartAtom msrRenamePartAtom::create (
  const string&         longName,
  const string&         shortName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  map<string, string>&  stringToStringMapVariable)
{
  msrRenamePartAtom* o = new
    msrRenamePartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (o != nullptr);
  return o;
}

msrRenamePartAtom::msrRenamePartAtom (
  const string&         longName,
  const string&         shortName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  map<string, string>&  stringToStringMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToStringMapVariable (
      stringToStringMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrRenamePartAtom::~msrRenamePartAtom ()
{}

void msrRenamePartAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrRenamePartAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*(.+)[[:space:]]*"
    ":"
    "[[:space:]]*(.+)[[:space:]]*"
  );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part rename string '" << theString <<
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
          '[' << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-msrPartRename argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    oldPartName = sm [1],
    newPartName = sm [2];

  mfTrimFromBothEnds (oldPartName);
  mfTrimFromBothEnds (newPartName);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fStringToStringMapVariable.find (oldPartName);

  if (it != fStringToStringMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << oldPartName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringToStringMapVariable [oldPartName] = newPartName;
    fSetByAnOption = true;
  }
}

void msrRenamePartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrRenamePartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrRenamePartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrRenamePartAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrRenamePartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrRenamePartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrRenamePartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    s << "[EMPTY]";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrRenamePartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrRenamePartAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToStringMapVariable" << " : " <<
    endl;

  if (! fStringToStringMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void msrRenamePartAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToStringMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", set by an option";

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_msrRenamePartAtom& elt)
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
S_msrOahGroup gGlobalMsrOahGroup;

S_msrOahGroup msrOahGroup::create ()
{
  msrOahGroup* o = new msrOahGroup ();
  assert (o != nullptr);
  return o;
}

msrOahGroup::msrOahGroup ()
  : oahGroup (
    "MSR",
    "help-msr", "hmsr",
R"(These options control the way MSR data is handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsrOahGroup ();
}

msrOahGroup::~msrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msrOahGroup::initializeMsrtracingOah ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "MSR Trace",
      "help-msr-trace", "hmsrt",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-msr", "tmsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "fTraceMsr",
        fTraceMsr));

  // MSR visitors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-msr-visitors", "tmsrvis",
R"(Write a trace of the MSR graphs visiting activity to standard error.)",
        "fTraceMsrVisitors",
        fTraceMsrVisitors));

  // MSR durations

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-msr-durations", "tmsrd",
R"(Write a trace of the MSR durations handling activity to standard error.)",
        "fTraceMsrDurations",
        fTraceMsrDurations));
}
#endif

void msrOahGroup::initializeMsrDisplayOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Display",
      "help-msr-trace-and-display", "hmsrtd",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // display partgroups

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-part-groups", "dpg",
R"(Write the structure of the part groups to standard error.)",
        "fDisplayPartGroups",
        fDisplayPartGroups));

  // display MSR skeleton

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-skeleton", "dmsrskel",
R"(Write the contents of the MSR skeleton data to standard error.)",
        "fDisplayMsrSkeleton",
        fDisplayMsrSkeleton));

  // display first MSR

  S_oahBooleanAtom
    displayFirstMsrShortBooleanAtom =
      oahBooleanAtom::create (
        "display-msr-1-short", "dmsr1short",
R"(Write the contents of the first MSR data, short version, to standard error.)",
        "fDisplayFirstMsrShort",
        fDisplayFirstMsrShort);
  subGroup->
    appendAtomToSubGroup (
      displayFirstMsrShortBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahAtomAlias::create (
        "display-msr-1", "dmsr1",
R"(Write the contents of the first MSR data, short version, to standard error.
This option is an alias for '-display-msr-1-short, -dmsr1short'.)",
        displayFirstMsrShortBooleanAtom));

  S_oahBooleanAtom
    displayFirstMsrFullBooleanAtom =
      oahBooleanAtom::create (
        "display-msr-1-full", "dmsr1full",
  R"(Write the contents of the first MSR data, full version, to standard error.)",
        "fDisplayFirstMsrFull",
        fDisplayFirstMsrFull);

  subGroup->
    appendAtomToSubGroup (
      displayFirstMsrFullBooleanAtom);

  // display first MSR slices

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-1-slices", "dmsr1slices",
R"(Write the contents of the first MSR data slices to standard error.)",
        "fDisplayFirstMsrShortSlices",
        fDisplayFirstMsrShortSlices));

  // display second MSR

  S_oahBooleanAtom
    displaySecondMsrShortBooleanAtom =
      oahBooleanAtom::create (
        "display-msr-2-short", "dmsr2short",
R"(Write the contents of the second MSR data, short version, to standard error.)",
        "fDisplaySecondMsrShort",
        fDisplaySecondMsrShort);
  subGroup->
    appendAtomToSubGroup (
      displaySecondMsrShortBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahAtomAlias::create (
        "display-msr-2", "dmsr2",
R"(Write the contents of the second MSR data, short version, to standard error.
This option is an alias for '-display-msr-2-short, -dmsr2short'.)",
        displaySecondMsrShortBooleanAtom));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-2-full", "dmsr2full",
R"(Write the contents of the second MSR data, full version, to standard error.)",
        "fDisplaySecondMsrFull",
        fDisplaySecondMsrFull));

  // display second MSR slices

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-2-slices", "dmsr2slices",
R"(Write the contents of the second MSR data slices to standard error.)",
        "fDisplaySecondMsrShortSlices",
        fDisplaySecondMsrShortSlices));

  // display first MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-1-names", "dmsr1names",
R"(Only write a view of the names in the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrShortNames",
        fDisplayFirstMsrShortNames));

  // display second MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-2-names", "dmsr2names",
R"(Only write a view of the names in the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrShortNames",
        fDisplaySecondMsrShortNames));

  // display first MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-1-summary", "dmsr1sum",
R"(Only write a summary of the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrShortSummary",
        fDisplayFirstMsrShortSummary));

  // display second MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr-2-summary", "dmsr2sum",
R"(Only write a summary of the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrShortSummary",
        fDisplaySecondMsrShortSummary));
}

void msrOahGroup::initializeMsrLanguagesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Languages",
      "help-msr-languages", "hmsrlang",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // notes pitches

  if (! setMsrQuarterTonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      msrQuarterTonesPitchesLanguageKind::kQTPNederlands; //LilyPond default value

  subGroup->
    appendAtomToSubGroup (
      msrPitchesLanguageAtom::create (
        "msr-pitches-language", "mplang",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the MSR logs and text views.
The NUMBER MSR pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            regex ("PITCHES_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            existingQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "fMsrQuarterTonesPitchesLanguageKind",
        fMsrQuarterTonesPitchesLanguageKind));
}

void msrOahGroup::initializeMsrHeaderOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "help-lpsr-header", "hlpsrh",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // file name as work title
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "use-filename-as-work-title", "ufawt",
R"(Use the file name as work title if there is none in the MusicXML data.
Standard input (-) becomes 'Standard input' in that case.)",
        "fUseFilenameAsWorkCreditTypeTitle",
        fUseFilenameAsWorkCreditTypeTitle));

  // lyricists as poets
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "use-lyricists-as-poets", "ulap",
R"(Set the 'poet' to the value of 'lyricist' in the LilyPond code \header.
This is useful because LilyPond doesn't feature a 'lyricist' variable.)",
        "fUseLyricistsAsPoets",
        fUseLyricistsAsPoets));
}

void msrOahGroup::initializeMsrPartsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "help-msr-parts", "hmsrp",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR rename part

  subGroup->
    appendAtomToSubGroup (
      msrRenamePartAtom::create (
        "msr-rename-part", "mrp",
        regex_replace (
R"(PART_RENAME_SPEC should be of the form ORIGINAL_NAME:NEW_NAME.
Rename part ORIGINAL_NAME to NEW_NAME, for example after displaying
the names in the score or a summary of the latter in a first run with options
'-dmsrnames, -display-msr-names' or 'dmsrsum, -display-msr-summary'.
There can be spaces around the ':', in which case quoting is needed.
There can be several occurrences of this option.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "PART_RENAME_SPEC",
        "fMsrPartsRenamingMap",
        fMsrPartsRenamingMap));
}

void msrOahGroup::initializeMsrStavesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "help-msr-staves", "hmsrs",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // single line staves

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-single-line-staves-as-rythmic", "cslsar",
R"(Create staves with a single line as rythmic staves.
By default, drum staves are created in this case.)",
        "fCreateSingleLineStavesAsRythmic",
        fCreateSingleLineStavesAsRythmic));
}

void msrOahGroup::initializeMsrVoicesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "help-msr-voices", "hmsrv",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // staff relative numbers

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-voices-staff-relative-numbers", "cvsrvn",
R"(Generate voices names with numbers relative to their staff.
By default, the voice numbers found are used,
which may be global to the score.)",
        "fCreateVoicesStaffRelativeNumbers",
        fCreateVoicesStaffRelativeNumbers));
}

void msrOahGroup::initializeMsrHarmoniesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Harmonies",
      "help-msr-harmonies", "hmsrh",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // show harmonies voices
  // --------------------------------------


  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "show-harmony-voices", "shv",
R"(Show the parts harmonies voices in the MSR data
even though it does not contain music.)",
        "fShowHarmoniesVoices",
        fShowHarmoniesVoices));
}

void msrOahGroup::initializeMsrFiguredBassOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Figured bass",
      "help-msr-figured-bass", "hmsrfb",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // show figured bass voices
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "show-figured-bass-voices", "sfbv",
R"(Show the figured bass voices in the MSR data
even though they do not contain music.)",
        "fShowFiguredBassVoices",
        fShowFiguredBassVoices));
}

void msrOahGroup::initializeMsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsrtracingOah ();
#endif

  // display
  // --------------------------------------
  initializeMsrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeMsrLanguagesOptions ();

  // header
  // --------------------------------------
  initializeMsrHeaderOptions ();

  // parts
  // --------------------------------------

  initializeMsrPartsOptions ();

  // staves
  // --------------------------------------
  initializeMsrStavesOptions ();

  // voices
  // --------------------------------------
  initializeMsrVoicesOptions ();

  // harmonies
  // --------------------------------------

  initializeMsrHarmoniesOptions ();

  // figured bass
  // --------------------------------------

  initializeMsrFiguredBassOptions ();
}

//______________________________________________________________________________
Bool msrOahGroup::setMsrQuarterTonesPitchesLanguage (const string& language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (language);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msrOahGroup::printMsrOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The MSR options are:" <<
    endl;

  ++gIndenter;

  // trace and display
  // --------------------------------------

  gLogStream <<
    "Trace and display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fTraceMsr" << " : " <<
    fTraceMsr <<
    endl <<

    setw (valueFieldWidth) << "fTraceMsrVisitors" << " : " <<
    fTraceMsrVisitors <<
    endl <<

    setw (valueFieldWidth) << "fDisplayPartGroups" << " : " <<
    fDisplayPartGroups <<
    endl <<

    setw (valueFieldWidth) << "fDisplayFirstMsrShort" << " : " <<
    fDisplayFirstMsrShort <<
    endl <<
    setw (valueFieldWidth) << "fDisplaySecondMsrShort" << " : " <<
    fDisplaySecondMsrShort <<
    endl <<

    setw (valueFieldWidth) << "fDisplayFirstMsrFull" << " : " <<
    fDisplayFirstMsrFull <<
    endl <<
    setw (valueFieldWidth) << "fDisplaySecondMsrFull" << " : " <<
    fDisplaySecondMsrFull <<
    endl <<

    setw (valueFieldWidth) << "fDisplayFirstMsrShortNames" << " : " <<
    fDisplayFirstMsrShortNames <<
    endl <<
    setw (valueFieldWidth) << "fDisplaySecondMsrShortNames" << " : " <<
    fDisplaySecondMsrShortNames <<
    endl <<

    setw (valueFieldWidth) << "fDisplayFirstMsrShortSummary" << " : " <<
    fDisplayFirstMsrShortSummary <<
    endl <<

    setw (valueFieldWidth) << "fDisplaySecondMsrShortSummary" << " : " <<
    fDisplaySecondMsrShortSummary <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
     "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "msrPitchesLanguage" << " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKind) <<
    endl;

  --gIndenter;

  // parts
  // --------------------------------------

  gLogStream <<
     "Parts:" <<
    endl;

  ++gIndenter;

  // parts renaming

  gLogStream << left <<
    setw (valueFieldWidth) << "parts renaming" << " : ";

  if (! fMsrPartsRenamingMap.size ()) {
    gLogStream <<
      "[NONE]";
  }
  else {
    for (
      map<string, string>::const_iterator i =
        fMsrPartsRenamingMap.begin ();
      i != fMsrPartsRenamingMap.end ();
      ++i
  ) {
        gLogStream <<
          "\"" << ((*i).first) << "\" -> \"" << ((*i).second) << "\"";
    } // for
  }

  gLogStream << endl;

  --gIndenter;

  // staves
  // --------------------------------------

  gLogStream <<
    "Staves:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fCreateSingleLineStavesAsRythmic" << " : " <<
    fCreateSingleLineStavesAsRythmic <<
    endl;

  --gIndenter;

  // voices
  // --------------------------------------

  gLogStream <<
    "Voices:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) <<
    "fCreateVoicesStaffRelativeNumbers" << " : " <<
    fCreateVoicesStaffRelativeNumbers <<
    endl;

  --gIndenter;

  // harmonies
  // --------------------------------------

  gLogStream <<
    "Harmonies:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fShowHarmoniesVoices" << " : " <<
    fShowHarmoniesVoices <<
    endl;

  --gIndenter;

  // figured bass
  // --------------------------------------

  gLogStream <<
    "Figured bass:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fShowFiguredBassVoices" << " : " <<
    fShowFiguredBassVoices <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrOahGroup& elt)
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
S_msrOahGroup createGlobalMsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global MSR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsrOahGroup) {
    // create the global options group
    gGlobalMsrOahGroup =
      msrOahGroup::create ();
    assert (gGlobalMsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsrOahGroup;
}


}
