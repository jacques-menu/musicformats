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

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "msrMidi.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string lilypondScoreNotationKindAsString (
  lilypondScoreNotationKind notationKind)
{
  string result;

  switch (notationKind) {
    case lilypondScoreNotationKind::kNotationWestern:
      result = "western";
      break;
    case lilypondScoreNotationKind::kNotationJianpu:
      result = "jiǎnpǔ";
      break;
    case lilypondScoreNotationKind::kNotationABC:
      result = "ABC";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_lilypondScoreOutputKindAtom lilypondScoreOutputKindAtom::create (
  const string&        shortName,
  const string&        longName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
{
  lilypondScoreOutputKindAtom* o = new
    lilypondScoreOutputKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondScoreOutputKindVariable);
  assert (o != nullptr);
  return o;
}

lilypondScoreOutputKindAtom::lilypondScoreOutputKindAtom (
  const string&        shortName,
  const string&        longName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lilypondScoreOutputKindVariable)
{}

lilypondScoreOutputKindAtom::~lilypondScoreOutputKindAtom ()
{}

void lilypondScoreOutputKindAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondScoreOutputKindAtom'" <<
      endl;
  }
#endif
}

void lilypondScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondScoreOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondScoreOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

string lilypondScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

void lilypondScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondScoreOutputKindAtom:" <<
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
    "fLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondScoreOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"";

  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_lilypondScoreOutputKindAtom& elt)
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
S_lilypondTransposePartNameAtom lilypondTransposePartNameAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartNameAtom* o = new
    lilypondTransposePartNameAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (o != nullptr);
  return o;
}

lilypondTransposePartNameAtom::lilypondTransposePartNameAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToMsrSemiTonesPitchAndOctaveMapVariable (
      stringToMsrSemiTonesPitchAndOctaveMapVariable)
{}

lilypondTransposePartNameAtom::~lilypondTransposePartNameAtom ()
{}

void lilypondTransposePartNameAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondTransposePartNameAtom'" <<
      endl;
  }
#endif

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  string regularExpression (
    "(.*)" // partName
    "="
    "(.*)" // destinationPitchName
    );
//    "[[:space:]]*(.*)[[:space:]]*" JMI
//    "="
//    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

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
          '[' << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-lilypond-transpose-part-name argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    partName             = sm [1],
    destinationPitchName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partName);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part name \"" << partName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partName] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_MF_NO_INPUT_LINE_NUMBER,
        destinationPitchName);

    fSetByAnOption = true;
  }
}

void lilypondTransposePartNameAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondTransposePartNameAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondTransposePartNameAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartNameAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondTransposePartNameAtom::asShortNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      '-' << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

string lilypondTransposePartNameAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      '-' << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

void lilypondTransposePartNameAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartNameAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << " : " <<
    endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
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

void lilypondTransposePartNameAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fSetByAnOption) {
    os <<
      "ffSetByAnOption: " <<
      fSetByAnOption;
  }

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "Part name \"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_lilypondTransposePartNameAtom& elt)
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
S_lilypondTransposePartIDAtom lilypondTransposePartIDAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartIDAtom* o = new
    lilypondTransposePartIDAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (o != nullptr);
  return o;
}

lilypondTransposePartIDAtom::lilypondTransposePartIDAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  map<string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringToMsrSemiTonesPitchAndOctaveMapVariable (
      stringToMsrSemiTonesPitchAndOctaveMapVariable)
{}

lilypondTransposePartIDAtom::~lilypondTransposePartIDAtom ()
{}

void lilypondTransposePartIDAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondTransposePartIDAtom'" <<
      endl;
  }
#endif

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  string regularExpression (
    "(.*)" // partID
    "="
    "(.*)" // destination pitch name
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

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
          '[' << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-lilypond-transpose-part-id argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    partID               = sm [1],
    destinationPitchName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> partID = \"" << partID << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"" <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partID);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part ID \"" << partID << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (s.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partID] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_MF_NO_INPUT_LINE_NUMBER,
        destinationPitchName);

    fSetByAnOption = true;
  }
}

void lilypondTransposePartIDAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondTransposePartIDAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondTransposePartIDAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondTransposePartIDAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondTransposePartIDAtom::asShortNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      '-' << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

string lilypondTransposePartIDAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    s <<
      '-' << fShortName << " \"";

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    s << "\"";
  }

  return s.str ();
}

void lilypondTransposePartIDAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartIDAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << " : " <<
    endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
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

void lilypondTransposePartIDAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fSetByAnOption) {
    os <<
      "ffSetByAnOption: " <<
      fSetByAnOption;
  }

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_lilypondTransposePartIDAtom& elt)
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
S_msrOctaveEntryVariable msrOctaveEntryVariable::create (
  const string&      variableName,
  msrOctaveEntryKind octaveEntryKind)
{
  msrOctaveEntryVariable* o = new
    msrOctaveEntryVariable (
      variableName,
      octaveEntryKind);
  assert (o != nullptr);
  return o;
}

msrOctaveEntryVariable::msrOctaveEntryVariable (
  const string&      variableName,
  msrOctaveEntryKind octaveEntryKind)
{
  fVariableName    = variableName;
  fOctaveEntryKind = octaveEntryKind;
}

msrOctaveEntryVariable::~msrOctaveEntryVariable ()
{}

void msrOctaveEntryVariable::setRelativeOctaveEntryVariableValue (
  msrOctaveEntryKind value)
{
  fOctaveEntryKind = value;
}

void msrOctaveEntryVariable::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OctaveEntryVariable :" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "fOctaveEntryKind" << " : " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKind) <<
    endl;

  --gIndenter;
}

void msrOctaveEntryVariable::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKind) <<
    endl;

  // no CamelCase here, these strings are used in the command line options

  switch (fOctaveEntryKind) {
    case msrOctaveEntryKind::kOctaveEntryRelative: // default value
      os << "kOctaveEntryRelative";
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      os << "kOctaveEntryAbsolute";
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      os << "kOctaveEntryFixed";
      break;
  } // switch
}

ostream& operator << (ostream& os, const msrOctaveEntryVariable& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondAbsoluteOctaveEntryAtom lilypondAbsoluteOctaveEntryAtom::create (
  const string&           longName,
  const string&           shortName,
  const string&           description,
  const string&           variableName,
  msrOctaveEntryVariable& octaveEntryKindVariable)
{
  lilypondAbsoluteOctaveEntryAtom* o = new
    lilypondAbsoluteOctaveEntryAtom (
      longName,
      shortName,
      description,
      variableName,
      octaveEntryKindVariable);
  assert (o != nullptr);
  return o;
}

lilypondAbsoluteOctaveEntryAtom::lilypondAbsoluteOctaveEntryAtom (
  const string&           longName,
  const string&           shortName,
  const string&           description,
  const string&           variableName,
  msrOctaveEntryVariable& octaveEntryKindVariable)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout),
    fOctaveEntryKindVariable (
      octaveEntryKindVariable)
{}

lilypondAbsoluteOctaveEntryAtom::~lilypondAbsoluteOctaveEntryAtom ()
{}

void lilypondAbsoluteOctaveEntryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () <<
      "' is a lilypondAbsoluteOctaveEntryAtom" <<
      endl;
  }
#endif

  // set octave entry kind
  fOctaveEntryKindVariable.setOctaveEntryKind (
    msrOctaveEntryKind::kOctaveEntryAbsolute);
}

void lilypondAbsoluteOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

void lilypondAbsoluteOctaveEntryAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  stringstream s;

  s <<
    "applying absolute octave entry option \"" << fetchNames () <<
    "\" with a value";

  oahInternalError (s.str ());
}

void lilypondAbsoluteOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondAbsoluteOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "octaveEntryKindVariable" << " : " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    endl;

  --gIndenter;
}

void lilypondAbsoluteOctaveEntryAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
//   os << left << // JMI CAFE
//     setw (valueFieldWidth) <<
//     fOctaveEntryKindVariable.getVariableName () <<
//     " : " <<
//     "- no possible value here -" <<
//     endl;
}

//______________________________________________________________________________
S_lilypondRelativeOctaveEntryAtom lilypondRelativeOctaveEntryAtom::create (
  const string&   shortName,
  const string&   longName,
  const string&   description,
  const string&   valueSpecification,
  const string&   variableName,
  msrOctaveEntryVariable&
                  octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                  lilypondRelativeOctaveEntryVariable)
{
  lilypondRelativeOctaveEntryAtom* o = new
    lilypondRelativeOctaveEntryAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      octaveEntryKindVariable,
      lilypondRelativeOctaveEntryVariable);
  assert (o != nullptr);
  return o;
}

lilypondRelativeOctaveEntryAtom::lilypondRelativeOctaveEntryAtom (
  const string&   shortName,
  const string&   longName,
  const string&   description,
  const string&   valueSpecification,
  const string&   variableName,
  msrOctaveEntryVariable&
                  octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                  lilypondRelativeOctaveEntryVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fOctaveEntryKindVariable (
      octaveEntryKindVariable),
    fSemiTonesPitchAndOctaveVariable (
      lilypondRelativeOctaveEntryVariable)
{}

lilypondRelativeOctaveEntryAtom::~lilypondRelativeOctaveEntryAtom ()
{}

void lilypondRelativeOctaveEntryAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the relative reference note

/* JMI
  // is it in the score output kinds map?

  map<string, msrOctaveEntryKind>::const_iterator
    it =
      gGlobalMsrOctaveEntryKindsMap.find (
        theString);

  if (it == gGlobalMsrOctaveEntryKindsMap.end ()) {
    // no, octave entry kind is unknown in the map

    printOptionsSummary (gLogStream);

    stringstream s;

    s <<
      "octave entry kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsrOctaveEntryKindsMap.size () <<
      " known octave entry kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsrOctaveEntryKinds ();

    --gIndenter;

    oahError (s.str ());
  }

  if (! theString.size ()) {
    oahError ("'the '-rel, relative' option needs a reference pitch and octave");
  }
*/

  // set octave entry kind
  fOctaveEntryKindVariable.setOctaveEntryKind (
    msrOctaveEntryKind::kOctaveEntryRelative);

  // set relative octave reference value
  setRelativeOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_MF_NO_INPUT_LINE_NUMBER,
      theString));
}

void lilypondRelativeOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondRelativeOctaveEntryAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lilypondRelativeOctaveEntryAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lilypondRelativeOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondRelativeOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "octaveEntryKindVariable" << " : " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    endl;

  --gIndenter;
}

void lilypondRelativeOctaveEntryAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOctaveEntryKindVariable <<
    " : ";

  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }
}

ostream& operator << (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt)
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
S_lilypondFixedOctaveEntryAtom lilypondFixedOctaveEntryAtom::create (
  const string&       longName,
  const string&       shortName,
  const string&       description,
  const string&       valueSpecification,
  const string&       variableName,
  msrOctaveEntryVariable&
                      octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                      lilypondFixedOctaveEntryVariable)
{
  lilypondFixedOctaveEntryAtom* o = new
    lilypondFixedOctaveEntryAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      octaveEntryKindVariable,
      lilypondFixedOctaveEntryVariable);
  assert (o != nullptr);
  return o;
}

lilypondFixedOctaveEntryAtom::lilypondFixedOctaveEntryAtom (
  const string&       longName,
  const string&       shortName,
  const string&       description,
  const string&       valueSpecification,
  const string&       variableName,
  msrOctaveEntryVariable&
                      octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                      lilypondFixedOctaveEntryVariable)
  : oahAtomExpectingAValue (
      longName,
      shortName,
      description),
    fOctaveEntryKindVariable (
      octaveEntryKindVariable),
    fSemiTonesPitchAndOctaveVariable (
      lilypondFixedOctaveEntryVariable)
{}

lilypondFixedOctaveEntryAtom::~lilypondFixedOctaveEntryAtom ()
{}

void lilypondFixedOctaveEntryAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondFixedOctaveEntryAtom'" <<
      endl;
  }
#endif

  // theString contains the fixed reference note

  // set octave entry kind
  fOctaveEntryKindVariable.setOctaveEntryKind (
    msrOctaveEntryKind::kOctaveEntryFixed);

  // set fixed octave reference value
  setFixedOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_MF_NO_INPUT_LINE_NUMBER,
      theString));
}

void lilypondFixedOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondFixedOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondFixedOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondFixedOctaveEntryAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lilypondFixedOctaveEntryAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lilypondFixedOctaveEntryAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondFixedOctaveEntryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "octaveEntryKindVariable" << " : " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    endl;

  --gIndenter;
}

void lilypondFixedOctaveEntryAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOctaveEntryKindVariable <<
    " : ";

  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }
}

ostream& operator << (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt)
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
S_lilypondAccidentalStyleKindAtom lilypondAccidentalStyleKindAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  lpsrAccidentalStyleKind&
                lilypondAccidentalStyleKindVariable)
{
  lilypondAccidentalStyleKindAtom* o = new
    lilypondAccidentalStyleKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondAccidentalStyleKindVariable);
  assert (o != nullptr);
  return o;
}

lilypondAccidentalStyleKindAtom::lilypondAccidentalStyleKindAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  lpsrAccidentalStyleKind&
                lilypondAccidentalStyleKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrAccidentalStyleKindVariable (
      lilypondAccidentalStyleKindVariable)
{}

lilypondAccidentalStyleKindAtom::~lilypondAccidentalStyleKindAtom ()
{}

void lilypondAccidentalStyleKindAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the accidental styles map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gGlobalLpsrAccidentalStyleKindsMap.find (
        theString);

  if (it == gGlobalLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidental style is unknown in the map
    stringstream s;

    s <<
      "LPSR accidental style '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrAccidentalStyleKindsMap.size () - 1 <<
      " known LPSR accidental styles are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrAccidentalStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrAccidentalStyleKindVariable (
    (*it).second);
}

void lilypondAccidentalStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondAccidentalStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondAccidentalStyleKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondAccidentalStyleKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return s.str ();
}

string lilypondAccidentalStyleKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return s.str ();
}

void lilypondAccidentalStyleKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondAccidentalStyleKindAtom:" <<
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
    "fLpsrAccidentalStyleKindVariable" << " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void lilypondAccidentalStyleKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable);

  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt)
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
S_lilypondChordsDisplayAtom lilypondChordsDisplayAtom::create (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  list<pair<string, string> >&
                        lilypondChordsDisplayVariable)
{
  lilypondChordsDisplayAtom* o = new
    lilypondChordsDisplayAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondChordsDisplayVariable);
  assert (o != nullptr);
  return o;
}

lilypondChordsDisplayAtom::lilypondChordsDisplayAtom (
  const string&         shortName,
  const string&         longName,
  const string&         description,
  const string&         valueSpecification,
  const string&         variableName,
  list<pair<string, string> >&
                        lilypondChordsDisplayVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringsPairListVariable (
      lilypondChordsDisplayVariable)
{}

lilypondChordsDisplayAtom::~lilypondChordsDisplayAtom ()
{}

void lilypondChordsDisplayAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'" <<
      endl;
  }
#endif

  /*
    % Exception music is chords with markups
    #(define (lower-extension pitch chbass)
       "Return lowered markup for pitch note name."
       #{
         \markup \raise #-1.9 \halign #0.2
         #(note-name->markup pitch chbass)
       #})

    chExceptionMusic = {
      <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
      <c e g bes>1-\markup { \super "7" }
      <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
      <c f g bes>1-\markup { \super {"7(sus4)"} }
      <c e g a d'>1-\markup { \super "6/9" }
      <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
      <c f g bes d'>1-\markup { \super {"9(sus4)"} }
      <c e g bes d'>1-\markup { \super "9" }
      <c e g b d'>1-\markup { \super "maj9" }
      <c e gis bes d'>1-\markup { \super "9+" }
      <c e g bes d' fis'>1-\markup { \super "9#11" }
      <c e g bes d' f'>1-\markup { \super "11" }
      <c e g bes d' a'>1-\markup { \super "13" }
      <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
      <c e g a d'>1-\markup { \super "6(add9)" }
    }

    chExceptions = #(append
                     (sequential-music-to-chord-exceptions chExceptionMusic #t)
                     ignatzekExceptions)

    -7b5:
      -cd, -chord-display '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'
  */

  // theString contains a specification such as:
  //   '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "(<.+>)"
    "[[:space:]]+"
    "(.+)"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for chords display string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-chords-display argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string chordContents     = sm [1];
  string chordsDisplaycode = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "FAA chordContents  = " <<
      chordContents <<
      endl <<
      "chordsDisplaycode = " <<
      chordsDisplaycode <<
      endl;

  setStringsPairListVariable (
    make_pair (
      chordContents, chordsDisplaycode));
  }
#endif
}

void lilypondChordsDisplayAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondChordsDisplayAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondChordsDisplayAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondChordsDisplayAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondChordsDisplayAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ';

  if (fStringsPairListVariable.size ()) { // JMI superflous???
    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
      s << ","; // JMI
    } // for
  }

  return s.str ();
}

string lilypondChordsDisplayAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ';

  if (fStringsPairListVariable.size ()) { // JMI superflous???
    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
      s << ","; // JMI
    } // for
  }

  return s.str ();
}

void lilypondChordsDisplayAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondChordsDisplayAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringsPairListVariable" << " : '" <<
    endl;

  if (fStringsPairListVariable.size ()) {
    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        endl;
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void lilypondChordsDisplayAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (fStringsPairListVariable.size ()) {
    os << endl;
    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for

    if (fSetByAnOption) {
      os <<
        "ffSetByAnOption: " <<
        fSetByAnOption <<
        endl;
    }

    --gIndenter;
  }
  else {
    os << "[EMPTY]";
  }
  os << endl;
}

ostream& operator << (ostream& os, const S_lilypondChordsDisplayAtom& elt)
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
S_lilypondLyricsDurationsKindAtom lilypondLyricsDurationsKindAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrLyricsDurationsKind&
                    lilypondLyricsDurationsKindVariable)
{
  lilypondLyricsDurationsKindAtom* o = new
    lilypondLyricsDurationsKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondLyricsDurationsKindVariable);
  assert (o != nullptr);
  return o;
}

lilypondLyricsDurationsKindAtom::lilypondLyricsDurationsKindAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrLyricsDurationsKind&
                    lilypondLyricsDurationsKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrLyricsDurationsKindVariable (
      lilypondLyricsDurationsKindVariable)
{}

lilypondLyricsDurationsKindAtom::~lilypondLyricsDurationsKindAtom ()
{}

void lilypondLyricsDurationsKindAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondLyricsDurationsKindAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the lyrics alignment kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondLyricsDurationsKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrLyricsDurationsKind>::const_iterator
    it =
      gGlobalLpsrLyricsDurationsKindsMap.find (
        theString);

  if (it == gGlobalLpsrLyricsDurationsKindsMap.end ()) {
    // no, lyrics alignment kind is unknown in the map
    stringstream s;

    s <<
      "LPSR lyrics alignment kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrLyricsDurationsKindsMap.size () - 1 <<
      " known LPSR lyrics alignment kind are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrLyricsDurationsKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrLyricsDurationsKindVariable (
    (*it).second);
}

void lilypondLyricsDurationsKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondLyricsDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsDurationsKindAtom>*> (v)) {
        S_lilypondLyricsDurationsKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondLyricsDurationsKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondLyricsDurationsKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondLyricsDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsDurationsKindAtom>*> (v)) {
        S_lilypondLyricsDurationsKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondLyricsDurationsKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondLyricsDurationsKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondLyricsDurationsKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondLyricsDurationsKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable);

  return s.str ();
}

string lilypondLyricsDurationsKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable);

  return s.str ();
}

void lilypondLyricsDurationsKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondLyricsDurationsKindAtom:" <<
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
    "fLpsrLyricsDurationsKindVariable" << " : " <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable) <<
    endl;

  --gIndenter;
}

void lilypondLyricsDurationsKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    lpsrLyricsDurationsKindAsString (
      fLpsrLyricsDurationsKindVariable);

  if (fSetByAnOption) {
    os <<
      ", set by an option";

  }
  os << endl;
}

ostream& operator << (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt)
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
S_lilypondDynamicsTextSpannersStyleKindAtom lilypondDynamicsTextSpannersStyleKindAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrDynamicsTextSpannersStyleKind&
                    lilypondDynamicsTextSpannersStyleKindVariable)
{
  lilypondDynamicsTextSpannersStyleKindAtom* o = new
    lilypondDynamicsTextSpannersStyleKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondDynamicsTextSpannersStyleKindVariable);
  assert (o != nullptr);
  return o;
}

lilypondDynamicsTextSpannersStyleKindAtom::lilypondDynamicsTextSpannersStyleKindAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrDynamicsTextSpannersStyleKind&
                    lilypondDynamicsTextSpannersStyleKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrdDynamicsTextSpannersStyleKindVariable (
      lilypondDynamicsTextSpannersStyleKindVariable)
{}

lilypondDynamicsTextSpannersStyleKindAtom::~lilypondDynamicsTextSpannersStyleKindAtom ()
{}

void lilypondDynamicsTextSpannersStyleKindAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondDynamicsTextSpannersStyleKindAtom'" <<
      endl;
  }
#endif

  // theString contains the dynamics text spanners style name:
  // is it in the lyrics alignment kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondDynamicsTextSpannersStyleKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrDynamicsTextSpannersStyleKind>::const_iterator
    it =
      getLpsrDynamicsTextSpannersStyleKindsMap ().find (
        theString);

  if (it == getLpsrDynamicsTextSpannersStyleKindsMap ().end ()) {
    // no, dynamics text spanners style kind is unknown in the map
    stringstream s;

    s <<
      "dynamics text spanners style kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      getLpsrDynamicsTextSpannersStyleKindsMap ().size () - 1 <<
      " known LPSR lyrics alignment kind are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrDynamicsTextSpannersStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrdDynamicsTextSpannersStyleKindVariable (
    (*it).second);
}

void lilypondDynamicsTextSpannersStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*> (v)) {
        S_lilypondDynamicsTextSpannersStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondDynamicsTextSpannersStyleKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lilypondDynamicsTextSpannersStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*> (v)) {
        S_lilypondDynamicsTextSpannersStyleKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lilypondDynamicsTextSpannersStyleKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lilypondDynamicsTextSpannersStyleKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string lilypondDynamicsTextSpannersStyleKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrDynamicsTextSpannersStyleKindAsString (fLpsrdDynamicsTextSpannersStyleKindVariable);

  return s.str ();
}

string lilypondDynamicsTextSpannersStyleKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrDynamicsTextSpannersStyleKindAsString (fLpsrdDynamicsTextSpannersStyleKindVariable);

  return s.str ();
}

void lilypondDynamicsTextSpannersStyleKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondDynamicsTextSpannersStyleKindAtom:" <<
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
    "fLpsrdDynamicsTextSpannersStyleKindVariable" << " : " <<
    lpsrDynamicsTextSpannersStyleKindAsString (
      fLpsrdDynamicsTextSpannersStyleKindVariable) <<
    endl;

  --gIndenter;
}

void lilypondDynamicsTextSpannersStyleKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    lpsrDynamicsTextSpannersStyleKindAsString (
      fLpsrdDynamicsTextSpannersStyleKindVariable);

  if (fSetByAnOption) {
    os <<
      ", set by an option";

  }
  os << endl;
}

ostream& operator << (ostream& os, const S_lilypondDynamicsTextSpannersStyleKindAtom& elt)
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
S_lpsr2lilypondOahGroup gGlobalLpsr2lilypondOahGroup;

S_lpsr2lilypondOahGroup lpsr2lilypondOahGroup::create ()
{
  lpsr2lilypondOahGroup* o = new lpsr2lilypondOahGroup ();
  assert (o != nullptr);
  return o;
}

lpsr2lilypondOahGroup::lpsr2lilypondOahGroup ()
  : oahGroup (
      "Lpsr2lilyPond",
      "help-lpsr-to-lilypond", "hlp2lily",
  R"(These options control which LilyPond code is generated.)",
      oahElementVisibilityKind::kElementVisibilityWhole),
    fOctaveEntryVariable (
      "fOctaveEntryVariable ???", // JMI
      msrOctaveEntryKind::kOctaveEntryAbsolute)
{
  initializeLilypondGenerationOahGroup ();
}

lpsr2lilypondOahGroup::~lpsr2lilypondOahGroup ()
{}

void lpsr2lilypondOahGroup::initializeLilypondVersionOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LilyPond version",
        "help-lilypond-version", "hlpversion",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // LilyPond version

  fLilypondVersionDefaultValue = "2.22.0";

  fLilypondVersion = fLilypondVersionDefaultValue;

  fLilypondVersionAtom =
    oahStringAtom::create (
      "lilypond-version", "lpv",
      regex_replace (
R"(Set the Lilypond '\version' to STRING in the Lilypond code.
The default is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        fLilypondVersionDefaultValue),
      "STRING",
      "fLilypondVersion",
      fLilypondVersion);

  subGroup->
    appendAtomToSubGroup (
      fLilypondVersionAtom);
}

void lpsr2lilypondOahGroup::initializeScoreNotationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Score notation",
        "help-lilypond-score-notation", "hlpsn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // score notation
  // --------------------------------------

//  fScoreNotationKind =  lilypondScoreNotationKind::kNotationWestern;
    // default value

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jianpu", "",
R"(Generate the score using the Jianpu (numbered) notation
instead of the default western notation.
This option needs lilypond-Jianpu to be accessible to LilyPond
(https://github.com/nybbs2003/lilypond-Jianpu/jianpu10a.ly).)",
        "fJianpu",
        fJianpu));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lyluatex", "",
R"(Generate lyLuaTeX code arout the LilyPond code to produce a '.tex' file.
(lyLuaTeX is available at https://www.ctan.org/pkg/lyluatex).)",
        "fLyLuaTexOutput",
        fLyLuaTexOutput));
}

void lpsr2lilypondOahGroup::initializeGlobalStaffSizeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Global staff size",
        "help-global-staff-size", "hgss",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // global staff size

  fStaffGlobalSizeDefaultValue = 20; // LilyPond default

  fGlobalStaffSize = fStaffGlobalSizeDefaultValue;

  fGlobalStaffSizeAtom =
    oahFloatAtom::create (
      "global-staff-size", "gss",
      regex_replace (
R"(Set the LilyPond '#(set-global-staff-size ...)' to NUMBER in the LilyPond code.
NUMBER should be a floating point or integer number.
The default is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        to_string (fStaffGlobalSizeDefaultValue)),
      "NUMBER",
      "fGlobalStaffSize",
      fGlobalStaffSize);

  subGroup->
    appendAtomToSubGroup (
      fGlobalStaffSizeAtom);
}

void lpsr2lilypondOahGroup::initializeIdentificationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Identification",
        "help-lilypond-identification", "hlpi",
R"(These options can be used to enforce values in the generated LilyPond code,
thus overriding the ones that may be present in the MSR data.)",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'identification' monoplex string atom
  // --------------------------------------

  S_oahFactorizedStringAtom
    identificationFactorizedStringAtom =
      oahFactorizedStringAtom::create (
        "set-variable-in-header", "svin",
        "Set variable HEADER_VARIABLE in the \\header in the generated LilyPond code.",
        "HEADER_VARIABLE",
        "STRING");

  subGroup->
    appendAtomToSubGroup (
      identificationFactorizedStringAtom);

  // MusicXML informations
  // --------------------------------------

  S_oahStringAtom
    rightsAtom =
      oahStringAtom::create (
        "rights", "",
R"(Set the 'rights' to STRING in the LilyPond code \header.)",
        "STRING",
        "fRights",
        fRights);
  subGroup->
    appendAtomToSubGroup (
      rightsAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      rightsAtom);

  S_oahStringAtom
    composerAtom =
      oahStringAtom::create (
        "composer", "",
R"(Set the 'composer' to STRING in the LilyPond code \header.)",
        "STRING",
        "fComposer",
        fComposer);
  subGroup->
    appendAtomToSubGroup (
      composerAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      composerAtom);

  S_oahStringAtom
    arrangerAtom =
      oahStringAtom::create (
        "arranger", "",
R"(Set the 'arranger' to STRING in the LilyPond code \header.)",
        "STRING",
        "fArranger",
        fArranger);
  subGroup->
    appendAtomToSubGroup (
      arrangerAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      arrangerAtom);

  fPoetAtom =
    oahStringAtom::create (
      "poet", "",
R"(Set the 'poet' to STRING in the LilyPond code \header.)",
      "STRING",
      "fPoet",
      fPoet);
  subGroup->
    appendAtomToSubGroup (
      fPoetAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      fPoetAtom);

  S_oahStringAtom
    lyricistAtom =
      oahStringAtom::create (
        "lyricist", "",
R"(Set the 'lyricist' to STRING in the LilyPond code \header.)",
        "STRING",
        "fLyricist",
        fLyricist);
  subGroup->
    appendAtomToSubGroup (
      lyricistAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      lyricistAtom);

  S_oahStringAtom
    softwareAtom =
      oahStringAtom::create (
        "software", "",
R"(Set the 'software' to STRING in the LilyPond code \header.)",
        "STRING",
        "fSoftware",
        fSoftware);
  subGroup->
    appendAtomToSubGroup (
      softwareAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      softwareAtom);

  // LilyPond informations

  S_oahStringAtom
    dedicationAtom =
      oahStringAtom::create (
        "dedication", "",
R"(Set 'dedication' to STRING in the LilyPond code \header.)",
        "STRING",
        "fDedication",
        fDedication);
  subGroup->
    appendAtomToSubGroup (
      dedicationAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      dedicationAtom);

  S_oahStringAtom
    pieceAtom =
      oahStringAtom::create (
        "piece", "",
R"(Set 'piece' to STRING in the LilyPond code \header.)",
        "STRING",
        "fPiece",
        fPiece);
  subGroup->
    appendAtomToSubGroup (
      pieceAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      pieceAtom);

  S_oahStringAtom
    opusAtom =
      oahStringAtom::create (
        "opus", "",
R"(Set 'opus' to STRING in the LilyPond code \header.)",
        "STRING",
        "fOpus",
        fOpus);
  subGroup->
    appendAtomToSubGroup (
      opusAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      opusAtom);

//   S_oahBooleanAtom JMI v0.9.62
//     opusInTitleAtom =
//       oahBooleanAtom::create (
//          "opus-in-title", "oit",
// R"(Place the opus number in the LilyPond code \header.)",
//         "fOpusInTitle",
//         fOpusInTitle);
//   subGroup->
//     appendAtomToSubGroup (
//       opusInTitleAtom);

  S_oahStringAtom
    titleAtom =
      oahStringAtom::create (
        "title", "",
R"(Set 'title' to STRING in the LilyPond code \header.)",
        "STRING",
        "fTitle",
        fTitle);
  subGroup->
    appendAtomToSubGroup (
      titleAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      titleAtom);

  S_oahStringAtom
    subtitleAtom =
      oahStringAtom::create (
        "subtitle", "",
R"(Set 'subtitle' to STRING in the LilyPond code \header.)",
        "STRING",
        "fSubTitle",
        fSubTitle);
  subGroup->
    appendAtomToSubGroup (
      subtitleAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      subtitleAtom);

  S_oahStringAtom
    subsubtitleAtom =
      oahStringAtom::create (
        "subsubtitle", "",
R"(Set 'subsubtitle' to STRING in the LilyPond code \header.)",
        "STRING",
        "fSubSubTitle",
        fSubSubTitle);
  subGroup->
    appendAtomToSubGroup (
      subsubtitleAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      subsubtitleAtom);

  S_oahStringAtom
    instrumentAtom =
      oahStringAtom::create (
        "header-instrument", "",
R"(Set 'instrument' to STRING in the LilyPond code \header.)",
        "STRING",
        "fHeaderInstrument",
        fHeaderInstrument);
  subGroup->
    appendAtomToSubGroup (
      instrumentAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      instrumentAtom);

  S_oahStringAtom
    meterAtom =
      oahStringAtom::create (
        "header-meter", "meter",
R"(Set 'meter' to STRING in the LilyPond code \header.)",
        "STRING",
        "fHeaderMeter",
        fHeaderMeter);
  subGroup->
    appendAtomToSubGroup (
      meterAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      meterAtom);

  S_oahStringAtom
    taglineAtom =
      oahStringAtom::create (
        "tagline", "",
R"(Set 'tagline' to STRING in the LilyPond code \header.)",
        "STRING",
        "fTagline",
        fTagline);
  subGroup->
    appendAtomToSubGroup (
      taglineAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      taglineAtom);

  S_oahStringAtom
    copyrightAtom =
      oahStringAtom::create (
        "copyright", "",
R"(Set 'copyright' to STRING in the LilyPond code \header.)",
        "STRING",
        "fCopyright",
        fCopyright);
  subGroup->
    appendAtomToSubGroup (
      copyrightAtom);
  identificationFactorizedStringAtom->
    addStringAtom (
      copyrightAtom);
}

void lpsr2lilypondOahGroup::initializePartsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "help-lilypond-parts", "hlpp",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // part name transposition
  // --------------------------------------

  S_lilypondTransposePartNameAtom
    transposePartNameAtom =
      lilypondTransposePartNameAtom::create (
        "lilypond-transpose-part-name", "lilytpn",
R"(PART_TRANSPOSITION_SPEC should be of the form PART_NAME:TRANSPOSITION.
Transpose part PART_NAME using TRANSPOSITION in the LilyPond code.
There can be spaces around the ':', in which case quoting is needed.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed, and they are relative to c\', i.e. middle C.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively.
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "fPartNamesTranspositionMap",
        fPartNamesTranspositionMap);

  transposePartNameAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
        transposePartNameAtom);

  // part ID transposition
  // --------------------------------------

  S_lilypondTransposePartIDAtom
    transposePartIDAtom =
      lilypondTransposePartIDAtom::create (
        "lilypond-transpose-part-id", "lilytranspi",
R"(Transpose part PART_ID using TRANSPOSITION in the LilyPond code.
PART_TRANSPOSITION_SPEC can be:
'PART_ID = TRANSPOSITION'
or
"PART_ID = TRANSPOSITION"
This is handly when a part doesn't have a part name.
See option '-lilypond-transpose-part-name' for the details.
There can be several occurrences of this option.)",
        "PART_TRANSPOSITION_SPEC",
        "fPartIDsTranspositionMap",
        fPartIDsTranspositionMap);

  transposePartIDAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
        transposePartIDAtom);
}

void lpsr2lilypondOahGroup::initializeVoicesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "help-lilypond-voices", "hlpvoices",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // auto-voices
  // --------------------------------------

if (false) // JMI CAFE
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "auto-voices", "",
R"###(Generate the 'auto-voices' Scheme functions in the LilyPond output.
This is a temporary option for experimenting with rests code generation.)###",
        "fAutoVoices",
        fAutoVoices));
}

void lpsr2lilypondOahGroup::initializeEngraversOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Engravers",
        "help-lilypond-engravers", "hlpe",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // ambitus engraver
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ambitus", "",
R"(Generate an ambitus range at the beginning of the staves/voices.)",
        "fAmbitusEngraver",
        fAmbitusEngraver));

  // custos engraver
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "custos", "",
R"(Generate custos at the end of the lines.)",
        "fCustosEngraver",
        fCustosEngraver));
}

void lpsr2lilypondOahGroup::initializeClefsKeysTimesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "clefs, keys, time signatures",
        "help-clefs-keys-times", "hckt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // clefs
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-initial-treble-clef", "nitc",
R"(Don't generate an initial treble clef, which is LiyPond's default clef.)",
        "fNoInitialTrebleClef",
        fNoInitialTrebleClef));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "comment-clef-changes", "ccc",
R"(Comment clef changes in the LilyPond code.
They won't show up in the score, but the information is not lost.)",
        "fCommentClefChanges",
        fCommentClefChanges));

  // keys
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-initial-c-major-key", "nicmk",
R"(Don't generate an initial C major key, which is LiyPond's default key.)",
        "fNoInitialCMajorKey",
        fNoInitialCMajorKey));

  // time signatures
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-initial-common-time", "nict",
R"(Don't generate an initial common time (4/4) time, which is LiyPond's default time.)",
        "fNoInitialCommonTimeSignature",
        fNoInitialCommonTimeSignature));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "numerical-time-signatures", "numtimesigs",
R"(Generate numerical time signatures, such as '4/4' instead of 'C'.)",
        "fNumericalTimeSignature",
        fNumericalTimeSignature));
}

void lpsr2lilypondOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "help-lilypond-notes", "hlpn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // octave entry
  // --------------------------------------

  msrOctaveEntryKind
    octaveEntryKindDefaultValue =
      msrOctaveEntryKind::kOctaveEntryAbsolute; // relative is LilyPond's default value
  fOctaveEntryVariable.setOctaveEntryKind (
    octaveEntryKindDefaultValue);

  // leave fOctaveEntrySemiTonesPitchAndOctave equal to nullptr here,
  // since \relative without a pitch and absolute octave entry
  // can be used in LilyPond, in which case the pitch and actave is:
  fSemiTonesPitchAndOctaveDefaultValue = // JMI
    msrSemiTonesPitchAndOctave::create (
      // F under middle C, LilyPond default for relative octave entry
      msrSemiTonesPitchKind::kSTP_F_Natural,
      msrOctaveKind::kOctave3);

  subGroup->
    appendAtomToSubGroup (
      lilypondAbsoluteOctaveEntryAtom::create (
        "absolute", "abs",
R"(Use absolute octave entry in the generated LilyPond code.
This is the default.)",
        "fOctaveEntryVariable",
        fOctaveEntryVariable));

  subGroup->
    appendAtomToSubGroup (
      lilypondRelativeOctaveEntryAtom::create (
        "relative", "rel",
        regex_replace (
R"(Use relative octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and
an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -rel "c''".
The default is to use LilyPond's implicit reference 'DEFAULT_VALUE'.)",
          regex ("DEFAULT_VALUE"),
          msrSemiTonesPitchAndOctaveAsLilypondString (
            gGlobalLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind (),
            fSemiTonesPitchAndOctaveDefaultValue)),
        "PITCH_AND_OCTAVE",
        "fRelativeOctaveEntrySemiTonesPitchAndOctave",
        fOctaveEntryVariable,
        fRelativeOctaveEntrySemiTonesPitchAndOctave));

  subGroup->
    appendAtomToSubGroup (
      lilypondFixedOctaveEntryAtom::create (
        "fixed", "",
R"(Use fixed octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and
an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -fixed "c''")",
        "PITCH_AND_OCTAVE",
        "fFixedOctaveEntrySemiTonesPitchAndOctave",
        fOctaveEntryVariable,
        fFixedOctaveEntrySemiTonesPitchAndOctave));

  // durations
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-all-lilypond-durations", "gald",
R"(Generate all LilyPond durations.
The LilyPond default is for them is to be a quarter note,
or the last duration specified in the current voice if relevant.)",
        "fAllDurations",
        fAllDurations));

  // beaming
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-auto-beaming", "noab",
R"(Generate '\set Staff.autoBeaming = ##f' in each voice
to prevent LilyPond from handling beams automatically.)",
        "fNoAutoBeaming",
        fNoAutoBeaming));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-beams", "nbeams",
R"(Don't generate beams in the LilyPond code.
This is handy when the MusicXML data contains beam in vocal voices)",
        "fNoBeams",
        fNoBeams));

  // string numbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "roman-string-numbers", "rsn",
R"(Generate '\romanStringNumbers' in each voice
for LilyPond to generate roman instead of arabic string numbers.)",
        "fRomanStringNumbers",
        fRomanStringNumbers));

  // open strings
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "avoid-open-strings", "aos",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent LilyPond from using open strings.)",
        "fAvoidOpenStrings",
        fAvoidOpenStrings));

  // accidentals
  // --------------------------------------

  const lpsrAccidentalStyleKind
    lpsrAccidentalStyleKindDefaultValue =
      lpsrAccidentalStyleKind::kAccidentalStyleDefault; // default value

  fAccidentalStyleKind = lpsrAccidentalStyleKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lilypondAccidentalStyleKindAtom::create (
        "lilypond-accidental-style", "as", // JMI
          regex_replace (
            regex_replace (
              regex_replace (
R"(STYLE should be one of the NUMBER LilyPond accidental styles available:
ACCIDENTAL_STYLES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrAccidentalStyleKindsMap.size ())),
            regex ("ACCIDENTAL_STYLES"),
            existingLpsrAccidentalStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrAccidentalStyleKindAsLilypondString (
            lpsrAccidentalStyleKindDefaultValue)),
        "STYLE",
        "fAccidentalStyleKind",
        fAccidentalStyleKind));

  // multiple full-bar rests
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "compress-empty-measures-in-lilypond", "cemil",
R"(Compress empty measures in the LilyPond output. TEMP !!! JMI
This causes a \compressFullBarRests command to be generated.)",
        "fCompressFullBarRestsInLilypond",
        fCompressFullBarRestsInLilypond));

  // merge rests
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "merge-staff-common-rests-in-lilypond", "mscril",
R"(Merge common rests in LilyPond staves to obtain better looking scores.
Limitations: this only handles two voices
and does not handle multiple full-bar rests.)",
        "fMergeStaffCommonRests",
        fMergeStaffCommonRests));

  // multiple full-bar rests
  // --------------------------------------

  fMultipleFullBarRestsExpandLimitAtom =
      oahIntegerAtom::create (
        "multiple full-bar-rests-expand-limit", "fbrel",
R"(Set the maximum number of measures expanded in church rests to N,
where N is a positive integer.
The default value is 10.)",
        "N",
        "fMultipleFullBarRestsExpandLimit",
        fMultipleFullBarRestsExpandLimit);

  subGroup->
    appendAtomToSubGroup (
      fMultipleFullBarRestsExpandLimitAtom);

  // input line numbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "input-line-numbers", "iln",
        regex_replace (
R"(Generate after each note and barLine a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "fInputLineNumbers",
        fInputLineNumbers));

  // notes comments
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "notes-comments", "nc",
        regex_replace (
R"(Generate after each note a comment describing its nature.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "fNotesComments",
        fNotesComments));

  // original measure numbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "show-musicxml-measure-numbers", "smxmlmn",
        regex_replace (
R"(Generate after each end of measure a comment
containing its original MusicXML measure number.
This is useful to locate where to add line breaks and page breaks,
and when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "fOriginalMeasureNumbers",
        fOriginalMeasureNumbers));

  // generate positions in measures
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-positions-in-measures", "gpim",
        regex_replace (
R"(Generate after each note and barLine a comment containing
its position in the measure.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "fGenerateMeasurePositions",
        fGenerateMeasurePositions));

  // generate moments in measures
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-moments-in-measures", "gmim",
        regex_replace (
R"(Generate after each note and barLine a comment containing
its moment in the measure.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "fGenerateMomentsInMeasures",
        fGenerateMomentsInMeasures));

  // non-print notes head color
  // --------------------------------------

  fNonPrintNotesHeadRGBColor =
    msrRGBColor (0.0, 0.0, 0.0); // black by default

  fNonPrintNotesHeadRGBColorAtom =
    oahRGBColorAtom::create (
      "non-print-notes-head-color", "npnhc",
R"(Use RGB_COLOR for non print notes head color,
for MusicXML's <note print-object="no"> for example.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "fNonPrintNotesHeadRGBColor",
      fNonPrintNotesHeadRGBColor);

  subGroup->
    appendAtomToSubGroup (
      fNonPrintNotesHeadRGBColorAtom);
}

void lpsr2lilypondOahGroup::initializeBarsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Bar numbers",
        "help-lilypond-bars-numbers", "hlpbn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // show all bar numbers
  // --------------------------------------

  S_oahBooleanAtom
    allBarNumbersAtom =
      oahBooleanAtom::create (
        "all-bar-numbers", "abn",
R"(Generate LilyPond code to show all bar numbers.)",
        "fShowAllBarNumbers",
        fShowAllBarNumbers);

  subGroup->
    appendAtomToSubGroup (allBarNumbersAtom);

  // show measure numbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahAtomAlias::create (
        "all-lilypond-measure-numbers", "almn",
R"(Generate LilyPond code to show all measure numbers.
This option is an alias for '-all-bar-numbers, -abn'.)",
        allBarNumbersAtom));

  // show measure number at
  // --------------------------------------

  fShowNumbersAtMeasureAtom =
    oahStringSetElementAtom::create (
      "show-lilypond-measure-number-at", "slmna",
R"(Generate LilyPond code to show the measure number at measure MEASURE_NUMBER.)",
      "MEASURE_NUMBER",
      "fShowNumbersAtMeasureSet",
      fShowNumbersAtMeasureSet);

  subGroup->
    appendAtomToSubGroup (
      fShowNumbersAtMeasureAtom);

  // reset measure number
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapElementAtom::create (
        "reset-measure-number", "rmn",
R"(Generate a '\set Score.currentBarNumber = #NEW' command
at the beginning of measure OLD in the LilyPond code.
RESET_NUMBER_SPEC can be:
'OLD = NEW'
or
"OLD = NEW" .
OLD is the MusicXML original measure number (a string),
that can be generated in the LilyPond code in '| % ...' comments
with option '-show-musicxml-measure-numbers, -smxmlmn'.
NEW is a LilyPond (integer) measure number.
This comes in handy when scanning several movements from a single PDF score,
in which case measure numbers are a single sequence.
There can be several occurrences of this option.)",
        "RESET_NUMBER_SPEC",
        "fResetMeasureElementMeasureNumberMap",
        fResetMeasureElementMeasureNumberMap));

  // generate a box around all bar numbers
  // --------------------------------------

  S_oahBooleanAtom
    generateABoxAroundAllBarNumbers =
      oahBooleanAtom::create (
        "generate-a-box-around-all-bar-numbers", "gabaabn",
R"(Generate LilyPond code to show a box around all bar numbers.)",
        "fBoxAroundAllBarNumbers",
        fBoxAroundAllBarNumbers);

  subGroup->
    appendAtomToSubGroup (generateABoxAroundAllBarNumbers);

  // generate a box around bar number
  // --------------------------------------

  S_oahIntSetElementAtom
    generateABoxAroundBarNumberAtom =
      oahIntSetElementAtom::create (
        "generate-a-box-around-bar-number", "gababn",
R"(Generate a box around LilyPond bar number BAR_NUMBER, // purist JMI ???
where BAR_NUMBER is an integer.
This implies that bar numbers are centered on the bars.
There can be several occurrences of this option.)",
        "BAR_NUMBER",
        "fBoxAroundBarNumberSet",
        fBoxAroundBarNumberSet);

  subGroup->
    appendAtomToSubGroup (
      generateABoxAroundBarNumberAtom);

  // bar numbers size
  // --------------------------------------

  fBarNumbersSizeAtom =
    oahFloatAtom::create (
      "bar-numbers-size", "bns",
R"(Set the LilyPond bar numbers to SIZE, where SIZE is a floating number.
The default is 0, -1 is smaller, +1 is bigger.
Each step of 1 is approximately 12% larger; 6 steps are exactly a factor of 2.)",
      "SIZE",
      "fBarNumbersSize",
      fBarNumbersSize);

  subGroup->
    appendAtomToSubGroup (
      fBarNumbersSizeAtom);

  // bar number checks
  // --------------------------------------

  fNoBarNumberChecksAtom =
    oahBooleanAtom::create (
      "no-bar-number-checks", "nbnc",
R"(Don't generate bar number checks in the LilyPond code.)",
      "fNoBarNumberChecks",
      fNoBarNumberChecks);

  subGroup->
    appendAtomToSubGroup (
      fNoBarNumberChecksAtom);
}

void lpsr2lilypondOahGroup::initializePageAndLineBreaksOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page and line breaks",
        "help-lilypond-page-and-line-breaks", "hlpplb",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // page and line breaks
  // --------------------------------------

  fIgnoreLpsrPageBreaksAtom =
    oahBooleanAtom::create (
      "ignore-lpsr-page-breaks", "ilppb",
R"(Suppress the page breaks from the LPSR data - let LilyPond decide about them,
and don't generate an empty \myPageBreak.)",
      "fIgnoreLpsrPageBreaks",
      fIgnoreLpsrPageBreaks);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreLpsrPageBreaksAtom);

  // break page after measure number
  // --------------------------------------

/* JMI
  subGroup->
    appendAtomToSubGroup (
      oahStringsSetElementAtom::create (
        "break-page-after-measure-number", "bpamn",
R"(Generate a '\pageBreak' command after measure NUMBER in the LilyPond code.
NUMBER is a MusicXML measure number (a string), to be found in the latter.
This comes in handy when scanning several movements from a single PDF score.
There can be several occurrences of this option.)",
        "NUMBER",
        "fBreakPageAfterMeasureNumberSet",
        fBreakPageAfterMeasureNumberSet));
        */

  // line breaks
  // --------------------------------------

  // JMI fBreakLinesAtIncompleteRightMeasures = false; // ??? forgotten ???

  fIgnoreLpsrLineBreaksAtom =
    oahBooleanAtom::create (
      "ignore-lpsr-line-breaks", "ilplb",
R"(Suppress the line breaks from the LPSR data - let LilyPond decide about them,
and don't generate an empty \myBreak.)",
      "fIgnoreLpsrLineBreaks",
      fIgnoreLpsrLineBreaks);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreLpsrLineBreaksAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "break-lines-at-incomplete-right-measures", "blairm",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
        "fBreakLinesAtIncompleteRightMeasures",
        fBreakLinesAtIncompleteRightMeasures));

  fSeparatorLineEveryNMeasures = -1; // JMI

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "separator-line-every-n-measures", "slenm",
R"(Generate an additional separator line for readability every N measures,
where N is a positive integer.
Nothing special is done by default.)",
        "N",
        "fSeparatorLineEveryNMeasures",
        fSeparatorLineEveryNMeasures));

/* JMI
  subGroup->
    appendAtomToSubGroup (
      oahStringsSetElementAtom::create (
        "break-line-after-measure-number", "blamn",
R"(Generate a '\break' command after measure NUMBER in the LilyPond code.
NUMBER is a MusicXML measure number (a string), to be found in the latter.
This comes in handy when scanning several movements from a single PDF score.
There can be several occurrences of this option.)",
        "NUMBER",
        "fBreakLineAfterMeasureNumberSet",
        fBreakLineAfterMeasureNumberSet));
        */
}

void lpsr2lilypondOahGroup::initializeStavesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
       "help-lilypond-staves",  "hlps",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // tabs
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "modern-tab", "mtab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
        "fModernTab",
        fModernTab));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tab-full-notation", "tfn",
R"(Generate '\tabFullNotation' to obtain more complete tab notation,
including note stems.)",
        "fTabFullNotation",
        fTabFullNotation));

  // staves
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "keep-staff-size", "kss",
R"(Use the staff size value found in the MusicXML data.)",
        "fKeepStaffSize",
        fKeepStaffSize));

  // ledger lines
  // --------------------------------------

  fLedgerLinesRGBColor =
    msrRGBColor (0.0, 0.0, 0.0); // black by default

  fLedgerLinesRGBColorAtom =
    oahRGBColorAtom::create (
     "ledger-lines-color",  "llc",
R"(Use RGB_COLOR for the ledger lines.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "fLedgerLinesRGBColor",
      fLedgerLinesRGBColor);

  subGroup->
    appendAtomToSubGroup (
      fLedgerLinesRGBColorAtom);
}

void lpsr2lilypondOahGroup::initializeChordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "help-lilypond-chordss", "hlpc",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // arpeggios
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "connect-arpeggios", "conarp",
R"(Connect arpeggios across piano staves.)",
        "fConnectArpeggios",
        fConnectArpeggios));
}

void lpsr2lilypondOahGroup::initializeTupletsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "help-lilypond-tuplets", "hlpt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // tuplets
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "indent-tuplets", "itups",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on one and the same line.)",
        "fIndentTuplets",
        fIndentTuplets));
}

void lpsr2lilypondOahGroup::initializeRepeatOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "help-lilypond-repeats", "hlpr",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // repeats
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "keep-repeat-barlines", "krbls",
R"(Generate repeats start and and bar lines even though LilyPond would take care of them.)",
        "fKeepRepeatBarLines",
        fKeepRepeatBarLines));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "repeat-brackets", "rbracks",
R"(Generate repeats with brackets instead of regular bar lines for readibility.)",
        "fRepeatBrackets",
        fRepeatBrackets));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-repeat-numbers", "irn",
R"(Ignore repeats numbers and let LilyPond determine them.)",
        "fIgnoreRepeatNumbers",
        fIgnoreRepeatNumbers));
}

void lpsr2lilypondOahGroup::initializeOrnamentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "help-lilypond-ornaments", "hlporns",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // ornaments
  // --------------------------------------

  fDelayedOrnamentsFraction = Rational (1, 2);

  string delayedOrnamentsFractionDefaultValue =
    to_string (fDelayedOrnamentsFraction.getNumerator ()) +
      '/' +
    to_string (fDelayedOrnamentsFraction.getDenominator ());

  subGroup->
    appendAtomToSubGroup (
      oahRationalAtom::create (
        "delayed-ornaments-fraction", "dof",
        regex_replace (
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default is 'DEFAULT_VALUE'.)",
          regex ("DEFAULT_VALUE"),
          delayedOrnamentsFractionDefaultValue),
        "NUM/DENOM",
        "fDelayedOrnamentsFraction",
        fDelayedOrnamentsFraction));
}

void lpsr2lilypondOahGroup::initializeChordsDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords display",
        "help-chords-display", "hchd",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // chords
  // --------------------------------------

  // chords display

  subGroup->
    appendAtomToSubGroup (
      lilypondChordsDisplayAtom::create (
        "chords-display", "chd",
/*
Including a closing parenthesis in a raw string:
  choose ### as the delimiter so only )###" ends the string:
    R"###(
        Some Text)"
    )###";
*/
R"###(Use SPECIFICATION to display chords using LilyPond's chordNameExceptions.
SPECIFICATION should contain a chord contents such as '<c ees ges bes>',
followed by code to display it, for example:
  '<c ees ges bes> \\super {"-7(" {\\small\\raise #0.5 \\flat} "5)"}'.
The LilyPond code has to escape backslashes, thus use '\\' to obtain '\'.
These two elements are passed over to LilyPond verbatim, without any check.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "fChordsDisplayList",
        fChordsDisplayList));

  // jazz chords

  fJazzChordsDisplayLilypondCode =
R"###(  <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
  <c e g bes>1-\markup { \super "7" }
  <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
  <c f g bes>1-\markup { \super {"7(sus4)"} }
  <c e g a d'>1-\markup { \super "6/9" }
  <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
  <c f g bes d'>1-\markup { \super {"9(sus4)"} }
  <c e g bes d'>1-\markup { \super "9" }
  <c e g b d'>1-\markup { \super "maj9" }
  <c e gis bes d'>1-\markup { \super "9+" }
  <c e g bes d' fis'>1-\markup { \super "9#11" }
  <c e g bes d' f'>1-\markup { \super "11" }
  <c e g bes d' a'>1-\markup { \super "13" }
  <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
  <c e g a d'>1-\markup { \super "6(add9)" })###";

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jazz-chords-display", "jchd",
        regex_replace (
R"(Display the chords using LilyPond's chordNameExceptions
and a set of standard specifications to display them, i.e.:
LILYPOND_CODE)",
          regex ("LILYPOND_CODE"),
          fJazzChordsDisplayLilypondCode),
        "fJazzChordsDisplay",
        fJazzChordsDisplay));
}

void lpsr2lilypondOahGroup::initializeDynamicsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics",
        "help-dynamics", "hdynamics",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // dynamics text spanners style
  // --------------------------------------

  const lpsrDynamicsTextSpannersStyleKind
    lpsrDynamicsTextSpannersStyleKindDefaultValue =
      lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleDashedLine; // default value

  fDynamicsTextSpannersStyleKind = lpsrDynamicsTextSpannersStyleKindDefaultValue;

  fDynamicsTextSpannersStyleKindAtom =
    lilypondDynamicsTextSpannersStyleKindAtom::create (
       "lilypond-dynamics-text-spanners-style", "dtss", // JMI
        regex_replace (
          regex_replace (
            regex_replace (
R"(The NUMBER LilyPond dynamics text spanners styles available are:
LYRICS_DYNAMICS_TEXT_SPANNERS_STYLE_KINDS.
The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (getLpsrDynamicsTextSpannersStyleKindsMap ().size ())),
          regex ("LYRICS_DYNAMICS_TEXT_SPANNERS_STYLE_KINDS"),
          existingLpsrDynamicsTextSpannersStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        lpsrDynamicsTextSpannersStyleKindAsString (
          lpsrDynamicsTextSpannersStyleKindDefaultValue)),
      "STYLE",
      "fDynamicsTextSpannersStyleKind",
      fDynamicsTextSpannersStyleKind);


  subGroup->
    appendAtomToSubGroup (
      fDynamicsTextSpannersStyleKindAtom);
}

void lpsr2lilypondOahGroup::initializeLyricsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "help-lyrics", "hlyrics",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // add stanzas mumbers
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "add-lilypond-stanzas-numbers", "asn",
R"(Add stanzas numbers to lyrics in LilyPond.)",
        "fAddLilypondStanzasNumbers",
        fAddLilypondStanzasNumbers));

  // lyrics durations
  // --------------------------------------

  const lpsrLyricsDurationsKind
    lpsrLyricsDurationsKindDefaultValue =
      lpsrLyricsDurationsKind::kLyricsDurationsExplicit; // default value

  fLyricsDurationsKind = lpsrLyricsDurationsKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lilypondLyricsDurationsKindAtom::create (
        "lilypond-lyrics-durations-style", "ld", // JMI
          regex_replace (
            regex_replace (
              regex_replace (
R"(The NUMBER LilyPond lyrics durations STYLEs available are:
LYRICS_DURATIONS_KINDS.
Using 'implicit' prevents the creation of lyrics attached to rests by LilyPond,
use 'explicit' in such cases.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrLyricsDurationsKindsMap.size ())),
            regex ("LYRICS_DURATIONS_KINDS"),
            existingLpsrLyricsDurationsKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrLyricsDurationsKindAsString (
            lpsrLyricsDurationsKindDefaultValue)),
        "STYLE",
        "fLyricsDurationsKind",
        fLyricsDurationsKind));
}

void lpsr2lilypondOahGroup::initializeFontsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Fonts",
        "help-fonts", "hfonts",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // fonts
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "jazz-fonts", "jazzf",
R"(Use the LilyJazz font for notes, braces, text and chords.
It can be downloaded from https://github.com/OpenLilyPondFonts/lilyjazz.
This font should be installed so that LilyPond can use it, see:
https://github.com/OpenLilyPondFonts/lilyjazz/blob/master/LilyPond-Fonts-Installation-And-Usage.txt.)",
        "fJazzFonts",
        fJazzFonts));
}

void lpsr2lilypondOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "help-lilypond-code-generation", "hlpcg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // infos
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lilypond-generation-infos", "lpgi",
R"(Generate initial comments showing the LilyPond generation informations.)",
        "fXml2lyInfos", // JMI lilyGen ???
        fXml2lyInfos));

  // comments
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lilypond-comments", "lpcom",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
        "fLilypondComments",
        fLilypondComments));

  // global
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
        "fGlobal",
        fGlobal));

  // staff size
  // --------------------------------------

  fNoSetGlobalStaffSizeAtom =
    oahBooleanAtom::create (
      "no-set-global-staff-size", "nsgss",
R"(Don't generate #(set-global-staff-size ...).)",
      "fNoSetGlobalStaffSize",
      fNoSetGlobalStaffSize);

  subGroup->
    appendAtomToSubGroup (
      fNoSetGlobalStaffSizeAtom);

  // languages
  // --------------------------------------

  fUseLilypondDefaultLanguagesAtom =
    oahBooleanAtom::create (
      "use-lilypond-default-languages", "uldf",
R"(Don't generate a \book block.)",
      "fUseLilypondDefaultLanguages",
      fUseLilypondDefaultLanguages);

  subGroup->
    appendAtomToSubGroup (
      fUseLilypondDefaultLanguagesAtom);

  // book block
  // --------------------------------------

  fNoBookBlockAtom =
    oahBooleanAtom::create (
      "no-book-block", "nbb",
R"(Don't generate a \book block.)",
      "fNoBookBlock",
      fNoBookBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoBookBlockAtom);

  // header block
  // --------------------------------------

  fNoHeaderBlockAtom =
    oahBooleanAtom::create (
      "no-header-block", "nhb",
R"(Don't generate a \header block.)",
      "fNoHeaderBlock",
      fNoHeaderBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoHeaderBlockAtom);

  // layout block
  // --------------------------------------

  // layout score context
  fNoLayoutScoreContextAtom =
    oahBooleanAtom::create (
      "no-layout-score-context", "nlsc",
R"(Don't generate a \score context in the \layout block.)",
      "fNoLayoutScoreContext",
      fNoLayoutScoreContext);

  subGroup->
    appendAtomToSubGroup (
      fNoLayoutScoreContextAtom);

  // layout voice context
  fNoLayoutVoiceContextAtom =
    oahBooleanAtom::create (
        "no-layout-voice-context", "nlvc",
  R"(Don't generate a \voice context in the \layout block.)",
        "fNoLayoutVoiceContext",
        fNoLayoutVoiceContext);

  subGroup->
    appendAtomToSubGroup (
      fNoLayoutVoiceContextAtom);

  // create block layout block
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-top-level-layout-block", "ctllb",
  R"(Don't create a top-level \layout block.)",
        "fNoTopLevelLayoutBlock",
        fNoTopLevelLayoutBlock));

  // paper block
  // --------------------------------------

  fNoPaperBlockAtom =
    oahBooleanAtom::create (
      "no-paper-block", "npb",
R"(Don't generate a \paper block.)",
      "fNoPaperBlock",
      fNoPaperBlock);

  subGroup->
    appendAtomToSubGroup (
      fNoPaperBlockAtom);

  // display music
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-music", "dm",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
        "fDisplayMusic",
        fDisplayMusic));

  // LilyPond code
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
       "no-lilypond-code",  "nolpc",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
        "fNoLilypondCode",
        fNoLilypondCode));

  // LilyPond lyrics
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-lilypond-lyrics", "nolpl",
R"(Don't generate any lyrics in the LilyPond code.)",
        "fNoLilypondLyrics",
        fNoLilypondLyrics));

  // conversion date
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lilypond-conversion-date", "lpcd",
R"(Generate code to include the conversion date
when LilyPond creates the score.)",
        "fLilypondRunDate",
        fLilypondRunDate));

  // draft mode
  // --------------------------------------

  S_oahBooleanAtom
    draftOahBooleanAtom =
      oahBooleanAtom::create (
        "draft-mode", "draft",
R"(Generate a difficult to use score
to get only an overview in the resulting PDF file.)",
        "fDraftMode",
        fDraftMode);
  draftOahBooleanAtom->
    setElementVisibilityKind (
      oahElementVisibilityKind::kElementVisibilityHidden);

  subGroup->
    appendAtomToSubGroup (
      draftOahBooleanAtom);

  // point and click
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "point-and-clic-off", "paco",
R"(Generate \pointAndClickOff at the beginning of the LilyPond code
to reduce the size of the resulting PDF file.)",
        "fPointAndClickOff",
        fPointAndClickOff));

  // stems directions
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-stems-directions", "gsd",
R"(Generate \stem* commands in the LilyPond code.
The default is to leave it to LilyPond itself to choose the orientation of stems.)",
        "fGenerateStemsDirections",
        fGenerateStemsDirections));

  // white note heads
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "white-note-heads", "wnh",
R"(Generate Scheme function 'whiteNoteHeads'
at the beginning of the LilyPond code.)",
        "fWhiteNoteHeads",
        fWhiteNoteHeads));

  // generate commented out variables
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-commented-out-variables", "gcov",
R"(Generate LilyPond variables as comment,
to avoid having to add them by hand afterwards.)",
        "fGenerateCommentedOutVariables",
        fGenerateCommentedOutVariables));

  // generate MSR visiting information
  // --------------------------------------

//   subGroup->
//     appendAtomToSubGroup (
//       oahBooleanAtom::create (
//         "generate-msr-visiting-information", "gmvisi",
// R"(Generate MSR visiting information as comment..)",
//         "fGenerateMsrVisitingInformation",
//         fGenerateMsrVisitingInformation));

  // generate LPSR visiting information
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-lpsr-visiting-information", "glvisi",
R"(Generate LPSR visiting information as comment..)",
        "fGenerateLpsrVisitingInformation",
        fGenerateLpsrVisitingInformation));
}

void lpsr2lilypondOahGroup::initializeMidiOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Midi",
        "help-lilypond-midi", "hlpm",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // midiTempo
  // --------------------------------------

  string midiTempoDuration  = "8";
  int    midiTempoPerSecond = 180;

  fMidiTempo.setMidiTempoDuration (midiTempoDuration);
  fMidiTempo.setMidiTempoPerSecond (midiTempoPerSecond);

  string midiTempoDefaultValue =
    midiTempoDuration + " = " + to_string (midiTempoPerSecond);

  fMidiTempoAtom =
    oahMidiTempoAtom::create (
      "midi-tempo", "mdtempo",
      regex_replace (
        regex_replace (
R"(Generate a '\tempo' command in the \midi block.
MIDI_TEMPO_SPEC should be of the form DURATION=PER_SECOND .
DURATION is a string such as '8.', and PER_SECOND is an integer.
There can be spaces around the '='.
The default is 'DEFAULT_VALUE'.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        regex ("DEFAULT_VALUE"),
        midiTempoDefaultValue),
      "MIDI_TEMPO_SPEC",
      "fMidiTempo",
      fMidiTempo);

  subGroup->
    appendAtomToSubGroup (
      fMidiTempoAtom);

  // noMidi
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-midi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
        "fNoMidi",
        fNoMidi));
}

void lpsr2lilypondOahGroup::initializeCombinedOptionsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "help-musicxml-lilypond-combined-options", "hmxmlco",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // minimal
  // --------------------------------------

  // create the 'minimal' combined boolean atom
  S_oahCombinedBooleansAtom
    minimalCombinedBooleansAtom =
      oahCombinedBooleansAtom::create (
      "minimal", "",
R"(Avoids the generation of some LilyPond code, with is otherwise
meant to facilitate manual editing and completion of the result.)");

  subGroup->
    appendAtomToSubGroup (
      minimalCombinedBooleansAtom);

  // populate the 'minimal' combined boolean atom
  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoSetGlobalStaffSizeAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fUseLilypondDefaultLanguagesAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoBookBlockAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoHeaderBlockAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoLayoutScoreContextAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoLayoutVoiceContextAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoPaperBlockAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fNoBarNumberChecksAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreLpsrLineBreaksAtom);

  minimalCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreLpsrPageBreaksAtom);
}

void lpsr2lilypondOahGroup::initializeLilypondGenerationOahGroup ()
{
  // LilyPond version
  // --------------------------------------
  initializeLilypondVersionOptions ();

  // score notation
  // --------------------------------------
  initializeScoreNotationOptions ();

  // global staff size
  // --------------------------------------
  initializeGlobalStaffSizeOptions ();

  // identification
  // --------------------------------------
  initializeIdentificationOptions ();

  // parts
  // --------------------------------------
  initializePartsOptions ();

  // voices
  // --------------------------------------
  initializeVoicesOptions ();

  // engravers
  // --------------------------------------
  initializeEngraversOptions ();

  // time
  // --------------------------------------
  initializeClefsKeysTimesOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // bars
  // --------------------------------------
  initializeBarsOptions ();

  // page and line breaks
  // --------------------------------------
  initializePageAndLineBreaksOptions ();

  // staves
  // --------------------------------------
  initializeStavesOptions ();

  // chords
  // --------------------------------------
  initializeChordsOptions ();

  // tuplets
  // --------------------------------------
  initializeTupletsOptions ();

  // repeats
  // --------------------------------------
  initializeRepeatOptions ();

  // ornaments
  // --------------------------------------
  initializeOrnamentsOptions ();

  // chords display
  // --------------------------------------
  initializeChordsDisplayOptions ();

  // lyrics
  // --------------------------------------
  initializeLyricsOptions ();

  // dynamics
  // --------------------------------------
  initializeDynamicsOptions ();

  // fonts
  // --------------------------------------
  initializeFontsOptions ();

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();

  // midi
  // --------------------------------------
  initializeMidiOptions ();

  // combined options
  // --------------------------------------
  initializeCombinedOptionsOptions ();
}

/* JMI
Bool lpsr2lilypondOahGroup::setAccidentalStyleKind (lpsrAccidentalStyleKind accidentalStyleKind)
{
  // is accidentalStyleKind in the accidental styles set?
  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gGlobalLpsrAccidentalStyleKindsMap.find (accidentalStyleKind);

  if (it == gGlobalLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidentalStyleKind is unknown
    return false;
  }

  fAccidentalStyleKind = it.second;

  return true;
}
*/

//______________________________________________________________________________
void lpsr2lilypondOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void lpsr2lilypondOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsr2lilypondOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsr2lilypondOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsr2lilypondOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsr2lilypondOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void lpsr2lilypondOahGroup::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "The LilyPond lilypond are:" <<
    endl;

  ++gIndenter;

  // LilyPond version
  // --------------------------------------

  gLogStream <<
    "LilyPond version:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "lilyPondVersion" << " : " <<
    fLilypondVersion <<
    endl;

  --gIndenter;

  // identification
  // --------------------------------------
  os <<
    "Identification:" <<
    endl;

  ++gIndenter;

  // MusicXML informations
  os << left <<
    setw (valueFieldWidth) << "fRights" << " : " <<
      fRights <<
      endl <<

    setw (valueFieldWidth) << "fComposer" << " : " <<
      fComposer <<
      endl <<
    setw (valueFieldWidth) << "fArranger" << " : " <<
      fArranger <<
      endl <<
    setw (valueFieldWidth) << "fPoet" << " : " <<
      fPoet <<
      endl <<
    setw (valueFieldWidth) << "fLyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (valueFieldWidth) << "fSoftware" << " : " <<
      fSoftware <<
      endl;

  // LilyPond informations
  os << left <<
    setw (valueFieldWidth) << "fDedication" << " : " <<
      fDedication <<
      endl <<

    setw (valueFieldWidth) << "fPiece" << " : " <<
      fPiece <<
      endl <<
    setw (valueFieldWidth) << "fOpus" << " : " <<
      fOpus <<
      endl <<

    setw (valueFieldWidth) << "fTitle" << " : " <<
      fTitle <<
      endl <<
    setw (valueFieldWidth) << "fSubTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (valueFieldWidth) << "fSubSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<

    setw (valueFieldWidth) << "fHeaderInstrument" << " : " <<
      fHeaderInstrument <<
      endl <<

    setw (valueFieldWidth) << "fHeaderMeter" << " : " <<
      fHeaderMeter <<
      endl <<

    setw (valueFieldWidth) << "fTagline" << " : " <<
      fTagline <<
      endl <<

    setw (valueFieldWidth) << "fCopyright" << " : " <<
      fCopyright <<
      endl;

  --gIndenter;


  // parts
  // --------------------------------------

  // part names transposition

  gLogStream << left <<
    setw (valueFieldWidth) << "part names transposition" << " : ";

  if (! fPartNamesTranspositionMap.size ()) {
    gLogStream << "[NONE]";
  }
  else {
    for (
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
        fPartNamesTranspositionMap.begin ();
      i != fPartNamesTranspositionMap.end ();
      ++i
    ) {
      gLogStream <<
        "Part name \"" << ((*i).first) <<
        " = " <<
        ((*i).second->asString ()) <<
        "\" ";
    } // for
  }

  // part IDs transposition

  gLogStream << left <<
    setw (valueFieldWidth) << "fPartIDsTranspositionMap" << " : ";

  if (! fPartIDsTranspositionMap.size ()) {
    gLogStream << "[EMPTY]";
  }
  else {
    for (
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
        fPartIDsTranspositionMap.begin ();
      i != fPartIDsTranspositionMap.end ();
      ++i
    ) {
      gLogStream <<
        "Part ID \"" << ((*i).first) <<
        " = " <<
        ((*i).second->asString ()) <<
        "\" ";
    } // for
  }

  // engravers
  // --------------------------------------
  os <<
    "Engravers:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fAmbitusEngraver" << " : " <<
      fAmbitusEngraver <<
      endl <<
    setw (valueFieldWidth) << "fCustosEngraver" << " : " <<
      fCustosEngraver <<
      endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  os <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fCommentClefChanges" << " : " <<
      fCommentClefChanges <<
      endl;

  --gIndenter;


  // keys
  // --------------------------------------
  os <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fCommentClefChanges" << " : " <<
      fCommentClefChanges <<
      endl;

  --gIndenter;


  // time signatures
  // --------------------------------------
  os <<
    "Times:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fNumericalTimeSignature" << " : " <<
      fNumericalTimeSignature <<
      endl;

  --gIndenter;


  // notes
  // --------------------------------------
  os <<
    "Notes:" <<
    endl;

  ++gIndenter;

  msrOctaveEntryKind
    octaveEntryKind =
      fetchOctaveEntryVariableValue ();

  os << left <<
    setw (valueFieldWidth) << "octaveEntryKind" << " : " <<
    msrOctaveEntryKindAsString (
      octaveEntryKind) <<
    endl;

  switch (octaveEntryKind) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      if (fRelativeOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) <<
          "relativeOctaveEntrySemiTonesPitchAndOctave" << " : ";
/* JMI
        if (fSetByAnOption) {
          os <<
            ", fSetByAnOption: " <<
            fSetByAnOption;
        }
*/
        os <<
          fRelativeOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      if (fFixedOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) <<
          "fixedOctaveEntrySemiTonesPitchAndOctave" << " : ";
/* JMI
        if (fSetByAnOption) {
          os <<
            ", fSetByAnOption: " <<
            fSetByAnOption;
        }
*/
        os <<
          fFixedOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
  } // switch

  os << left <<
    setw (valueFieldWidth) << "fInputLineNumbers" << " : " <<
      fAllDurations <<
      endl <<

    setw (valueFieldWidth) << "fNoAutoBeaming" << " : " <<
      fNoAutoBeaming <<
      endl <<
    setw (valueFieldWidth) << "fNoBeams" << " : " <<
      fNoBeams <<
      endl <<

    setw (valueFieldWidth) << "fRomanStringNumbers" << " : " <<
      fRomanStringNumbers <<
      endl <<
    setw (valueFieldWidth) << "fAvoidOpenStrings" << " : " <<
      fAvoidOpenStrings <<
      endl <<

    setw (valueFieldWidth) << "fAccidentalStyleKind" << " : " <<
      lpsrAccidentalStyleKindAsString (fAccidentalStyleKind) <<
      endl <<

    setw (valueFieldWidth) << "fLyricsDurationsKind" << " : " <<
      lpsrLyricsDurationsKindAsString (fLyricsDurationsKind) <<
      endl <<

    setw (valueFieldWidth) << "fCompressFullBarRestsInLilypond" << " : " <<
      fCompressFullBarRestsInLilypond <<
      endl <<

    setw (valueFieldWidth) << "fMergeStaffCommonRests" << " : " <<
      fMergeStaffCommonRests <<
      endl <<

    setw (valueFieldWidth) << "fMultipleFullBarRestsExpandLimit" << " : " <<
      fMultipleFullBarRestsExpandLimit <<
      endl <<

    setw (valueFieldWidth) << "fInputLineNumbers" << " : " <<
      fInputLineNumbers <<
      endl <<
    setw (valueFieldWidth) << "fNotesComments" << " : " <<
      fNotesComments <<
      endl <<

    setw (valueFieldWidth) << "fOriginalMeasureNumbers" << " : " <<
      fOriginalMeasureNumbers <<
      endl <<

    setw (valueFieldWidth) << "fGenerateMeasurePositions" << " : " <<
      fGenerateMeasurePositions <<
      endl <<

    setw (valueFieldWidth) << "fGenerateMomentsInMeasures" << " : " <<
      fGenerateMomentsInMeasures <<
      endl <<

    setw (valueFieldWidth) << "fNonPrintNotesHeadRGBColor" << " : " <<
      fNonPrintNotesHeadRGBColor.asString () <<
      endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  os <<
    "Bars:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fShowAllBarNumbers" << " : " <<
    fShowAllBarNumbers <<
    endl <<
    setw (valueFieldWidth) << "fBoxAroundAllBarNumbers" << " : " <<
    fBoxAroundAllBarNumbers <<
    endl;

  os << left <<
    setw (valueFieldWidth) <<
    fetchNamesBetweenQuotes () <<
    " :";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    os << " empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << endl;

  os << left <<
    setw (valueFieldWidth) << "boxAroundBarNumberSet" << " : ";
  if (! fBoxAroundBarNumberSet.size ()) {
    os << "[EMPTY]";
  }
  else {
    set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  os <<
    "Line breaks:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fIgnoreLpsrLineBreaks" << " : " <<
      fIgnoreLpsrLineBreaks <<
      endl <<

    setw (valueFieldWidth) << "fBreakLinesAtIncompleteRightMeasures" << " : " <<
      fBreakLinesAtIncompleteRightMeasures <<
      endl <<

    setw (valueFieldWidth) << "fSeparatorLineEveryNMeasures" << " : " <<
      fSeparatorLineEveryNMeasures <<
      endl;

  --gIndenter;


  // page and line breaks
  // --------------------------------------
  os <<
    "Page breaks:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fIgnoreLpsrPageBreaks" << " : " <<
    fIgnoreLpsrPageBreaks <<
    endl;

  --gIndenter;


  // staves
  // --------------------------------------
  os <<
    "Staves:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fModernTab" << " : " <<
    fModernTab <<
    endl <<
    setw (valueFieldWidth) << "fTabFullNotation" << " : " <<
    fTabFullNotation <<
    endl <<

    setw (valueFieldWidth) << "fKeepStaffSize" << " : " <<
    fKeepStaffSize <<
    endl;

  --gIndenter;


  // chords
  // --------------------------------------

  os <<
    "Chords:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fConnectArpeggios" << " : " <<
    fConnectArpeggios <<
    endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  os <<
    "Tuplets:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fIndentTuplets" << " : " <<
    fIndentTuplets <<
    endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  os <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fKeepRepeatBarLines" << " : " <<
    fKeepRepeatBarLines <<
    endl <<
    setw (valueFieldWidth) << "fRepeatBrackets" << " : " <<
    fRepeatBrackets <<
    endl <<
    setw (valueFieldWidth) << "fIgnoreRepeatNumbers" << " : " <<
    fIgnoreRepeatNumbers <<
    endl;

  --gIndenter;


  // lyrics
  // --------------------------------------

  gLogStream <<
    "Lyrics:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (valueFieldWidth) << "fAddLilypondStanzasNumbers" << " : " <<
    fAddLilypondStanzasNumbers <<
    endl;

  --gIndenter;

  // ornaments
  // --------------------------------------

  os <<
    "Ornaments:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  os <<
    "Chords display:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "chordsDisplayList" << " : ";

  if (fChordsDisplayList.size ()) {
    os << endl;

    ++gIndenter;

    list<pair<string, string> >::const_iterator
      iBegin = fChordsDisplayList.begin (),
      iEnd   = fChordsDisplayList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        gTab <<
        (*i).first <<
        " -> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
  //     os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[NONE]";
  }
  os << endl;

  os << left <<
    setw (valueFieldWidth) << "fJazzChordsDisplay" << " : " <<
    fJazzChordsDisplay <<
    endl <<
    setw (valueFieldWidth) << "fJazzChordsDisplayLilypondCode" << " : " <<
    fJazzChordsDisplayLilypondCode <<
    endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  os <<
    "Fonts:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fJazzFonts" << " : " <<
    fJazzFonts <<
    endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  os <<
    "LilyPond code generation:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fXml2lyInfos" << " : " <<
      fXml2lyInfos <<
      endl <<

    setw (valueFieldWidth) << "fLilypondComments" << " : " <<
      fLilypondComments <<
      endl <<

    setw (valueFieldWidth) << "fGlobal" << " : " <<
      fGlobal <<
      endl <<

    setw (valueFieldWidth) << "fNoPaperBlock" << " : " <<
      fNoPaperBlock <<
      endl <<

    setw (valueFieldWidth) << "fDisplayMusic" << " : " <<
      fDisplayMusic <<
      endl <<

    setw (valueFieldWidth) << "fNoLilypondCode" << " : " <<
      fNoLilypondCode <<
      endl <<

    setw (valueFieldWidth) << "fNoLilypondLyrics" << " : " <<
      fNoLilypondLyrics <<
      endl <<

    setw (valueFieldWidth) << "fLilypondRunDate" << " : " <<
      fLilypondRunDate <<
      endl <<

    setw (valueFieldWidth) << "fDraftMode" << " : " <<
      fDraftMode <<
      endl <<

    setw (valueFieldWidth) << "fPointAndClickOff" << " : " <<
      fPointAndClickOff <<
      endl <<

    setw (valueFieldWidth) << "fGenerateStemsDirections" << " : " <<
      fGenerateStemsDirections <<
      endl <<

    setw (valueFieldWidth) << "fWhiteNoteHeads" << " : " <<
      fWhiteNoteHeads <<
      endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  os <<
    "Score notation:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fJianpu" << " : " <<
      fJianpu <<
      endl;

  --gIndenter;


  // midi
  // --------------------------------------
  os <<
    "Midi:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (valueFieldWidth) << "fMidiTempo" << " : " <<
      fMidiTempo.asString () <<
      endl <<

    setw (valueFieldWidth) << "fNoMidi" << " : " <<
      fNoMidi <<
      endl;

  --gIndenter;

  --gIndenter;
}

void lpsr2lilypondOahGroup::printLilypondGenerationOahValues (int fieldWidth)
{
  gLogStream <<
    "The LilyPond lilypond are:" <<
    endl;

  ++gIndenter;

  // global staff size
  // --------------------------------------

  gLogStream <<
    "Global staff size:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fGlobalStaffSize" << " : " <<
    fGlobalStaffSize <<
    endl <<
    setw (fieldWidth) << "fStaffGlobalSizeDefaultValue" << " : " <<
    fStaffGlobalSizeDefaultValue <<
    endl;

  --gIndenter;

  // identification
  // --------------------------------------
  gLogStream <<
    "Identification:" <<
    endl;

  ++gIndenter;

  // MusicXML informations
  gLogStream << left <<
    setw (fieldWidth) << "fRights" << " : " <<
      fRights <<
      endl <<

    setw (fieldWidth) << "fComposer" << " : " <<
      fComposer <<
      endl <<
    setw (fieldWidth) << "fArranger" << " : " <<
      fArranger <<
      endl <<
    setw (fieldWidth) << "fPoet" << " : " <<
      fPoet <<
      endl <<
    setw (fieldWidth) << "fLyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (fieldWidth) << "fSoftware" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations
    setw (fieldWidth) << "fDedication" << " : " <<
      fDedication <<
      endl <<

    setw (fieldWidth) << "fPiece" << " : " <<
      fPiece <<
      endl <<
    setw (fieldWidth) << "fOpus" << " : " <<
      fOpus <<
      endl <<

    setw (fieldWidth) << "fTitle" << " : " <<
      fTitle <<
      endl <<
    setw (fieldWidth) << "fSubTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (fieldWidth) << "fSubSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<
    setw (fieldWidth) << "fHeaderInstrument" << " : " <<
      fHeaderInstrument <<
      endl <<
    setw (fieldWidth) << "fHeaderMeter" << " : " <<
      fHeaderMeter <<
      endl <<

    setw (fieldWidth) << "fTagline" << " : " <<
      fTagline <<
      endl <<

    setw (fieldWidth) << "fCopyright" << " : " <<
      fCopyright <<
      endl;

  --gIndenter;


  // engravers
  // --------------------------------------
  gLogStream <<
    "Engravers:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fAmbitusEngraver" << " : " <<
      fAmbitusEngraver <<
      endl <<
    setw (fieldWidth) << "fCustosEngraver" << " : " <<
      fCustosEngraver <<
      endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  gLogStream <<
    "Clefs:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fNoInitialTrebleClef" << " : " <<
      fNoInitialTrebleClef <<
      endl <<
    setw (fieldWidth) << "fCommentClefChanges" << " : " <<
      fCommentClefChanges <<
      endl;

  --gIndenter;


  // keys
  // --------------------------------------
  gLogStream <<
    "Keys:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fNoInitialCMajorKey" << " : " <<
      fNoInitialCMajorKey <<
      endl;

  --gIndenter;


  // time
  // --------------------------------------
  gLogStream <<
    "Times:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fNoInitialCommonTimeSignature" << " : " <<
      fNoInitialCommonTimeSignature <<
      endl <<
    setw (fieldWidth) << "fNumericalTimeSignature" << " : " <<
      fNumericalTimeSignature <<
      endl;

  --gIndenter;


  // notes
  // --------------------------------------
  gLogStream <<
    "Notes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fOctaveEntryVariable" << " : " <<
      msrOctaveEntryKindAsString (
        fOctaveEntryVariable.getOctaveEntryKind ()) <<
      endl <<

    setw (fieldWidth) << "fAllDurations" << " : " <<
      fAllDurations <<
      endl <<

    setw (fieldWidth) << "fNoAutoBeaming" << " : " <<
      fNoAutoBeaming <<
      endl <<
    setw (fieldWidth) << "fNoBeams" << " : " <<
      fNoBeams <<
      endl <<

    setw (fieldWidth) << "fRomanStringNumbers" << " : " <<
      fRomanStringNumbers <<
      endl <<
    setw (fieldWidth) << "fAvoidOpenStrings" << " : " <<
      fAvoidOpenStrings <<
      endl <<

    setw (fieldWidth) << "fAccidentalStyleKind" << " : " <<
      lpsrAccidentalStyleKindAsString (fAccidentalStyleKind) <<
      endl <<

    setw (fieldWidth) << "fLyricsDurationsKind" << " : " <<
      lpsrLyricsDurationsKindAsString (fLyricsDurationsKind) <<
      endl <<

    setw (fieldWidth) << "fCompressFullBarRestsInLilypond" << " : " <<
      fCompressFullBarRestsInLilypond <<
      endl <<

    setw (fieldWidth) << "fMergeStaffCommonRests" << " : " <<
      fMergeStaffCommonRests <<
      endl <<

    setw (fieldWidth) << "fMultipleFullBarRestsExpandLimit" << " : " <<
      fMultipleFullBarRestsExpandLimit <<
      endl <<

    setw (fieldWidth) << "fInputLineNumbers" << " : " <<
      fInputLineNumbers <<
      endl <<
    setw (fieldWidth) << "fNotesComments" << " : " <<
      fNotesComments <<
      endl <<

    setw (fieldWidth) << "fOriginalMeasureNumbers" << " : " <<
      fOriginalMeasureNumbers <<
      endl <<

    setw (fieldWidth) << "fGenerateMeasurePositions" << " : " <<
      fGenerateMeasurePositions <<
      endl <<

    setw (fieldWidth) << "fGenerateMomentsInMeasures" << " : " <<
      fGenerateMomentsInMeasures <<
      endl <<

    setw (fieldWidth) << "fNonPrintNotesHeadRGBColor" << " : " <<
      fNonPrintNotesHeadRGBColor.asString () <<
      endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  gLogStream <<
    "Bars:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fShowAllBarNumbers" << " : " <<
    fShowAllBarNumbers <<
    endl <<
    setw (fieldWidth) << "fBoxAroundAllBarNumbers" << " : " <<
    fBoxAroundAllBarNumbers <<
    endl;

  gLogStream << left <<
    setw (fieldWidth) << "fResetMeasureElementMeasureNumberMap" << " : ";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    gLogStream << "[EMPTY]";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  gLogStream << left <<
    setw (fieldWidth) << "fBoxAroundBarNumberSet" << " : ";
  if (! fBoxAroundBarNumberSet.size ()) {
    gLogStream << "[EMPTY]";
  }
  else {
    set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  gLogStream <<
    "Line breaks:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fIgnoreLpsrLineBreaks" << " : " <<
      fIgnoreLpsrLineBreaks <<
      endl <<

    setw (fieldWidth) << "fBreakLinesAtIncompleteRightMeasures" << " : " <<
      fBreakLinesAtIncompleteRightMeasures <<
      endl <<

    setw (fieldWidth) << "fSeparatorLineEveryNMeasures" << " : " <<
      fSeparatorLineEveryNMeasures <<
      endl;

  --gIndenter;


  // page and line breaks
  // --------------------------------------
  gLogStream <<
    "Page breaks:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fIgnoreLpsrPageBreaks" << " : " <<
    fIgnoreLpsrPageBreaks <<
    endl;

  --gIndenter;


  // staves
  // --------------------------------------
  gLogStream <<
    "Staves:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fModernTab" << " : " <<
    fModernTab <<
    endl <<
    setw (fieldWidth) << "fTabFullNotation" << " : " <<
    fTabFullNotation <<
    endl;

  --gIndenter;


  // chords
  // --------------------------------------

  gLogStream <<
    "Chords:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fConnectArpeggios" << " : " <<
    fConnectArpeggios <<
    endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  gLogStream <<
    "Tuplets:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fIndentTuplets" << " : " <<
    fIndentTuplets <<
    endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  gLogStream <<
    "Repeats:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fKeepRepeatBarLines" << " : " <<
    fKeepRepeatBarLines <<
    endl <<
    setw (fieldWidth) << "fRepeatBrackets" << " : " <<
    fRepeatBrackets <<
    endl <<
    setw (fieldWidth) << "fIgnoreRepeatNumbers" << " : " <<
    fIgnoreRepeatNumbers <<
    endl;

  --gIndenter;


  // ornaments
  // --------------------------------------

  gLogStream <<
    "Ornaments:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  gLogStream <<
    "Chords display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fJazzChordsDisplay" << " : " <<
    fJazzChordsDisplay <<
    endl <<
    setw (fieldWidth) << "fJazzChordsDisplayLilypondCode" << " : " <<
    fJazzChordsDisplayLilypondCode <<
    endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  gLogStream <<
    "Fonts:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fJazzFonts" << " : " <<
    fJazzFonts <<
    endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  gLogStream <<
    "LilyPond code generation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fXml2lyInfos" << " : " <<
      fXml2lyInfos <<
      endl <<

    setw (fieldWidth) << "fLilypondComments" << " : " <<
      fLilypondComments <<
      endl <<

    setw (fieldWidth) << "fGlobal" << " : " <<
      fGlobal <<
      endl <<

    setw (fieldWidth) << "fDisplayMusic" << " : " <<
      fDisplayMusic <<
      endl <<

    setw (fieldWidth) << "fNoLilypondCode" << " : " <<
      fNoLilypondCode <<
      endl <<

    setw (fieldWidth) << "fNoLilypondLyrics" << " : " <<
      fNoLilypondLyrics <<
      endl <<

    setw (fieldWidth) << "fLilypondRunDate" << " : " <<
      fLilypondRunDate <<
      endl <<

    setw (fieldWidth) << "fDraftMode" << " : " <<
      fDraftMode <<
      endl <<

    setw (fieldWidth) << "fPointAndClickOff" << " : " <<
      fPointAndClickOff <<
      endl <<

    setw (fieldWidth) << "fGenerateStemsDirections" << " : " <<
      fGenerateStemsDirections <<
      endl <<

    setw (fieldWidth) << "fWhiteNoteHeads" << " : " <<
      fWhiteNoteHeads <<
      endl <<

    setw (fieldWidth) << "fGenerateCommentedOutVariables" << " : " <<
      fGenerateCommentedOutVariables <<
      endl <<

//     setw (fieldWidth) << "fGenerateMsrVisitingInformation" << " : " <<
//       fGenerateMsrVisitingInformation <<
//       endl <<

    setw (fieldWidth) << "fGenerateLpsrVisitingInformation" << " : " <<
      fGenerateLpsrVisitingInformation <<
      endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  gLogStream <<
    "Score notation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fJianpu" << " : " <<
      fJianpu <<
      endl;

  --gIndenter;


  // midi
  // --------------------------------------
  gLogStream <<
    "Midi:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fMidiTempo" << " : " <<
      fMidiTempo.asString () <<
      endl <<

    setw (fieldWidth) << "fNoMidi" << " : " <<
      fNoMidi <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_lpsr2lilypondOahGroup& elt)
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
S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    gLogStream <<
      "Initializing LilyPond OAH handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalLpsr2lilypondOahGroup) {
    // create the global OAH group
    gGlobalLpsr2lilypondOahGroup =
      lpsr2lilypondOahGroup::create ();
    assert (gGlobalLpsr2lilypondOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLpsr2lilypondOahGroup;
}


}


/*
//______________________________________________________________________________
S_lilypondBreakPageAfterMeasureNumberAtom lilypondBreakPageAfterMeasureNumberAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable)
{
  lilypondBreakPageAfterMeasureNumberAtom* o = new
    lilypondBreakPageAfterMeasureNumberAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      fStringSetVariable);
  assert (o != nullptr);
  return o;
}

lilypondBreakPageAfterMeasureNumberAtom::lilypondBreakPageAfterMeasureNumberAtom (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
  set<string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      fStringSetVariable)
{}

lilypondBreakPageAfterMeasureNumberAtom::~lilypondBreakPageAfterMeasureNumberAtom ()
{}

void lilypondBreakPageAfterMeasureNumberAtom::applyAtomWithValue (
  const string& theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // musicxmlMeasureNumber
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // lilypondMeasureNumber
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-breakPageAfterMeasureNumber argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string musicxmlMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os <<
      "musicxmlMeasureNumber  = " <<
      musicxmlMeasureNumber <<
      endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;

  fStringSetVariable [musicxmlMeasureNumber] = lilypondMeasureNumber;
  }
#endif
}

void lilypondBreakPageAfterMeasureNumberAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondBreakPageAfterMeasureNumberAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
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

void lilypondBreakPageAfterMeasureNumberAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, int>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        ", fSetByAnOption: " <<
        fSetByAnOption <<
        endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

ostream& operator << (ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}
*/

