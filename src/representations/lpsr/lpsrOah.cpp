/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...
#include <string>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPitchesLanguageAtom lpsrPitchesLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    lpsrPitchesLanguageKindVariable)
{
  lpsrPitchesLanguageAtom* o = new
    lpsrPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrPitchesLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

lpsrPitchesLanguageAtom::lpsrPitchesLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    lpsrPitchesLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fMsrQuarterTonesPitchesLanguageKindVariable (
      lpsrPitchesLanguageKindVariable)
{}

lpsrPitchesLanguageAtom::~lpsrPitchesLanguageAtom ()
{}

void lpsrPitchesLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'" <<
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
      "LPSR pitches language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known LPSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);
}

void lpsrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrPitchesLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrPitchesLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

string lpsrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);

  return s.str ();
}

void lpsrPitchesLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrPitchesLanguageAtom:" <<
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

void lpsrPitchesLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fMsrQuarterTonesPitchesLanguageKindVariable);
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrChordsLanguageAtom lpsrChordsLanguageAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrChordsLanguageKind&
                    lpsrChordsLanguageKindVariable)
{
  lpsrChordsLanguageAtom* o = new
    lpsrChordsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrChordsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

lpsrChordsLanguageAtom::lpsrChordsLanguageAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  lpsrChordsLanguageKind&
                    lpsrChordsLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrChordsLanguageKindVariable (
      lpsrChordsLanguageKindVariable)
{}

lpsrChordsLanguageAtom::~lpsrChordsLanguageAtom ()
{}

void lpsrChordsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'" <<
      endl;
  }
#endif

  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gGlobalLpsrChordsLanguageKindsMap.find (theString);

  if (it == gGlobalLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    stringstream s;

    s <<
      "LPSR chords language '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalLpsrChordsLanguageKindsMap.size () - 1 <<
      " known LPSR chords languages apart from the default Ignatzek are:" <<
      endl;

    ++gIndenter;

    s <<
      existingLpsrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLpsrChordsLanguageKindVariable (
    (*it).second);
}

void lpsrChordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrChordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrChordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrChordsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrChordsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

string lpsrChordsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return s.str ();
}

void lpsrChordsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrChordsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fLpsrChordsLanguageKindVariable" << " : " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    endl;

  --gIndenter;
}

void lpsrChordsLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable);
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrTransposeAtom lpsrTransposeAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  S_msrSemiTonesPitchAndOctave&
                    semiTonesPitchAndOctaveVariable)
{
  lpsrTransposeAtom* o = new
    lpsrTransposeAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      semiTonesPitchAndOctaveVariable);
  assert (o != nullptr);
  return o;
}

lpsrTransposeAtom::lpsrTransposeAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  S_msrSemiTonesPitchAndOctave&
                    semiTonesPitchAndOctaveVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fSemiTonesPitchAndOctaveVariable (
      semiTonesPitchAndOctaveVariable)
{}

lpsrTransposeAtom::~lpsrTransposeAtom ()
{}

void lpsrTransposeAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'lpsrTransposeAtom'" <<
      endl;
  }
#endif

  // create the semitones pitch and octave from theString
  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave =
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        theString);

  // set the transpose atom variable value
  setSemiTonesPitchAndOctaveVariable (
    semiTonesPitchAndOctave);
}

void lpsrTransposeAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrTransposeAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrTransposeAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrTransposeAtom::browseData ()" <<
      endl;
  }
#endif
}

string lpsrTransposeAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

string lpsrTransposeAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return s.str ();
}

void lpsrTransposeAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrTransposeAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void lpsrTransposeAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";
  if (fSemiTonesPitchAndOctaveVariable) {
    os << endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable <<
      ", fSetByUser: " <<
      fSetByUser;
    --gIndenter;
  }
  else {
    os <<
      "empty" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lpsrOahGroup gGlobalLpsrOahGroup;

S_lpsrOahGroup lpsrOahGroup::create ()
{
  lpsrOahGroup* o = new lpsrOahGroup ();
  assert (o != nullptr);
  return o;
}

lpsrOahGroup::lpsrOahGroup ()
  : oahGroup (
    "LPSR",
    "help-lpsr", "hlpsr",
R"(These options control the way LPSR data is handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeLpsrOahGroup ();
}

lpsrOahGroup::~lpsrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void lpsrOahGroup::initializeLpsrtracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LPSR Trace",
        "help-lpsr-trace", "hlpsrtrace",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace- lpsr

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-lpsr", "tlpsr",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "fTraceLpsr",
        fTraceLpsr));

  // trace lilypond version

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-lilypond-version", "tlpv",
R"(Write a trace of the LilyPond version to standard error.)",
        "fTraceLilypondVersion",
        fTraceLilypondVersion));

  // trace lpsr visitors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-lpsr-visitors", "tlpsrvis",
R"(Write a trace of the LPSR graphs visiting activity to standard error.)",
        "fTraceLpsrVisitors",
        fTraceLpsrVisitors));

  // trace lpsr blocks

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-lpsr-blocks", "tlpsrb",
R"(Write a trace of the LPSR blocks to standard error.)",
        "fTraceLpsrBlocks",
        fTraceLpsrBlocks));

  // trace scheme functions

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-scheme-functions", "tsf",
R"(Write a trace of the activity regarding Scheme functions to standard error.)",
        "fTraceSchemeFunctions",
        fTraceSchemeFunctions));
}
#endif

void lpsrOahGroup::initializeLpsrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "help-lpsr-display", "hlpsrd",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display LPSR

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr", "dlpsr",
R"(Write the contents of the LPSR data with a summary of it MSR component to standard error.)",
        "fDisplayLpsr",
        fDisplayLpsr));

  // display LPSR full

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-full", "dlpsrfull",
R"(Write the contents of the LPSR data with its full MSR component to standard error.)",
        "fDisplayLpsrFull",
        fDisplayLpsrFull));

  // display LPSR short

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-short", "dlpsrshort",
R"(Write the contents of the LPSR data, short version, to standard error.)",
        "fDisplayLpsrShort",
        fDisplayLpsrShort));
}

void lpsrOahGroup::initializeLpsrPaperOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "help-lpsr-paper", "hlpsrpaper",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // all paper variables

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         "all-paper-variables", "apv",
R"(Generate LilyPond comments containing paper variables
that are not present in the MusicXML data nor specified in options.
Default values are supplied.
This helps the user adapting the generated code for their needs.)",
        "fAllPaperVariables",
        fAllPaperVariables));

  // length unit kind

  fLengthUnitKindDefaultValue = msrLengthUnitKind::kUnitMillimeter; // default value;
  fLengthUnitKind = fLengthUnitKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      oahLengthUnitKindAtom::create (
        "length-unit", "",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Set the LilyPond length variables unit to UNIT in the forthcoming options.
The NUMBER LilyPond length variables units available are:
LENTGTH_UNITS.
This option should precede options that set paper size and indents
if they don't specify a unit.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsrLengthUnitKindsMap.size ())),
            regex ("LENTGTH_UNITS"),
            gIndenter.indentMultiLineString (
              existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrLengthUnitKindAsString (
            fLengthUnitKindDefaultValue)),
        "UNIT",
        "fLengthUnitKind",
        fLengthUnitKind));

  // paper height

  fPaperHeight.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperHeight.setLengthValue (297);

  fPaperHeightAtom =
    oahLengthAtom::create (
      "paper-height", "",
R"(Set the LilyPond 'paper-height' paper variable to HEIGHT in the LilyPond code.
HEIGHT should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, LilyPond uses 297 mm (A4 format).)",
      "HEIGHT",
      "fPaperHeight",
      fPaperHeight);
  subGroup->
    appendAtomToSubGroup (
      fPaperHeightAtom);

  // paper width

  fPaperWidth.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperWidth.setLengthValue (210);

  fPaperWidthAtom =
    oahLengthAtom::create (
      "paper-width", "",
R"(Set the LilyPond 'paper-width' paper variable to WIDTH in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, LilyPond uses 210 mm (A4 format).)",
      "WIDTH",
      "fPaperWidth",
      fPaperWidth);
  subGroup->
    appendAtomToSubGroup (
      fPaperWidthAtom);

  // left margin

  fPaperLeftMargin.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperLeftMargin.setLengthValue (15);

  fPaperLeftMarginAtom =
    oahLengthAtom::create (
      "left-margin", "",
R"(Set the LilyPond 'left-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "fPaperLeftMargin",
      fPaperLeftMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperLeftMarginAtom);

  // right margin

  fPaperRightMargin.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperRightMargin.setLengthValue (15);

  fPaperRightMarginAtom =
    oahLengthAtom::create (
      "right-margin", "",
R"(Set the LilyPond 'right-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "fPaperRightMargin",
      fPaperRightMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperRightMarginAtom);

  // top margin

  fPaperTopMargin.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperTopMargin.setLengthValue (15);

  fPaperTopMarginAtom =
    oahLengthAtom::create (
      "top-margin", "",
R"(Set the LilyPond 'top-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "fPaperTopMargin",
      fPaperTopMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperTopMarginAtom);

  // bottom margin

  fPaperBottomMargin.setLengthUnitKind (msrLengthUnitKind::kUnitMillimeter);
  fPaperBottomMargin.setLengthValue (15);

  fPaperBottomMarginAtom =
    oahLengthAtom::create (
      "bottom-margin", "",
R"(Set the LilyPond 'bottom-margin' paper variable to MARGIN in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default, this is left to LilyPond'.)",
      "MARGIN",
      "fPaperBottomMargin",
      fPaperBottomMargin);
  subGroup->
    appendAtomToSubGroup (
      fPaperBottomMarginAtom);

/* JMI superflous
  // a4

   subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "a4", "",
R"(Set the LilyPond 'paper-width' and 'paper-height' paper variables
to 210 mm  and 297 mm respectively in the LilyPond code.
WIDTH should be a positive floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
By default is 297 mm (A4 format).)",
        "WIDTH",
        "fPaperWidth",
        fPaperWidth));
*/

  // fPaperHorizontalShift is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "horizontal-shift", "hshift",
R"(Set the LilyPond 'horizontal-shift' paper variable to SHIFT in the LilyPond code.
SHIFT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "SHIFT",
        "fPaperHorizontalShift",
        fPaperHorizontalShift));

  // fPaperIndent is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "indent", "",
R"(Set the LilyPond 'indent' paper variable to INDENT in the LilyPond code.
INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "INDENT",
        "fPaperIndent",
        fPaperIndent));

  // short indent

  // fPaperShortIndent is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "short-indent", "",
R"(Set the LilyPond 'short-indent' paper variable to SHORT_INDENT in the LilyPond code.
SHORT_INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
        "SHORT_INDENT",
        "fPaperShortIndent",
        fPaperShortIndent));

  // fMarkupSystemSpacingPadding is 0.0 mm by default

  subGroup->
    appendAtomToSubGroup (
      oahLengthAtom::create (
        "markup-system-spacing-padding", "mssp",
R"(Set the LilyPond 'markup-system-spacing.padding' paper variable
to PADDING in the LilyPond code.
PADDING should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
LilyPond's default value is 0.0 mm.)",
        "INDENT",
        "fMarkupSystemSpacingPadding",
        fMarkupSystemSpacingPadding));

  // ragged bottom

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-bottom", "",
R"(Set the LilyPond 'ragged-bottom' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "fRaggedBottom",
        fRaggedBottom));

  // ragged last

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-last", "",
R"(Set the LilyPond 'ragged-last' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "fRaggedLast",
        fRaggedLast));

  // ragged last bottom

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-last-bottom", "",
R"(Set the LilyPond 'ragged-last-bottom' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "fRaggedLastBottom",
        fRaggedLastBottom));

  // ragged right

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ragged-right", "",
R"(Set the LilyPond 'ragged-right' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "fRaggedRight",
        fRaggedRight));

/* LPSR or LilyPond option?"
  // tagline

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tagline", "",
R"(Set the LilyPond 'tagline' paper variable to '##f' in the LilyPond code.
LilyPond's default value is '##t'.)",
        "fTagline",
        fTagline));
*/

  // page count

  fPageCount = 0;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "page-count", "",
R"(Set the LilyPond 'page-count' paper variable to PAGE_COUNT in the LilyPond code.
PAGE_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
        "PAGE_COUNT",
        "fPageCount",
        fPageCount));

  // system count

  fSystemCount = 0;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "system-count", "",
R"(Set the LilyPond 'system-count' paper variable to SYSTEM_COUNT in the LilyPond code.
SYSTEM_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
        "SYSTEM_COUNT",
        "fSystemCount",
        fSystemCount));
}

void lpsrOahGroup::initializeLpsrVoicesOptions ()
{/* JMI
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "help-lpsr-voices", "hlpsrs",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // auto-voices

  fReplicateEmptyMeasureNumber = "";

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "auto-voices", "",
R"###(Generate the 'auto-voices' Scheme functions in the LilyPond output.
This is a temporary option for experimenting with rests code generation.)###",
        "fAutoVoices",
        fAutoVoices));
        */
}

void lpsrOahGroup::initializeLpsrMeasuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "help-lpsr-measures", "hlpsrmeasures",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // replicate empty measure

  fReplicateEmptyMeasureNumber = "";
  fReplicateEmptyMeasureReplicas = 0;

  subGroup->
    appendAtomToSubGroup (
      oahStringAndIntegerAtom::create (
        "replicate-empty-lpsr-measure", "rem",
R"###(Replicate an empty LPSR mesure, adding empty others according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER:REPLICATES',
where MEASURE_NUMBER is a string, and REPLICATES is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and REPLICATES should be at least 1, , such as '17:3'.
There can be spaces around the ':'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "fReplicateEmptyMeasureNumber",
        fReplicateEmptyMeasureNumber,
        "fReplicateEmptyMeasureReplicas",
        fReplicateEmptyMeasureReplicas));
}

void lpsrOahGroup::initializeLpsrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "help-lpsr-languages", "hlpsrl",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lpsr pitches language

  if (
    ! setLpsrQuarterTonesPitchesLanguage ("nederlands")
  ) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "LPSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known LPSR pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fLpsrQuarterTonesPitchesLanguageKind;

  fLpsrQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lpsrPitchesLanguageAtom::create (
        "lpsr-pitches-language", "lppl",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display note pitches in the LPSR logs and views,
as well as in the generated LilyPond code.
The NUMBER LPSR pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            regex ("PITCHES_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "fLpsrQuarterTonesPitchesLanguageKind",
        fLpsrQuarterTonesPitchesLanguageKind));

  // lpsr chords language

  const lpsrChordsLanguageKind
    lpsrChordsLanguageKindDefaultValue =
      lpsrChordsLanguageKind::kChordsIgnatzek; // LilyPond default

  fLpsrChordsLanguageKind =
    lpsrChordsLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lpsrChordsLanguageAtom::create (
        "lpsr-chords-language", "lpcl",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to display chord names, their root and bass notes,
in the LPSR logs and views and the generated LilyPond code.
The NUMBER LPSR chords pitches languages available are:
CHORDS_LANGUAGES.
'ignatzek' is Ignatzek's jazz-like, english naming used by LilyPond by default.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalLpsrChordsLanguageKindsMap.size ())),
            regex ("CHORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingLpsrChordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          lpsrChordsLanguageKindAsString (
            lpsrChordsLanguageKindDefaultValue)),
        "LANGUAGE",
        "fLpsrChordsLanguageKind",
        fLpsrChordsLanguageKind));
}

void lpsrOahGroup::initializeLpsrTransposeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpose",
        "help-lpsr-transpose", "hlpsrt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lpsr transpose

  subGroup->
    appendAtomToSubGroup (
      lpsrTransposeAtom::create (
        "lpsr-transpose", "lptrans",
R"(Use TRANSPOSITION to tranpose in the LPSR data,
thus in the generated LilyPond code as well.
TRANSPOSITION should contain a diatonic pitch, followed if needed
by a sequence of ',' or '\'' octave indications.
Such indications cannot be mixed.
For example, 'a', 'f' and 'bes,' can be used respectively
for instruments in 'a', 'f' and B flat respectively)",
        "TRANSPOSITION",
        "fTransposeSemiTonesPitchAndOctave",
        fTransposeSemiTonesPitchAndOctave));
}

void lpsrOahGroup::initializeLpsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace and display
  // --------------------------------------
  initializeLpsrtracingOah ();
#endif

  // display
  // --------------------------------------
  initializeLpsrDisplayOptions ();

  // paper
  // --------------------------------------
  initializeLpsrPaperOptions ();

  // voices
  // --------------------------------------
  initializeLpsrVoicesOptions ();

  // measures
  // --------------------------------------
  initializeLpsrMeasuresOptions ();

  // languages
  // --------------------------------------
  initializeLpsrLanguagesOptions ();

/* superfluous JMI ???
  // transpose
  // --------------------------------------
  initializeLpsrTransposeOptions ();
*/
}

//______________________________________________________________________________
Bool lpsrOahGroup::setLpsrQuarterTonesPitchesLanguage (string language)
{
  // is language in the note names languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (language);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
Bool lpsrOahGroup::setLpsrChordsLanguageKind (string language)
{
  // is language in the chords languages map?
  map<string, lpsrChordsLanguageKind>::const_iterator
    it =
      gGlobalLpsrChordsLanguageKindsMap.find (language);

  if (it == gGlobalLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fLpsrChordsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void lpsrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
  fTraceLpsr = false;
  fTraceLilypondVersion = false;
  fTraceLpsrVisitors = false;
  fTraceLpsrBlocks = false;
  fTraceSchemeFunctions = false;
#endif

  fDisplayLpsr = false;
  fDisplayLpsrFull = false;
  fDisplayLpsrShort = false;
}

//______________________________________________________________________________
void lpsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching lpsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> lpsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void lpsrOahGroup::printLpsrOahValues (int fieldWidth)
{
  gLogStream <<
    "The LPSR options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fTraceLpsr" << " : " <<
    fTraceLpsr <<
    endl <<

    setw (fieldWidth) << "fTraceLilypondVersion" << " : " <<
    fTraceLilypondVersion <<
    endl <<

    setw (fieldWidth) << "fTraceLpsrVisitors" << " : " <<
    fTraceLpsrVisitors <<
    endl <<

    setw (fieldWidth) << "fTraceLpsrBlocks" << " : " <<
    fTraceLpsrBlocks <<
    endl <<

    setw (fieldWidth) << "fTraceSchemeFunctions" << " : " <<
    fTraceSchemeFunctions <<
    endl;

  --gIndenter;
#endif

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDisplayLpsr" << " : " <<
    fDisplayLpsr <<
    endl <<
    setw (fieldWidth) << "fDisplayLpsrFull" << " : " <<
    fDisplayLpsrFull <<
    endl <<
    setw (fieldWidth) << "fDisplayLpsrShort" << " : " <<
    fDisplayLpsrShort <<
    endl;

  --gIndenter;

  // paper
  // --------------------------------------

  gLogStream <<
    "Paper:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fAllPaperVariables" << " : " <<
    fAllPaperVariables <<
    endl <<

    setw (fieldWidth) << "fLengthUnitKind" << " : " <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    endl <<

    setw (fieldWidth) << "fPaperHeight" << " : " <<
    fPaperHeight.asString () <<
    endl <<
    setw (fieldWidth) << "fPaperWidth" << " : " <<
    fPaperWidth.asString () <<
    endl <<

    setw (fieldWidth) << "paperfPaperLeftMarginLeftMargin" << " : " <<
    fPaperLeftMargin.asString () <<
    endl <<
    setw (fieldWidth) << "fPaperRightMargin" << " : " <<
    fPaperRightMargin.asString () <<
    endl <<
    setw (fieldWidth) << "fPaperTopMargin" << " : " <<
    fPaperTopMargin.asString () <<
    endl <<
    setw (fieldWidth) << "fPaperBottomMargin" << " : " <<
    fPaperBottomMargin.asString () <<
    endl <<

    setw (fieldWidth) << "fPaperIndent" << " : " <<
    fPaperIndent.asString () <<
    endl <<
    setw (fieldWidth) << "fPaperShortIndent" << " : " <<
    fPaperShortIndent.asString () <<
    endl <<

    setw (fieldWidth) << "fMarkupSystemSpacingPadding" << " : " <<
    fMarkupSystemSpacingPadding.asString () <<
    endl <<

    setw (fieldWidth) << "fRaggedBottom" << " : " <<
    fRaggedBottom <<
    endl <<
    setw (fieldWidth) << "fRaggedLastBottom" << " : " <<
    fRaggedLastBottom <<
    endl <<
    setw (fieldWidth) << "fRaggedLast" << " : " <<
    fRaggedLast <<
    endl <<
    setw (fieldWidth) << "fRaggedRight" << " : " <<
    fRaggedRight <<
    endl <<

    setw (fieldWidth) << "fTagline" << " : " <<
    fTagline <<
    endl <<

    setw (fieldWidth) << "fPageCount" << " : " <<
    fPageCount <<
    endl <<
    setw (fieldWidth) << "fSystemCount" << " : " <<
    fSystemCount <<
    endl;

  --gIndenter;

/* JMI
  // measures
  // --------------------------------------

  gLogStream <<
    "Measures:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "resetMeasureElementMeasureNumberMap" << " : ";
  if (! fAddEmptyMeasuresStringToIntMap.size ()) {
    gLogStream << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fAddEmptyMeasuresStringToIntMap.begin (),
      iEnd   = fAddEmptyMeasuresStringToIntMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLogStream << ",";
    } // for
  }
  gLogStream << endl;

  --gIndenter;
*/

  // tempos
  // --------------------------------------

  gLogStream <<
    "Tempos:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fConvertMusicXMLTemposToMsrRehearsalMarks" << " : " <<
    fConvertMusicXMLTemposToMsrRehearsalMarks <<
    endl << endl;

  --gIndenter;

  // words
  // --------------------------------------

  gLogStream <<
    "Words:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fConvertLpsrWordsToTempo" << " : " <<
    fConvertLpsrWordsToTempo <<
    endl <<
    setw (fieldWidth) << "fAddLpsrWordsFromTheLyrics" << " : " <<
    fAddLpsrWordsFromTheLyrics <<
    endl <<

    setw (fieldWidth) << "fConvertLpsrWordsToRehearsalMarks" << " : " <<
    fConvertLpsrWordsToRehearsalMarks <<
    endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLogStream <<
    "Languages:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fLpsrQuarterTonesPitchesLanguageKind" << " : " <<
    msrQuarterTonesPitchesLanguageKindAsString (
      fLpsrQuarterTonesPitchesLanguageKind) <<
    endl <<

    setw (fieldWidth) << "fLpsrChordsLanguageKind" << " : " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKind) <<
    endl;

  --gIndenter;

  // transpose
  // --------------------------------------

  gLogStream <<
    "Transpose:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fTransposeSemiTonesPitchAndOctave" << " : ";

    if (fTransposeSemiTonesPitchAndOctave) {
      gLogStream <<
        fTransposeSemiTonesPitchAndOctave->asString ();
    }
    else {
      gLogStream <<
        "none";
    }
  gLogStream << endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrOahGroup createGlobalLpsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global LPSR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalLpsrOahGroup) {
    // create the global options group
    gGlobalLpsrOahGroup =
      lpsrOahGroup::create ();
    assert (gGlobalLpsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLpsrOahGroup;
}


}
