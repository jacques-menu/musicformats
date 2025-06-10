/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "mfConstants.h"
#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind)
{
  std::string result;

  switch (facSimileKind) {
    case bsrFacSimileKind::kFacSimileYes:
      result = "facSimileYes";
      break;
    case bsrFacSimileKind::kFacSimileNo:
      result = "facSimileNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrFacSimileKind& elt)
{
  os << bsrFacSimileKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_bsrFacSimileKindAtom bsrFacSimileKindAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  bsrFacSimileKind  bsrFacSimileKindVariable)
{
  bsrFacSimileKindAtom* obj = new
    bsrFacSimileKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      bsrFacSimileKindVariable);
  assert (obj != nullptr);
  return obj;
}

bsrFacSimileKindAtom::bsrFacSimileKindAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
  // JMI ???

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void bsrFacSimileKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrFacSimileKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrFacSimileKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrFacSimileKindAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string bsrFacSimileKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return ss.str ();
}

std::string bsrFacSimileKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return ss.str ();
}

void bsrFacSimileKindAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrFacSimileKindAtom:" <<
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
    "fBsrFacSimileKindVariable" << ": " <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
    std::endl;

  --gIndenter;
}

void bsrFacSimileKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable);
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

std::ostream& operator << (std::ostream& os, const S_bsrFacSimileKindAtom& elt)
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
S_bsrTextsLanguageAtom bsrTextsLanguageAtom::create (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  const std::string&  valueSpecification,
  const std::string&  variableName,
  bsrTextsLanguageKind&
                 bsrTextsLanguageKindVariable)
{
  bsrTextsLanguageAtom* obj = new
    bsrTextsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      bsrTextsLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
}

bsrTextsLanguageAtom::bsrTextsLanguageAtom (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  const std::string&  valueSpecification,
  const std::string&  variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
  // JMI ???

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void bsrTextsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTextsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrTextsLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrTextsLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string bsrTextsLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return ss.str ();
}

std::string bsrTextsLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return ss.str ();
}

void bsrTextsLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "bsrTextsLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fBsrTextsLanguageKindVariable" << ": " <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    std::endl;

  --gIndenter;
}

void bsrTextsLanguageAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable);
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

std::ostream& operator << (std::ostream& os, const S_bsrTextsLanguageAtom& elt)
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
S_bsrOahGroup bsrOahGroup::create ()
{
  bsrOahGroup* obj = new bsrOahGroup ();
  assert (obj != nullptr);
  return obj;
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
    displayFirstBsrScoreShortBooleanAtom =
      oahBooleanAtom::create (
        "display-bsr1", "dbsr1",
R"(Write the contents of the first BSR data to standard error.)",
        "fDisplayFirstBsr",
        fDisplayFirstBsr);

  subGroup->
    appendAtomToSubGroup (
      displayFirstBsrScoreShortBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-bsr1-full", "dbsr1full",
R"(Write the contents of the first BSR data, full version, to standard error.)",
        "fDisplayFirstBsrFull",
        fDisplayFirstBsrFull));

  // display second BSR

  S_oahBooleanAtom
    displaySecondBsrScoreShortBooleanAtom =
      oahBooleanAtom::create (
        "display-bsr2", "dbsr2",
R"(Write the contents of the second BSR data to standard error.)",
        "fDisplaySecondBsr",
        fDisplaySecondBsr);

  subGroup->
    appendAtomToSubGroup (
      displaySecondBsrScoreShortBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-bsr2-full", "dbsr2full",
R"(Write the contents of the second BSR data, full version, to standard error.)",
        "fDisplaySecondBsrFull",
        fDisplaySecondBsrFull));
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
    std::stringstream ss;

    ss <<
      "INTERNAL INITIALIZATION ERROR: "
      "BSR texts language 'english' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalBsrTextsLanguageKindsMap.size () <<
      " known BSR texts languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableBsrTextsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
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
              std::regex ("NUMBER"),
              std::to_string (gGlobalBsrTextsLanguageKindsMap.size ())),
            std::regex ("TEXT_LANGUAGES"),
            availableBsrTextsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "fBsrTextsLanguageKind",
        fBsrTextsLanguageKind));
}

#ifdef MF_TRACE_IS_ENABLED
void bsrOahGroup::initializeBsrTraceOah ()
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
#endif // MF_TRACE_IS_ENABLED

void bsrOahGroup::initializeBsrOahGroup ()
{
  // display
  // --------------------------------------
  initializeBsrDisplayOptions ();

  // languages
  // --------------------------------------
  initializeBsrLanguagesOptions ();

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeBsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
Bool bsrOahGroup::setBsrTextsLanguage (const std::string& language)
{
  // is language in the chords languages map?
  std::map <std::string, bsrTextsLanguageKind>::const_iterator
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
  fDisplayFirstBsr = false;
  fDisplayFirstBsrFull = false;

  fDisplaySecondBsr = false;
  fDisplaySecondBsrFull = false;
}

//______________________________________________________________________________
void bsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_bsrOahGroup>*> (v)) {
        S_bsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching bsrOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> bsrOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void bsrOahGroup::displayBsrOahValues (int fieldWidth)
{
  gLog <<
    "The BSR bsr are:" <<
    std::endl;

  ++gIndenter;

  // display
  // --------------------------------------
  gLog <<
    "Display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fDisplayFirstBsr" << ": " <<
    fDisplayFirstBsr <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplayFirstBsrFull" << ": " <<
    fDisplayFirstBsrFull <<
    std::endl <<

    std::setw (fieldWidth) << "fDisplaySecondBsr" << ": " <<
    fDisplaySecondBsr <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplaySecondBsrFull" << ": " <<
    fDisplaySecondBsrFull <<
    std::endl;

  --gIndenter;

  // trace
  // --------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTraceBsr" << ": " <<
    fTraceBsr <<
    std::endl <<
    std::setw (fieldWidth) << "fTracePages" << ": " <<
    fTracePages <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceLines" << ": " <<
    fTraceLines <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceSpaces" << ": " <<
    fTraceSpaces <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceNumbers" << ": " <<
    fTraceNumbers <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceParallels" << ": " <<
    fTraceParallels <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceBsrVisitors" << ": " <<
    fTraceBsrVisitors <<
    std::endl;

  --gIndenter;
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
// hidden global BSR OAH group ariable
EXP S_bsrOahGroup pGlobalBsrOahGroup;

EXP S_bsrOahGroup getGlobalBsrOahGroup ()
{
  return pGlobalBsrOahGroup;
}

//______________________________________________________________________________
S_bsrOahGroup createGlobalBsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global BSR OAH group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalBsrOahGroup) {
    // create the bsr variables
    pGlobalBsrOahGroup =
      bsrOahGroup::create ();
    assert (pGlobalBsrOahGroup != 0);
  }

  // return the global OAH group
  return pGlobalBsrOahGroup;
}


}
