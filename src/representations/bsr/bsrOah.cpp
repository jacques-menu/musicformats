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

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind)
{
  string result;

  switch (facSimileKind) {
    case kFacSimileYes:
      result = "facSimileYes";
      break;
    case kFacSimileNo:
      result = "facSimileNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_bsrFacSimileKindAtom bsrFacSimileKindAtom::create (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  bsrFacSimileKind  bsrFacSimileKindVariable)
{
  bsrFacSimileKindAtom* o = new
    bsrFacSimileKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      bsrFacSimileKindVariable);
  assert (o != nullptr);
  return o;
}

bsrFacSimileKindAtom::bsrFacSimileKindAtom (
  const string&     longName,
  const string&     shortName,
  const string&     description,
  const string&     valueSpecification,
  const string&     variableName,
  bsrFacSimileKind  bsrFacSimileKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fBsrFacSimileKindVariable (
      bsrFacSimileKindVariable)
{}

bsrFacSimileKindAtom::~bsrFacSimileKindAtom ()
{}

void bsrFacSimileKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'" <<
      endl;
  }
#endif
}

void bsrFacSimileKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFacSimileKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFacSimileKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrFacSimileKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrFacSimileKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

string bsrFacSimileKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

void bsrFacSimileKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrFacSimileKindAtom:" <<
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
    "fBsrFacSimileKindVariable" << " : " <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
    endl;

  --gIndenter;
}

void bsrFacSimileKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable);
  if (fVariableHasBeenSet) {
    os <<
      ", has been set";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_bsrFacSimileKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTextsLanguageAtom bsrTextsLanguageAtom::create (
  const string&  longName,
  const string&  shortName,
  const string&  description,
  const string&  valueSpecification,
  const string&  variableName,
  bsrTextsLanguageKind&
                 bsrTextsLanguageKindVariable)
{
  bsrTextsLanguageAtom* o = new
    bsrTextsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      bsrTextsLanguageKindVariable);
  assert (o != nullptr);
  return o;
}

bsrTextsLanguageAtom::bsrTextsLanguageAtom (
  const string&  longName,
  const string&  shortName,
  const string&  description,
  const string&  valueSpecification,
  const string&  variableName,
  bsrTextsLanguageKind&
                 bsrTextsLanguageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fBsrTextsLanguageKindVariable (
      bsrTextsLanguageKindVariable)
{}

bsrTextsLanguageAtom::~bsrTextsLanguageAtom ()
{}

void bsrTextsLanguageAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // JMI ???

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
      endl;
  }
#endif
}

void bsrTextsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTextsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTextsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrTextsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrTextsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

string bsrTextsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

void bsrTextsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrTextsLanguageAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fBsrTextsLanguageKindVariable" << " : " <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    endl;

  --gIndenter;
}

void bsrTextsLanguageAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable);
  if (fVariableHasBeenSet) {
    os <<
      ", has been set";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_bsrTextsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrOahGroup gGlobalBsrOahGroup;

S_bsrOahGroup bsrOahGroup::create ()
{
  bsrOahGroup* o = new bsrOahGroup ();
  assert (o != nullptr);
  return o;
}

bsrOahGroup::bsrOahGroup ()
  : oahGroup (
      "BSR",
      "help-bsr", "hbsr",
  R"(These bsr control the way BSR data is handled.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeBsrOahGroup ();
}

bsrOahGroup::~bsrOahGroup ()
{}

void bsrOahGroup::initializeBsrDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display BSR",
        "help-bsr-display", "hbsrd",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display first BSR

  S_oahBooleanAtom
    displayBsrFirstBooleanAtom =
      oahBooleanAtom::create (
        "display-bsr-1", "dbsr1",
R"(Write the contents of the first BSR data to standard error.)",
        "fDisplayBsrFirst",
        fDisplayBsrFirst);
  subGroup->
    appendAtomToSubGroup (
      displayBsrFirstBooleanAtom);

  // display first BSR short

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-bsr-1-short", "dbsr1s",
R"(Write the contents of the first BSR data, short version, to standard error.)",
        "fDisplayBsrFirstShort",
        fDisplayBsrFirstShort));

  // display first BSR details

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "display-bsr-1-details", "dbsr1d",
R"(Write the contents of the first BSR data with more details to standard error.)",
        "fDisplayBsrFirstDetails",
        fDisplayBsrFirstDetails,
        displayBsrFirstBooleanAtom));

  // display second BSR

  S_oahBooleanAtom
    displayBsrSecondBooleanAtom =
      oahBooleanAtom::create (
        "display-bsr-2", "dbsr2",
R"(Write the contents of the second BSR data to standard error.)",
        "fDisplayBsrSecond",
        fDisplayBsrSecond);
  subGroup->
    appendAtomToSubGroup (
      displayBsrSecondBooleanAtom);

  // display second BSR short

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-bsr-2-short", "dbsr2s",
R"(Write the contents of the second BSR data, short version, to standard error.)",
        "fDisplayBsrSecondShort",
        fDisplayBsrSecondShort));

  // display second BSR details

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "display-bsr-2-details", "dbsr2d",
R"(Write the contents of the second BSR data with more details to standard error.)",
        "fDisplayBsrSecondDetails",
        fDisplayBsrSecondDetails,
        displayBsrSecondBooleanAtom));
}

void bsrOahGroup::initializeBsrLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "help-bsr-languages", "hbsrl",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // texts language

  if (! setBsrTextsLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "BSR texts language 'english' is unknown" <<
      endl <<
      "The " <<
      gGlobalBsrTextsLanguageKindsMap.size () <<
      " known BSR texts languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  const bsrTextsLanguageKind
    bsrTextsLanguageKindDefaultValue =
      fBsrTextsLanguageKind;

  subGroup->
    appendAtomToSubGroup (
      bsrTextsLanguageAtom::create (
         "bsr-texts-language", "btl",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to transcribe texts in the BSR logs and views,
as well as in the generated Braille.
The NUMBER texts languages available are:
TEXT_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalBsrTextsLanguageKindsMap.size ())),
            regex ("TEXT_LANGUAGES"),
            existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "fBsrTextsLanguageKind",
        fBsrTextsLanguageKind));
}

#ifdef TRACING_IS_ENABLED
void bsrOahGroup::initializeBsrtracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "BSR trace",
        "help-bsr-trace", "hbst",
R"(  Note: the options in this subgroup imply '-tbsr, -trace-bsr'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // BSR

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-bsr", "tbsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "fTraceBsr",
        fTraceBsr));

  // pages

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-pages", "tpages",
R"()",
        "fTracePages",
        fTracePages));

  // lines

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-lines", "tlines",
R"()",
        "fTraceLines",
        fTraceLines));

  // spaces

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-spaces", "tspaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "fTraceSpaces",
        fTraceSpaces));

  // numbers

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         "trace-numbers", "tnums",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "fTraceNumbers",
        fTraceNumbers));

  // parallels

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         "trace-parallels", "tpars",
R"()",
        "fTraceParallels",
        fTraceParallels));

  // BSR visitors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-bsr-visitors", "tbsrvis",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
        "fTraceBsrVisitors",
        fTraceBsrVisitors));
}
#endif

void bsrOahGroup::initializeBsrOahGroup ()
{
  // display
  // --------------------------------------
  initializeBsrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeBsrLanguagesOptions ();

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeBsrtracingOah ();
#endif
}

//______________________________________________________________________________
Bool bsrOahGroup::setBsrTextsLanguage (const string& language)
{
  // is language in the chords languages map?
  map<string, bsrTextsLanguageKind>::const_iterator
    it =
      gGlobalBsrTextsLanguageKindsMap.find (language);

  if (it == gGlobalBsrTextsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fBsrTextsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void bsrOahGroup::enforceGroupQuietness ()
{
  fDisplayBsrFirst = false;
  fDisplayBsrFirstShort = false;
  fDisplayBsrFirstDetails = false;

  fDisplayBsrSecond = false;
  fDisplayBsrSecondShort = false;
  fDisplayBsrSecondDetails = false;
}

//______________________________________________________________________________
void bsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching bsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> bsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsrOahGroup::printBsrOahValues (int fieldWidth)
{
  gLogStream <<
    "The BSR bsr are:" <<
    endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDisplayBsrFirst" << " : " <<
    fDisplayBsrFirst <<
    endl <<
    setw (fieldWidth) << "fDisplayBsrFirstShort" << " : " <<
    fDisplayBsrFirstShort <<
    endl <<
    setw (fieldWidth) << "fDisplayBsrFirstDetails" << " : " <<
    fDisplayBsrFirstDetails <<
    endl <<

    setw (fieldWidth) << "fDisplayBsrSecond" << " : " <<
    fDisplayBsrSecond <<
    endl <<
    setw (fieldWidth) << "fDisplayBsrSecondShort" << " : " <<
    fDisplayBsrSecondShort <<
    endl <<
    setw (fieldWidth) << "fDisplayBsrSecondDetails" << " : " <<
    fDisplayBsrSecondDetails <<
    endl;

  --gIndenter;

  // trace
  // --------------------------------------

#ifdef TRACING_IS_ENABLED
  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fTraceBsr" << " : " <<
    fTraceBsr <<
    endl <<
    setw (fieldWidth) << "fTracePages" << " : " <<
    fTracePages <<
    endl <<
    setw (fieldWidth) << "fTraceLines" << " : " <<
    fTraceLines <<
    endl <<
    setw (fieldWidth) << "fTraceSpaces" << " : " <<
    fTraceSpaces <<
    endl <<
    setw (fieldWidth) << "fTraceNumbers" << " : " <<
    fTraceNumbers <<
    endl <<
    setw (fieldWidth) << "fTraceParallels" << " : " <<
    fTraceParallels <<
    endl <<
    setw (fieldWidth) << "fTraceBsrVisitors" << " : " <<
    fTraceBsrVisitors <<
    endl;

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrOahGroup createGlobalBsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global BSR OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalBsrOahGroup) {
    // create the bsr variables
    gGlobalBsrOahGroup =
      bsrOahGroup::create ();
    assert (gGlobalBsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalBsrOahGroup;
}


}
