/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "mfPreprocessorSettings.h"

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

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string lilypondScoreNotationKindAsString (
  lilypondScoreNotationKind notationKind)
{
  std::string result;

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
  const std::string&   shortName,
  const std::string&   longName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
  lpsrScoreOutputKind& lilypondScoreOutputKindVariable)
{
  lilypondScoreOutputKindAtom* obj = new
    lilypondScoreOutputKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondScoreOutputKindVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondScoreOutputKindAtom::lilypondScoreOutputKindAtom (
  const std::string&   shortName,
  const std::string&   longName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
  // JMI ???

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondScoreOutputKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void lilypondScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondScoreOutputKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondScoreOutputKindAtom>*> (v)) {
        S_lilypondScoreOutputKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondScoreOutputKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondScoreOutputKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondScoreOutputKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return ss.str ();
}

std::string lilypondScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return ss.str ();
}

void lilypondScoreOutputKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondScoreOutputKindAtom:" <<
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
    "fLpsrScoreOutputKindVariable" << ": \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void lilypondScoreOutputKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"";

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

std::ostream& operator << (std::ostream& os, const S_lilypondScoreOutputKindAtom& elt)
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
S_lilypondTransposePartNameAtom lilypondTransposePartNameAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartNameAtom* obj = new
    lilypondTransposePartNameAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondTransposePartNameAtom::lilypondTransposePartNameAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map<std::string, S_msrSemiTonesPitchAndOctave>&
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondTransposePartNameAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  std::string regularExpression (
    "(.*)" // partName
    "="
    "(.*)" // destinationPitchName
    );
//    "[[:space:]]*(.*)[[:space:]]*" JMI
//    "="
//    "[[:space:]]*(.*)[[:space:]]*");

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLog <<
          '[' << sm [i] << "] ";
      } // for
      gLog << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "-lilypond-transpose-part-name argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    partName             = sm [1],
    destinationPitchName = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partName = \"" << partName << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::map<std::string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partName);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part name \"" << partName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partName] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_MF_INPUT_LINE_UNKNOWN_,
        destinationPitchName);
  }

  fSelected = true;
}

void lilypondTransposePartNameAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondTransposePartNameAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartNameAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondTransposePartNameAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartNameAtom>*> (v)) {
        S_lilypondTransposePartNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondTransposePartNameAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondTransposePartNameAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartNameAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondTransposePartNameAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    ss <<
      '-' << fShortName << " \"";

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    ss << "\"";
  }

  return ss.str ();
}

std::string lilypondTransposePartNameAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    ss <<
      '-' << fShortName << " \"";

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    ss << "\"";
  }

  return ss.str ();
}

void lilypondTransposePartNameAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartNameAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << ": " <<
    std::endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  os << std::endl;

  --gIndenter;
}

void lilypondTransposePartNameAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

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

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "Part name \"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_lilypondTransposePartNameAtom& elt)
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
S_lilypondTransposePartIDAtom lilypondTransposePartIDAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                stringToMsrSemiTonesPitchAndOctaveMapVariable)
{
  lilypondTransposePartIDAtom* obj = new
    lilypondTransposePartIDAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToMsrSemiTonesPitchAndOctaveMapVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondTransposePartIDAtom::lilypondTransposePartIDAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map<std::string, S_msrSemiTonesPitchAndOctave>&
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondTransposePartIDAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the part transpose specification
  // decipher it to extract the old and new part names

  std::string regularExpression (
    "(.*)" // partID
    "="
    "(.*)" // destination pitch name
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 3) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLog <<
          '[' << sm [i] << "] ";
      } // for
      gLog << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "-lilypond-transpose-part-id argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    partID               = sm [1],
    destinationPitchName = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partID = \"" << partID << "\", " <<
      "--> destinationPitchName = \"" << destinationPitchName << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::map<std::string, S_msrSemiTonesPitchAndOctave>::iterator
    it =
      fStringToMsrSemiTonesPitchAndOctaveMapVariable.find (partID);

  if (it != fStringToMsrSemiTonesPitchAndOctaveMapVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part ID \"" << partID << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringToMsrSemiTonesPitchAndOctaveMapVariable [partID] =
      msrSemiTonesPitchAndOctave::createFromString (
        K_MF_INPUT_LINE_UNKNOWN_,
        destinationPitchName);
  }

  fSelected = true;
}

void lilypondTransposePartIDAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondTransposePartIDAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartIDAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondTransposePartIDAtom>*
    p =
      dynamic_cast<visitor<S_lilypondTransposePartIDAtom>*> (v)) {
        S_lilypondTransposePartIDAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondTransposePartIDAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondTransposePartIDAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondTransposePartIDAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondTransposePartIDAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    ss <<
      '-' << fShortName << " \"";

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    ss << "\"";
  }

  return ss.str ();
}

std::string lilypondTransposePartIDAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  if (fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    ss <<
      '-' << fShortName << " \"";

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << "=" <<
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          (*i).second);
      if (++i == iEnd) break;
    } // for

    ss << "\"";
  }

  return ss.str ();
}

void lilypondTransposePartIDAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondTransposePartIDAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringToMsrSemiTonesPitchAndOctaveMapVariable" << ": " <<
    std::endl;

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  os << std::endl;

  --gIndenter;
}

void lilypondTransposePartIDAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

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

  if (! fStringToMsrSemiTonesPitchAndOctaveMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      iBegin = fStringToMsrSemiTonesPitchAndOctaveMapVariable.begin (),
      iEnd   = fStringToMsrSemiTonesPitchAndOctaveMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> " <<
        (*i).second <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_lilypondTransposePartIDAtom& elt)
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
S_msrOctaveEntryVariable msrOctaveEntryVariable::create (
  const std::string&      variableName,
  msrOctaveEntryKind octaveEntryKind)
{
  msrOctaveEntryVariable* obj = new
    msrOctaveEntryVariable (
      variableName,
      octaveEntryKind);
  assert (obj != nullptr);
  return obj;
}

msrOctaveEntryVariable::msrOctaveEntryVariable (
  const std::string&      variableName,
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

void msrOctaveEntryVariable::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OctaveEntryVariable :" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOctaveEntryKind" << ": " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKind) <<
    std::endl;

  --gIndenter;
}

void msrOctaveEntryVariable::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKind) <<
    std::endl;

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

std::ostream& operator << (std::ostream& os, const msrOctaveEntryVariable& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
S_lilypondAbsoluteOctaveEntryAtom lilypondAbsoluteOctaveEntryAtom::create (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  msrOctaveEntryVariable& octaveEntryKindVariable)
{
  lilypondAbsoluteOctaveEntryAtom* obj = new
    lilypondAbsoluteOctaveEntryAtom (
      longName,
      shortName,
      description,
      variableName,
      octaveEntryKindVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondAbsoluteOctaveEntryAtom::lilypondAbsoluteOctaveEntryAtom (
  const std::string&      longName,
  const std::string&      shortName,
  const std::string&      description,
  const std::string&      variableName,
  msrOctaveEntryVariable& octaveEntryKindVariable)
  : oahValueLessAtom (
      longName,
      shortName,
      description),
    fOctaveEntryKindVariable (
      octaveEntryKindVariable)
{}

lilypondAbsoluteOctaveEntryAtom::~lilypondAbsoluteOctaveEntryAtom ()
{}

void lilypondAbsoluteOctaveEntryAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () <<
      "' is a lilypondAbsoluteOctaveEntryAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set octave entry kind
  fOctaveEntryKindVariable.setOctaveEntryKind (
    msrOctaveEntryKind::kOctaveEntryAbsolute);

  fSelected = true;
}

void lilypondAbsoluteOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondAbsoluteOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAbsoluteOctaveEntryAtom>*> (v)) {
        S_lilypondAbsoluteOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondAbsoluteOctaveEntryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondAbsoluteOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAbsoluteOctaveEntryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lilypondAbsoluteOctaveEntryAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
  std::stringstream ss;

  ss <<
    "applying absolute octave entry option \"" << fetchNames () <<
    "\" with a value";

  oahInternalError (ss.str ());
}

void lilypondAbsoluteOctaveEntryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondAbsoluteOctaveEntryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "octaveEntryKindVariable" << ": " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    std::endl;

  --gIndenter;
}

void lilypondAbsoluteOctaveEntryAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
//   os << std::left << // JMI v0.9.67
//     std::setw (valueFieldWidth) <<
//     fOctaveEntryKindVariable.getVariableName () <<
//     ": " <<
//     "- no possible value here -" <<
//     std::endl;
}

//______________________________________________________________________________
S_lilypondRelativeOctaveEntryAtom lilypondRelativeOctaveEntryAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrOctaveEntryVariable&
                     octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                     lilypondRelativeOctaveEntryVariable)
{
  lilypondRelativeOctaveEntryAtom* obj = new
    lilypondRelativeOctaveEntryAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      octaveEntryKindVariable,
      lilypondRelativeOctaveEntryVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondRelativeOctaveEntryAtom::lilypondRelativeOctaveEntryAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondRelativeOctaveEntryAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the relative reference note

/* JMI
  // is it in the score output kinds map?

  std::map<std::string, msrOctaveEntryKind>::const_iterator
    it =
      gGlobalMsrOctaveEntryKindsMap.find (
        theString);

  if (it == gGlobalMsrOctaveEntryKindsMap.end ()) {
    // no, octave entry kind is unknown in the map

    printOptionsSummary (gLog);

    std::stringstream ss;

    ss <<
      "octave entry kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsrOctaveEntryKindsMap.size () <<
      " known octave entry kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsrOctaveEntryKinds ();

    --gIndenter;

    oahError (ss.str ());
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
      K_MF_INPUT_LINE_UNKNOWN_,
      theString));

  fSelected = true;
}

void lilypondRelativeOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondRelativeOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondRelativeOctaveEntryAtom>*> (v)) {
        S_lilypondRelativeOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondRelativeOctaveEntryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondRelativeOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondRelativeOctaveEntryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondRelativeOctaveEntryAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

std::string lilypondRelativeOctaveEntryAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

void lilypondRelativeOctaveEntryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondRelativeOctaveEntryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "octaveEntryKindVariable" << ": " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    std::endl;

  --gIndenter;
}

void lilypondRelativeOctaveEntryAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fOctaveEntryKindVariable <<
    ": ";

  if (fSemiTonesPitchAndOctaveVariable) {
    os << std::endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }
}

std::ostream& operator << (std::ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt)
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
S_lilypondFixedOctaveEntryAtom lilypondFixedOctaveEntryAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrOctaveEntryVariable&
                     octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                     lilypondFixedOctaveEntryVariable)
{
  lilypondFixedOctaveEntryAtom* obj = new
    lilypondFixedOctaveEntryAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      octaveEntryKindVariable,
      lilypondFixedOctaveEntryVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondFixedOctaveEntryAtom::lilypondFixedOctaveEntryAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrOctaveEntryVariable&
                     octaveEntryKindVariable,
  S_msrSemiTonesPitchAndOctave&
                     lilypondFixedOctaveEntryVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fOctaveEntryKindVariable (
      octaveEntryKindVariable),
    fSemiTonesPitchAndOctaveVariable (
      lilypondFixedOctaveEntryVariable)
{}

lilypondFixedOctaveEntryAtom::~lilypondFixedOctaveEntryAtom ()
{}

void lilypondFixedOctaveEntryAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondFixedOctaveEntryAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the fixed reference note

  // set octave entry kind
  fOctaveEntryKindVariable.setOctaveEntryKind (
    msrOctaveEntryKind::kOctaveEntryFixed);

  // set fixed octave reference value
  setFixedOctaveEntryVariableValue (
    msrSemiTonesPitchAndOctave::createFromString (
      K_MF_INPUT_LINE_UNKNOWN_,
      theString));

  fSelected = true;
}

void lilypondFixedOctaveEntryAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondFixedOctaveEntryAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondFixedOctaveEntryAtom>*
    p =
      dynamic_cast<visitor<S_lilypondFixedOctaveEntryAtom>*> (v)) {
        S_lilypondFixedOctaveEntryAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondFixedOctaveEntryAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondFixedOctaveEntryAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondFixedOctaveEntryAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondFixedOctaveEntryAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

std::string lilypondFixedOctaveEntryAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

void lilypondFixedOctaveEntryAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondFixedOctaveEntryAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "octaveEntryKindVariable" << ": " <<
    msrOctaveEntryKindAsString (
      fOctaveEntryKindVariable.getOctaveEntryKind ()) <<
    std::endl;

  --gIndenter;
}

void lilypondFixedOctaveEntryAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fOctaveEntryKindVariable <<
    ": ";

  if (fSemiTonesPitchAndOctaveVariable) {
    os << std::endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }
}

std::ostream& operator << (std::ostream& os, const S_lilypondFixedOctaveEntryAtom& elt)
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
S_lilypondAccidentalStyleKindAtom lilypondAccidentalStyleKindAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  lpsrAccidentalStyleKind&
                lilypondAccidentalStyleKindVariable)
{
  lilypondAccidentalStyleKindAtom* obj = new
    lilypondAccidentalStyleKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondAccidentalStyleKindVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondAccidentalStyleKindAtom::lilypondAccidentalStyleKindAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the accidental styles map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondAccidentalStyleKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map<std::string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gGlobalLpsrAccidentalStyleKindsMap.find (
        theString);

  if (it == gGlobalLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidental style is unknown in the map
    std::stringstream ss;

    ss <<
      "LPSR accidental style '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalLpsrAccidentalStyleKindsMap.size () - 1 <<
      " known LPSR accidental styles are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableLpsrAccidentalStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLpsrAccidentalStyleKindVariable (
    (*it).second);

  fSelected = true;
}

void lilypondAccidentalStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondAccidentalStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondAccidentalStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondAccidentalStyleKindAtom>*> (v)) {
        S_lilypondAccidentalStyleKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondAccidentalStyleKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondAccidentalStyleKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondAccidentalStyleKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondAccidentalStyleKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return ss.str ();
}

std::string lilypondAccidentalStyleKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrAccidentalStyleKindAsString (fLpsrAccidentalStyleKindVariable);

  return ss.str ();
}

void lilypondAccidentalStyleKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondAccidentalStyleKindAtom:" <<
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
    "fLpsrAccidentalStyleKindVariable" << ": \"" <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable) <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void lilypondAccidentalStyleKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    lpsrAccidentalStyleKindAsString (
      fLpsrAccidentalStyleKindVariable);

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

std::ostream& operator << (std::ostream& os, const S_lilypondAccidentalStyleKindAtom& elt)
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
S_lilypondChordsDisplayAtom lilypondChordsDisplayAtom::create (
  const std::string&    shortName,
  const std::string&    longName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  std::list<std::pair<std::string, std::string> >&
                        lilypondChordsDisplayVariable)
{
  lilypondChordsDisplayAtom* obj = new
    lilypondChordsDisplayAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondChordsDisplayVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondChordsDisplayAtom::lilypondChordsDisplayAtom (
  const std::string&    shortName,
  const std::string&    longName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  std::list<std::pair<std::string, std::string> >&
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    % std::exception music is chords with markups
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondChordsDisplayAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "(<.+>)"
    "[[:space:]]+"
    "(.+)"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for chords display string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "':" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "-chords-display argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string chordContents     = sm [1];
  std::string chordsDisplaycode = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "FAA chordContents: \"" <<
      chordContents <<
      std::endl <<
      "chordsDisplaycode: " <<
      chordsDisplaycode <<
      std::endl;

  setStringsPairListVariable (
    std::make_pair (
      chordContents, chordsDisplaycode));
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void lilypondChordsDisplayAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondChordsDisplayAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondChordsDisplayAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondChordsDisplayAtom>*
    p =
      dynamic_cast<visitor<S_lilypondChordsDisplayAtom>*> (v)) {
        S_lilypondChordsDisplayAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondChordsDisplayAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondChordsDisplayAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondChordsDisplayAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondChordsDisplayAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (fStringsPairListVariable.size ()) { // JMI superflous???
    std::list<std::pair<std::string, std::string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << " --> " << (*i).second <<
        std::endl;
      if (++i == iEnd) break;
      ss << ","; // JMI
    } // for
  }

  return ss.str ();
}

std::string lilypondChordsDisplayAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (fStringsPairListVariable.size ()) { // JMI superflous???
    std::list<std::pair<std::string, std::string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        (*i).first << " --> " << (*i).second <<
        std::endl;
      if (++i == iEnd) break;
      ss << ","; // JMI
    } // for
  }

  return ss.str ();
}

void lilypondChordsDisplayAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondChordsDisplayAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringsPairListVariable" << " : '" <<
    std::endl;

  if (fStringsPairListVariable.size ()) {
    ++gIndenter;

    std::list<std::pair<std::string, std::string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i).first << " --> " << (*i).second <<
        std::endl;
      if (++i == iEnd) break;
  // JMI    os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void lilypondChordsDisplayAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (fStringsPairListVariable.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list<std::pair<std::string, std::string> >::const_iterator
      iBegin = fStringsPairListVariable.begin (),
      iEnd   = fStringsPairListVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    switch (fEarlyOptionKind) {
      case oahEarlyOptionKind::kEarlyOptionNo:
        break;
      case oahEarlyOptionKind::kEarlyOptionYes:
        os <<
          "fEarlyOptionKind: " <<
          fEarlyOptionKind <<
          std::endl;
        break;
    } // switch

    if (fSelected) {
      os <<
        "fSelected: " <<
        fSelected <<
        std::endl;
    }

    --gIndenter;
  }
  else {
    os << "[EMPTY]";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lilypondChordsDisplayAtom& elt)
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
S_lilypondLyricsNotesDurationsKindAtom lilypondLyricsNotesDurationsKindAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  lpsrLyricsNotesDurationsKind&
                    lilypondLyricsNotesDurationsKindVariable)
{
  lilypondLyricsNotesDurationsKindAtom* obj = new
    lilypondLyricsNotesDurationsKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondLyricsNotesDurationsKindVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondLyricsNotesDurationsKindAtom::lilypondLyricsNotesDurationsKindAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  lpsrLyricsNotesDurationsKind&
                    lilypondLyricsNotesDurationsKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fLpsrLyricsNotesDurationsKindVariable (
      lilypondLyricsNotesDurationsKindVariable)
{}

lilypondLyricsNotesDurationsKindAtom::~lilypondLyricsNotesDurationsKindAtom ()
{}

void lilypondLyricsNotesDurationsKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondLyricsNotesDurationsKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the lyrics alignment kinds map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondLyricsNotesDurationsKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map<std::string, lpsrLyricsNotesDurationsKind>::const_iterator
    it =
      gGlobalLpsrLyricsNotesDurationsKindsMap.find (
        theString);

  if (it == gGlobalLpsrLyricsNotesDurationsKindsMap.end ()) {
    // no, lyrics alignment kind is unknown in the map
    std::stringstream ss;

    ss <<
      "LPSR lyrics alignment kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalLpsrLyricsNotesDurationsKindsMap.size () - 1 <<
      " known LPSR lyrics alignment kind are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableLpsrLyricsNotesDurationsKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLpsrLyricsNotesDurationsKindVariable (
    (*it).second);

  fSelected = true;
}

void lilypondLyricsNotesDurationsKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondLyricsNotesDurationsKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondLyricsNotesDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsNotesDurationsKindAtom>*> (v)) {
        S_lilypondLyricsNotesDurationsKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondLyricsNotesDurationsKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondLyricsNotesDurationsKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondLyricsNotesDurationsKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondLyricsNotesDurationsKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondLyricsNotesDurationsKindAtom>*> (v)) {
        S_lilypondLyricsNotesDurationsKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondLyricsNotesDurationsKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondLyricsNotesDurationsKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondLyricsNotesDurationsKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondLyricsNotesDurationsKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrLyricsNotesDurationsKindAsString (
      fLpsrLyricsNotesDurationsKindVariable);

  return ss.str ();
}

std::string lilypondLyricsNotesDurationsKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrLyricsNotesDurationsKindAsString (
      fLpsrLyricsNotesDurationsKindVariable);

  return ss.str ();
}

void lilypondLyricsNotesDurationsKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondLyricsNotesDurationsKindAtom:" <<
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
    "fLpsrLyricsNotesDurationsKindVariable" << ": " <<
    lpsrLyricsNotesDurationsKindAsString (
      fLpsrLyricsNotesDurationsKindVariable) <<
    std::endl;

  --gIndenter;
}

void lilypondLyricsNotesDurationsKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    lpsrLyricsNotesDurationsKindAsString (
      fLpsrLyricsNotesDurationsKindVariable);

  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os <<
      ", selected";

  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lilypondLyricsNotesDurationsKindAtom& elt)
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
S_lilypondDynamicsTextSpannersStyleKindAtom lilypondDynamicsTextSpannersStyleKindAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  lpsrDynamicsTextSpannersStyleKind&
                    lilypondDynamicsTextSpannersStyleKindVariable)
{
  lilypondDynamicsTextSpannersStyleKindAtom* obj = new
    lilypondDynamicsTextSpannersStyleKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lilypondDynamicsTextSpannersStyleKindVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondDynamicsTextSpannersStyleKindAtom::lilypondDynamicsTextSpannersStyleKindAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondDynamicsTextSpannersStyleKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the dynamics text spanners style name:
  // is it in the lyrics alignment kinds map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondDynamicsTextSpannersStyleKindAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map<std::string, lpsrDynamicsTextSpannersStyleKind>::const_iterator
    it =
      getLpsrDynamicsTextSpannersStyleKindsMap ().find (
        theString);

  if (it == getLpsrDynamicsTextSpannersStyleKindsMap ().end ()) {
    // no, dynamics text spanners style kind is unknown in the map
    std::stringstream ss;

    ss <<
      "dynamics text spanners style kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      getLpsrDynamicsTextSpannersStyleKindsMap ().size () - 1 <<
      " known LPSR lyrics alignment kind are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableLpsrDynamicsTextSpannersStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLpsrdDynamicsTextSpannersStyleKindVariable (
    (*it).second);

  fSelected = true;
}

void lilypondDynamicsTextSpannersStyleKindAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*> (v)) {
        S_lilypondDynamicsTextSpannersStyleKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondDynamicsTextSpannersStyleKindAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lilypondDynamicsTextSpannersStyleKindAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*
    p =
      dynamic_cast<visitor<S_lilypondDynamicsTextSpannersStyleKindAtom>*> (v)) {
        S_lilypondDynamicsTextSpannersStyleKindAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lilypondDynamicsTextSpannersStyleKindAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lilypondDynamicsTextSpannersStyleKindAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lilypondDynamicsTextSpannersStyleKindAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lilypondDynamicsTextSpannersStyleKindAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrDynamicsTextSpannersStyleKindAsString (fLpsrdDynamicsTextSpannersStyleKindVariable);

  return ss.str ();
}

std::string lilypondDynamicsTextSpannersStyleKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrDynamicsTextSpannersStyleKindAsString (fLpsrdDynamicsTextSpannersStyleKindVariable);

  return ss.str ();
}

void lilypondDynamicsTextSpannersStyleKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondDynamicsTextSpannersStyleKindAtom:" <<
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
    "fLpsrdDynamicsTextSpannersStyleKindVariable" << ": " <<
    lpsrDynamicsTextSpannersStyleKindAsString (
      fLpsrdDynamicsTextSpannersStyleKindVariable) <<
    std::endl;

  --gIndenter;
}

void lilypondDynamicsTextSpannersStyleKindAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    lpsrDynamicsTextSpannersStyleKindAsString (
      fLpsrdDynamicsTextSpannersStyleKindVariable);

  if (fSelected) {
    os <<
      ", selected";

  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lilypondDynamicsTextSpannersStyleKindAtom& elt)
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
S_lpsr2lilypondOahGroup gGlobalLpsr2lilypondOahGroup;

S_lpsr2lilypondOahGroup lpsr2lilypondOahGroup::create ()
{
  lpsr2lilypondOahGroup* obj = new lpsr2lilypondOahGroup ();
  assert (obj != nullptr);
  return obj;
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

  fLilypondVersionMinimumValue = "2.24.0";

  fLilypondVersion = fLilypondVersionMinimumValue;

  fLilypondVersionAtom =
    oahStringAtom::create (
      "lilypond-version", "lpv",
      regex_replace (
R"(Set the Lilypond '\version' to STRING in the Lilypond code.
The default is the mininum 'MINIMUM_VALUE' value.)",
        std::regex ("MINIMUM_VALUE"),
        fLilypondVersionMinimumValue),
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
        "lilyazz", "",
R"(Generate the score using the LilyJazz font
instead of the default Emmentaler font.
This option needs lilyjazz.ily to be accessible to LilyPond
(https://github.com/OpenLilyPondFonts/lilyjazz).)",
        "fLilyJazz;",
        fLilyJazz));

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
        std::regex ("DEFAULT_VALUE"),
        std::to_string (fStaffGlobalSizeDefaultValue)),
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

//   const S_oahBooleanAtom& JMI v0.9.62
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

if (false) // JMI v0.9.67
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
          std::regex ("DEFAULT_VALUE"),
          msrSemiTonesPitchAndOctaveAsLilypondString (
            gLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind (),
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
        "fAllNotesDurations",
        fAllNotesDurations));

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
        "roman-std::string-numbers", "rsn",
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
              std::regex ("NUMBER"),
              std::to_string (gGlobalLpsrAccidentalStyleKindsMap.size ())),
            std::regex ("ACCIDENTAL_STYLES"),
            availableLpsrAccidentalStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
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
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
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
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
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
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        "fOriginalMeasureNumbers",
        fOriginalMeasureNumbers));

  // generate measure positions
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-measure-positions", "gmp",
        regex_replace (
R"(Generate after each note and barLine a comment containing
its position in the measure.
This is useful when debugging EXECUTABLE.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        "fGenerateMeasurePositions",
        fGenerateMeasurePositions));

  // generate measure moments
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-measure-moments", "gmim",
        regex_replace (
R"(Generate after each note and barLine a comment containing
its moment in the measure.
This is useful when debugging EXECUTABLE.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        "fGenerateMeasureMoments",
        fGenerateMeasureMoments));

  // non-print notes head color
  // --------------------------------------

  fNonPrintNotesHeadColorRGB =
    msrColorRGB (0.0, 0.0, 0.0); // black by default

  fNonPrintNotesHeadColorRGBAtom =
    oahColorRGBAtom::create (
      "non-print-notes-head-color", "npnhc",
R"(Use RGB_COLOR for non print notes head color,
for MusicXML's <note print-object="no"> for example.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "fNonPrintNotesHeadColorRGB",
      fNonPrintNotesHeadColorRGB);

  subGroup->
    appendAtomToSubGroup (
      fNonPrintNotesHeadColorRGBAtom);
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
      oahValueLessAtomAlias::create (
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
that can be generated in the LilyPond code in '% ...' comments
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
R"(Suppress the line breaks from the LPSR data - let LilyPond decide about them.)",
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

  fLedgerLinesColorRGB =
    msrColorRGB (0.0, 0.0, 0.0); // black by default

  fLedgerLinesColorRGBAtom =
    oahColorRGBAtom::create (
     "ledger-lines-color",  "llc",
R"(Use RGB_COLOR for the ledger lines.
RGB_COLOR should be of the form 'r,g,b',
with r, g and b being float numbers between 0.0 and 1.0 inclusive.)",
      "RGB_COLOR",
      "fLedgerLinesColorRGB",
      fLedgerLinesColorRGB);

  subGroup->
    appendAtomToSubGroup (
      fLedgerLinesColorRGBAtom);
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

  fDelayedOrnamentsFraction = mfRational (1, 2);

  std::string delayedOrnamentsFractionDefaultValue =
    std::to_string (fDelayedOrnamentsFraction.getNumerator ()) +
      '/' +
    std::to_string (fDelayedOrnamentsFraction.getDenominator ());

  subGroup->
    appendAtomToSubGroup (
      oahRationalAtom::create (
        "delayed-ornaments-fraction", "dof",
        regex_replace (
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default is 'DEFAULT_VALUE'.)",
          std::regex ("DEFAULT_VALUE"),
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
Including a closing parenthesis in a raw std::string:
  choose ### as the delimiter so only )###" ends the std::string:
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
          std::regex ("LILYPOND_CODE"),
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
            std::regex ("NUMBER"),
            std::to_string (getLpsrDynamicsTextSpannersStyleKindsMap ().size ())),
          std::regex ("LYRICS_DYNAMICS_TEXT_SPANNERS_STYLE_KINDS"),
          availableLpsrDynamicsTextSpannersStyleKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        std::regex ("DEFAULT_VALUE"),
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

  const lpsrLyricsNotesDurationsKind
    lpsrLyricsNotesDurationsKindDefaultValue =
      lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit; // default value

  fLyricsNotesDurationsKind = lpsrLyricsNotesDurationsKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      lilypondLyricsNotesDurationsKindAtom::create (
        "lilypond-lyrics-durations-style", "ld", // JMI
          regex_replace (
            regex_replace (
              regex_replace (
R"(The NUMBER LilyPond lyrics durations STYLEs available are:
LYRICS_DURATIONS_KINDS.
Using 'implicit' prevents the creation of lyrics attached to rests by LilyPond,
use 'explicit' in such cases.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (gGlobalLpsrLyricsNotesDurationsKindsMap.size ())),
            std::regex ("LYRICS_DURATIONS_KINDS"),
            availableLpsrLyricsNotesDurationsKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          lpsrLyricsNotesDurationsKindAsString (
            lpsrLyricsNotesDurationsKindDefaultValue)),
        "STYLE",
        "fLyricsNotesDurationsKind",
        fLyricsNotesDurationsKind));
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
        "lilypond-comments", "lpcoms",
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
        "generate-lpsr-visiting-information", "genlpsrvis",
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

  std::string midiTempoNotesDuration  = "8";
  int    midiTempoPerSecond = 180;

  fMidiTempo.setMidiTempoNotesDuration (midiTempoNotesDuration);
  fMidiTempo.setMidiTempoPerSecond (midiTempoPerSecond);

  std::string midiTempoDefaultValue =
    midiTempoNotesDuration + " = " + std::to_string (midiTempoPerSecond);

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
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        std::regex ("DEFAULT_VALUE"),
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
  std::map<std::string, lpsrAccidentalStyleKind>::const_iterator
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsr2lilypondOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsr2lilypondOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsr2lilypondOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsr2lilypondOahGroup>*> (v)) {
        S_lpsr2lilypondOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsr2lilypondOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsr2lilypondOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsr2lilypondOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondOahGroup::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os <<
    "The LilyPond lilypond are:" <<
    std::endl;

  ++gIndenter;

  // LilyPond version
  // --------------------------------------

  gLog <<
    "LilyPond version:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "lilyPondVersion" << ": " <<
    fLilypondVersion <<
    std::endl;

  --gIndenter;

  // identification
  // --------------------------------------
  os <<
    "Identification:" <<
    std::endl;

  ++gIndenter;

  // MusicXML informations
  os << std::left <<
    std::setw (valueFieldWidth) << "fRights" << ": " <<
      fRights <<
      std::endl <<

    std::setw (valueFieldWidth) << "fComposer" << ": " <<
      fComposer <<
      std::endl <<
    std::setw (valueFieldWidth) << "fArranger" << ": " <<
      fArranger <<
      std::endl <<
    std::setw (valueFieldWidth) << "fPoet" << ": " <<
      fPoet <<
      std::endl <<
    std::setw (valueFieldWidth) << "fLyricist" << ": " <<
      fLyricist <<
      std::endl <<

    std::setw (valueFieldWidth) << "fSoftware" << ": " <<
      fSoftware <<
      std::endl;

  // LilyPond informations
  os << std::left <<
    std::setw (valueFieldWidth) << "fDedication" << ": " <<
      fDedication <<
      std::endl <<

    std::setw (valueFieldWidth) << "fPiece" << ": " <<
      fPiece <<
      std::endl <<
    std::setw (valueFieldWidth) << "fOpus" << ": " <<
      fOpus <<
      std::endl <<

    std::setw (valueFieldWidth) << "fTitle" << ": " <<
      fTitle <<
      std::endl <<
    std::setw (valueFieldWidth) << "fSubTitle" << ": " <<
      fSubTitle <<
      std::endl <<
    std::setw (valueFieldWidth) << "fSubSubTitle" << ": " <<
      fSubSubTitle <<
      std::endl <<

    std::setw (valueFieldWidth) << "fHeaderInstrument" << ": " <<
      fHeaderInstrument <<
      std::endl <<

    std::setw (valueFieldWidth) << "fHeaderMeter" << ": " <<
      fHeaderMeter <<
      std::endl <<

    std::setw (valueFieldWidth) << "fTagline" << ": " <<
      fTagline <<
      std::endl <<

    std::setw (valueFieldWidth) << "fCopyright" << ": " <<
      fCopyright <<
      std::endl;

  --gIndenter;


  // parts
  // --------------------------------------

  // part names transposition

  {
    std::stringstream ss;

    ss << std::left <<
      std::setw (valueFieldWidth) << "fPartNamesTranspositionMap" << ": ";

    if (! fPartNamesTranspositionMap.size ()) {
      ss << "[NONE]";
    }
    else {
      for (
        std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
          fPartNamesTranspositionMap.begin ();
        i != fPartNamesTranspositionMap.end ();
        ++i
      ) {
        ss <<
          "Part name \"" << ((*i).first) <<
          "\": \"" <<
          ((*i).second->asString ()) <<
          "\" ";
      } // for

      gLog <<
        ss.str () <<
        std::endl;;
    }
  }

  // part IDs transposition

  {
    std::stringstream ss;

    ss << std::left <<
      std::setw (valueFieldWidth) << "fPartIDsTranspositionMap" << ": ";

    if (! fPartIDsTranspositionMap.size ()) {
      ss << "[EMPTY]";
    }
    else {
      for (
        std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator i =
          fPartIDsTranspositionMap.begin ();
        i != fPartIDsTranspositionMap.end ();
        ++i
      ) {
        ss <<
          "Part ID \"" << ((*i).first) <<
          "\": " <<
          ((*i).second->asString ()) <<
          "\" ";
      } // for

      gLog <<
        ss.str () <<
        std::endl;;
    }
  }

  // engravers
  // --------------------------------------
  os <<
    "Engravers:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fAmbitusEngraver" << ": " <<
      fAmbitusEngraver <<
      std::endl <<
    std::setw (valueFieldWidth) << "fCustosEngraver" << ": " <<
      fCustosEngraver <<
      std::endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  os <<
    "Clefs:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fCommentClefChanges" << ": " <<
      fCommentClefChanges <<
      std::endl;

  --gIndenter;


  // keys
  // --------------------------------------
  os <<
    "Clefs:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fCommentClefChanges" << ": " <<
      fCommentClefChanges <<
      std::endl;

  --gIndenter;


  // time signatures
  // --------------------------------------
  os <<
    "Times:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fNumericalTimeSignature" << ": " <<
      fNumericalTimeSignature <<
      std::endl;

  --gIndenter;


  // notes
  // --------------------------------------
  os <<
    "Notes:" <<
    std::endl;

  ++gIndenter;

  msrOctaveEntryKind
    octaveEntryKind =
      fetchOctaveEntryVariableValue ();

  os << std::left <<
    std::setw (valueFieldWidth) << "octaveEntryKind" << ": " <<
    msrOctaveEntryKindAsString (
      octaveEntryKind) <<
    std::endl;

  switch (octaveEntryKind) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      if (fRelativeOctaveEntrySemiTonesPitchAndOctave) {
        os << std::left <<
          std::setw (valueFieldWidth) <<
          "relativeOctaveEntrySemiTonesPitchAndOctave" << ": ";
/* JMI
        switch (fEarlyOptionKind) {
          case oahEarlyOptionKind::kEarlyOptionNo:
            break;
          case oahEarlyOptionKind::kEarlyOptionYes:
            os <<
              "fEarlyOptionKind: " <<
              fEarlyOptionKind <<
              std::endl;
            break;
        } // switch

        if (fSelected) {
          os <<
            ", fSelected: " <<
            fSelected;
        }
*/
        os <<
          fRelativeOctaveEntrySemiTonesPitchAndOctave->asString () <<
          std::endl;
      }
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      if (fFixedOctaveEntrySemiTonesPitchAndOctave) {
        os << std::left <<
          std::setw (valueFieldWidth) <<
          "fixedOctaveEntrySemiTonesPitchAndOctave" << ": ";
/* JMI
        switch (fEarlyOptionKind) {
          case oahEarlyOptionKind::kEarlyOptionNo:
            break;
          case oahEarlyOptionKind::kEarlyOptionYes:
            os <<
              "fEarlyOptionKind: " <<
              fEarlyOptionKind <<
              std::endl;
            break;
        } // switch

        if (fSelected) {
          os <<
            ", fSelected: " <<
            fSelected;
        }
*/
        os <<
          fFixedOctaveEntrySemiTonesPitchAndOctave->asString () <<
          std::endl;
      }
      break;
  } // switch

  os << std::left <<
    std::setw (valueFieldWidth) << "fInputLineNumbers" << ": " <<
      fAllNotesDurations <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNoAutoBeaming" << ": " <<
      fNoAutoBeaming <<
      std::endl <<
    std::setw (valueFieldWidth) << "fNoBeams" << ": " <<
      fNoBeams <<
      std::endl <<

    std::setw (valueFieldWidth) << "fRomanStringNumbers" << ": " <<
      fRomanStringNumbers <<
      std::endl <<
    std::setw (valueFieldWidth) << "fAvoidOpenStrings" << ": " <<
      fAvoidOpenStrings <<
      std::endl <<

    std::setw (valueFieldWidth) << "fAccidentalStyleKind" << ": " <<
      lpsrAccidentalStyleKindAsString (fAccidentalStyleKind) <<
      std::endl <<

    std::setw (valueFieldWidth) << "fLyricsNotesDurationsKind" << ": " <<
      lpsrLyricsNotesDurationsKindAsString (fLyricsNotesDurationsKind) <<
      std::endl <<

    std::setw (valueFieldWidth) << "fCompressFullBarRestsInLilypond" << ": " <<
      fCompressFullBarRestsInLilypond <<
      std::endl <<

    std::setw (valueFieldWidth) << "fMergeStaffCommonRests" << ": " <<
      fMergeStaffCommonRests <<
      std::endl <<

    std::setw (valueFieldWidth) << "fMultipleFullBarRestsExpandLimit" << ": " <<
      fMultipleFullBarRestsExpandLimit <<
      std::endl <<

    std::setw (valueFieldWidth) << "fInputLineNumbers" << ": " <<
      fInputLineNumbers <<
      std::endl <<
    std::setw (valueFieldWidth) << "fNotesComments" << ": " <<
      fNotesComments <<
      std::endl <<

    std::setw (valueFieldWidth) << "fOriginalMeasureNumbers" << ": " <<
      fOriginalMeasureNumbers <<
      std::endl <<

    std::setw (valueFieldWidth) << "fGenerateMeasurePositions" << ": " <<
      fGenerateMeasurePositions <<
      std::endl <<

    std::setw (valueFieldWidth) << "fGenerateMeasureMoments" << ": " <<
      fGenerateMeasureMoments <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNonPrintNotesHeadColorRGB" << ": " <<
      fNonPrintNotesHeadColorRGB.asString () <<
      std::endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  os <<
    "Bars:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fShowAllBarNumbers" << ": " <<
    fShowAllBarNumbers <<
    std::endl <<
    std::setw (valueFieldWidth) << "fBoxAroundAllBarNumbers" << ": " <<
    fBoxAroundAllBarNumbers <<
    std::endl;

  os << std::left <<
    std::setw (valueFieldWidth) <<
    fetchNamesBetweenQuotes () <<
    ": ";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    os << " empty";
  }
  else {
    std::map<std::string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << std::endl;

  os << std::left <<
    std::setw (valueFieldWidth) << "boxAroundBarNumberSet" << ": ";
  if (! fBoxAroundBarNumberSet.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << ",";
    } // for
  }
  os << std::endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  os <<
    "Line breaks:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreLpsrLineBreaks" << ": " <<
      fIgnoreLpsrLineBreaks <<
      std::endl <<

    std::setw (valueFieldWidth) << "fBreakLinesAtIncompleteRightMeasures" << ": " <<
      fBreakLinesAtIncompleteRightMeasures <<
      std::endl <<

    std::setw (valueFieldWidth) << "fSeparatorLineEveryNMeasures" << ": " <<
      fSeparatorLineEveryNMeasures <<
      std::endl;

  --gIndenter;


  // page and line breaks
  // --------------------------------------
  os <<
    "Page breaks:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreLpsrPageBreaks" << ": " <<
    fIgnoreLpsrPageBreaks <<
    std::endl;

  --gIndenter;


  // staves
  // --------------------------------------
  os <<
    "Staves:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fModernTab" << ": " <<
    fModernTab <<
    std::endl <<
    std::setw (valueFieldWidth) << "fTabFullNotation" << ": " <<
    fTabFullNotation <<
    std::endl <<

    std::setw (valueFieldWidth) << "fKeepStaffSize" << ": " <<
    fKeepStaffSize <<
    std::endl;

  --gIndenter;


  // chords
  // --------------------------------------

  os <<
    "Chords:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fConnectArpeggios" << ": " <<
    fConnectArpeggios <<
    std::endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  os <<
    "Tuplets:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fIndentTuplets" << ": " <<
    fIndentTuplets <<
    std::endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  os <<
    "Repeats:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fKeepRepeatBarLines" << ": " <<
    fKeepRepeatBarLines <<
    std::endl <<
    std::setw (valueFieldWidth) << "fRepeatBrackets" << ": " <<
    fRepeatBrackets <<
    std::endl <<
    std::setw (valueFieldWidth) << "fIgnoreRepeatNumbers" << ": " <<
    fIgnoreRepeatNumbers <<
    std::endl;

  --gIndenter;


  // lyrics
  // --------------------------------------

  gLog <<
    "Lyrics:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    std::setw (valueFieldWidth) << "fAddLilypondStanzasNumbers" << ": " <<
    fAddLilypondStanzasNumbers <<
    std::endl;

  --gIndenter;

  // ornaments
  // --------------------------------------

  os <<
    "Ornaments:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fDelayedOrnamentsFraction" << ": " <<
    fDelayedOrnamentsFraction <<
    std::endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  os <<
    "Chords display:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "chordsDisplayList" << ": ";

  if (fChordsDisplayList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<std::pair<std::string, std::string> >::const_iterator
      iBegin = fChordsDisplayList.begin (),
      iEnd   = fChordsDisplayList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        gTab <<
        (*i).first <<
        " -> " <<
        (*i).second <<
        std::endl;
      if (++i == iEnd) break;
  //     os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (valueFieldWidth) << "fJazzChordsDisplay" << ": " <<
    fJazzChordsDisplay <<
    std::endl <<
    std::setw (valueFieldWidth) << "fJazzChordsDisplayLilypondCode" << ": " <<
    fJazzChordsDisplayLilypondCode <<
    std::endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  os <<
    "Fonts:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fJazzFonts" << ": " <<
    fJazzFonts <<
    std::endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  os <<
    "LilyPond code generation:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fXml2lyInfos" << ": " <<
      fXml2lyInfos <<
      std::endl <<

    std::setw (valueFieldWidth) << "fLilypondComments" << ": " <<
      fLilypondComments <<
      std::endl <<

    std::setw (valueFieldWidth) << "fGlobal" << ": " <<
      fGlobal <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNoPaperBlock" << ": " <<
      fNoPaperBlock <<
      std::endl <<

    std::setw (valueFieldWidth) << "fDisplayMusic" << ": " <<
      fDisplayMusic <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNoLilypondCode" << ": " <<
      fNoLilypondCode <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNoLilypondLyrics" << ": " <<
      fNoLilypondLyrics <<
      std::endl <<

    std::setw (valueFieldWidth) << "fLilypondRunDate" << ": " <<
      fLilypondRunDate <<
      std::endl <<

    std::setw (valueFieldWidth) << "fDraftMode" << ": " <<
      fDraftMode <<
      std::endl <<

    std::setw (valueFieldWidth) << "fPointAndClickOff" << ": " <<
      fPointAndClickOff <<
      std::endl <<

    std::setw (valueFieldWidth) << "fGenerateStemsDirections" << ": " <<
      fGenerateStemsDirections <<
      std::endl <<

    std::setw (valueFieldWidth) << "fWhiteNoteHeads" << ": " <<
      fWhiteNoteHeads <<
      std::endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  os <<
    "Score notation:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fLilyJazz" << ": " <<
      fLilyJazz <<
      std::endl;

  os << std::left <<
    std::setw (valueFieldWidth) << "fJianpu" << ": " <<
      fJianpu <<
      std::endl;

  --gIndenter;


  // midi
  // --------------------------------------
  os <<
    "Midi:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (valueFieldWidth) << "fMidiTempo" << ": " <<
      fMidiTempo.asString () <<
      std::endl <<

    std::setw (valueFieldWidth) << "fNoMidi" << ": " <<
      fNoMidi <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

void lpsr2lilypondOahGroup::displayLilypondGenerationOahValues (int fieldWidth)
{
  gLog <<
    "The LilyPond lilypond are:" <<
    std::endl;

  ++gIndenter;

  // global staff size
  // --------------------------------------

  gLog <<
    "Global staff size:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fGlobalStaffSize" << ": " <<
    fGlobalStaffSize <<
    std::endl <<
    std::setw (fieldWidth) << "fStaffGlobalSizeDefaultValue" << ": " <<
    fStaffGlobalSizeDefaultValue <<
    std::endl;

  --gIndenter;

  // identification
  // --------------------------------------
  gLog <<
    "Identification:" <<
    std::endl;

  ++gIndenter;

  // MusicXML informations
  gLog << std::left <<
    std::setw (fieldWidth) << "fRights" << ": " <<
      fRights <<
      std::endl <<

    std::setw (fieldWidth) << "fComposer" << ": " <<
      fComposer <<
      std::endl <<
    std::setw (fieldWidth) << "fArranger" << ": " <<
      fArranger <<
      std::endl <<
    std::setw (fieldWidth) << "fPoet" << ": " <<
      fPoet <<
      std::endl <<
    std::setw (fieldWidth) << "fLyricist" << ": " <<
      fLyricist <<
      std::endl <<

    std::setw (fieldWidth) << "fSoftware" << ": " <<
      fSoftware <<
      std::endl <<

  // LilyPond informations
    std::setw (fieldWidth) << "fDedication" << ": " <<
      fDedication <<
      std::endl <<

    std::setw (fieldWidth) << "fPiece" << ": " <<
      fPiece <<
      std::endl <<
    std::setw (fieldWidth) << "fOpus" << ": " <<
      fOpus <<
      std::endl <<

    std::setw (fieldWidth) << "fTitle" << ": " <<
      fTitle <<
      std::endl <<
    std::setw (fieldWidth) << "fSubTitle" << ": " <<
      fSubTitle <<
      std::endl <<
    std::setw (fieldWidth) << "fSubSubTitle" << ": " <<
      fSubSubTitle <<
      std::endl <<
    std::setw (fieldWidth) << "fHeaderInstrument" << ": " <<
      fHeaderInstrument <<
      std::endl <<
    std::setw (fieldWidth) << "fHeaderMeter" << ": " <<
      fHeaderMeter <<
      std::endl <<

    std::setw (fieldWidth) << "fTagline" << ": " <<
      fTagline <<
      std::endl <<

    std::setw (fieldWidth) << "fCopyright" << ": " <<
      fCopyright <<
      std::endl;

  --gIndenter;


  // engravers
  // --------------------------------------
  gLog <<
    "Engravers:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fAmbitusEngraver" << ": " <<
      fAmbitusEngraver <<
      std::endl <<
    std::setw (fieldWidth) << "fCustosEngraver" << ": " <<
      fCustosEngraver <<
      std::endl;

  --gIndenter;


  // clefs
  // --------------------------------------
  gLog <<
    "Clefs:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fNoInitialTrebleClef" << ": " <<
      fNoInitialTrebleClef <<
      std::endl <<
    std::setw (fieldWidth) << "fCommentClefChanges" << ": " <<
      fCommentClefChanges <<
      std::endl;

  --gIndenter;


  // keys
  // --------------------------------------
  gLog <<
    "Keys:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fNoInitialCMajorKey" << ": " <<
      fNoInitialCMajorKey <<
      std::endl;

  --gIndenter;


  // time
  // --------------------------------------
  gLog <<
    "Times:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fNoInitialCommonTimeSignature" << ": " <<
      fNoInitialCommonTimeSignature <<
      std::endl <<
    std::setw (fieldWidth) << "fNumericalTimeSignature" << ": " <<
      fNumericalTimeSignature <<
      std::endl;

  --gIndenter;


  // notes
  // --------------------------------------
  gLog <<
    "Notes:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fOctaveEntryVariable" << ": " <<
      msrOctaveEntryKindAsString (
        fOctaveEntryVariable.getOctaveEntryKind ()) <<
      std::endl <<

    std::setw (fieldWidth) << "fAllNotesDurations" << ": " <<
      fAllNotesDurations <<
      std::endl <<

    std::setw (fieldWidth) << "fNoAutoBeaming" << ": " <<
      fNoAutoBeaming <<
      std::endl <<
    std::setw (fieldWidth) << "fNoBeams" << ": " <<
      fNoBeams <<
      std::endl <<

    std::setw (fieldWidth) << "fRomanStringNumbers" << ": " <<
      fRomanStringNumbers <<
      std::endl <<
    std::setw (fieldWidth) << "fAvoidOpenStrings" << ": " <<
      fAvoidOpenStrings <<
      std::endl <<

    std::setw (fieldWidth) << "fAccidentalStyleKind" << ": " <<
      lpsrAccidentalStyleKindAsString (fAccidentalStyleKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fLyricsNotesDurationsKind" << ": " <<
      lpsrLyricsNotesDurationsKindAsString (fLyricsNotesDurationsKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fCompressFullBarRestsInLilypond" << ": " <<
      fCompressFullBarRestsInLilypond <<
      std::endl <<

    std::setw (fieldWidth) << "fMergeStaffCommonRests" << ": " <<
      fMergeStaffCommonRests <<
      std::endl <<

    std::setw (fieldWidth) << "fMultipleFullBarRestsExpandLimit" << ": " <<
      fMultipleFullBarRestsExpandLimit <<
      std::endl <<

    std::setw (fieldWidth) << "fInputLineNumbers" << ": " <<
      fInputLineNumbers <<
      std::endl <<
    std::setw (fieldWidth) << "fNotesComments" << ": " <<
      fNotesComments <<
      std::endl <<

    std::setw (fieldWidth) << "fOriginalMeasureNumbers" << ": " <<
      fOriginalMeasureNumbers <<
      std::endl <<

    std::setw (fieldWidth) << "fGenerateMeasurePositions" << ": " <<
      fGenerateMeasurePositions <<
      std::endl <<

    std::setw (fieldWidth) << "fGenerateMeasureMoments" << ": " <<
      fGenerateMeasureMoments <<
      std::endl <<

    std::setw (fieldWidth) << "fNonPrintNotesHeadColorRGB" << ": " <<
      fNonPrintNotesHeadColorRGB.asString () <<
      std::endl;

  --gIndenter;


  // bars and measures
  // --------------------------------------
  gLog <<
    "Bars:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fShowAllBarNumbers" << ": " <<
    fShowAllBarNumbers <<
    std::endl <<
    std::setw (fieldWidth) << "fBoxAroundAllBarNumbers" << ": " <<
    fBoxAroundAllBarNumbers <<
    std::endl;

  gLog << std::left <<
    std::setw (fieldWidth) << "fResetMeasureElementMeasureNumberMap" << ": ";
  if (! fResetMeasureElementMeasureNumberMap.size ()) {
    gLog << "[EMPTY]";
  }
  else {
    std::map<std::string, int>::const_iterator
      iBegin = fResetMeasureElementMeasureNumberMap.begin (),
      iEnd   = fResetMeasureElementMeasureNumberMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLog << ",";
    } // for
  }
  gLog << std::endl;

  gLog << std::left <<
    std::setw (fieldWidth) << "fBoxAroundBarNumberSet" << ": ";
  if (! fBoxAroundBarNumberSet.size ()) {
    gLog << "[EMPTY]";
  }
  else {
    std::set<int>::const_iterator
      iBegin = fBoxAroundBarNumberSet.begin (),
      iEnd   = fBoxAroundBarNumberSet.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i);
      if (++i == iEnd) break;
      gLog << ",";
    } // for
  }
  gLog << std::endl;

  --gIndenter;


  // line breaks
  // --------------------------------------
  gLog <<
    "Line breaks:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fIgnoreLpsrLineBreaks" << ": " <<
      fIgnoreLpsrLineBreaks <<
      std::endl <<

    std::setw (fieldWidth) << "fBreakLinesAtIncompleteRightMeasures" << ": " <<
      fBreakLinesAtIncompleteRightMeasures <<
      std::endl <<

    std::setw (fieldWidth) << "fSeparatorLineEveryNMeasures" << ": " <<
      fSeparatorLineEveryNMeasures <<
      std::endl;

  --gIndenter;


  // page and line breaks
  // --------------------------------------
  gLog <<
    "Page breaks:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fIgnoreLpsrPageBreaks" << ": " <<
    fIgnoreLpsrPageBreaks <<
    std::endl;

  --gIndenter;


  // staves
  // --------------------------------------
  gLog <<
    "Staves:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fModernTab" << ": " <<
    fModernTab <<
    std::endl <<
    std::setw (fieldWidth) << "fTabFullNotation" << ": " <<
    fTabFullNotation <<
    std::endl;

  --gIndenter;


  // chords
  // --------------------------------------

  gLog <<
    "Chords:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fConnectArpeggios" << ": " <<
    fConnectArpeggios <<
    std::endl;

  --gIndenter;


  // tuplets
  // --------------------------------------

  gLog <<
    "Tuplets:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fIndentTuplets" << ": " <<
    fIndentTuplets <<
    std::endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  gLog <<
    "Repeats:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fKeepRepeatBarLines" << ": " <<
    fKeepRepeatBarLines <<
    std::endl <<
    std::setw (fieldWidth) << "fRepeatBrackets" << ": " <<
    fRepeatBrackets <<
    std::endl <<
    std::setw (fieldWidth) << "fIgnoreRepeatNumbers" << ": " <<
    fIgnoreRepeatNumbers <<
    std::endl;

  --gIndenter;


  // ornaments
  // --------------------------------------

  gLog <<
    "Ornaments:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fDelayedOrnamentsFraction" << ": " <<
    fDelayedOrnamentsFraction <<
    std::endl;

  --gIndenter;


  // chords display
  // --------------------------------------

  gLog <<
    "Chords display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fJazzChordsDisplay" << ": " <<
    fJazzChordsDisplay <<
    std::endl <<
    std::setw (fieldWidth) << "fJazzChordsDisplayLilypondCode" << ": " <<
    fJazzChordsDisplayLilypondCode <<
    std::endl;

  --gIndenter;


  // fonts
  // --------------------------------------

  gLog <<
    "Fonts:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fJazzFonts" << ": " <<
    fJazzFonts <<
    std::endl;

  --gIndenter;


  // code generation
  // --------------------------------------

  gLog <<
    "LilyPond code generation:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fXml2lyInfos" << ": " <<
      fXml2lyInfos <<
      std::endl <<

    std::setw (fieldWidth) << "fLilypondComments" << ": " <<
      fLilypondComments <<
      std::endl <<

    std::setw (fieldWidth) << "fGlobal" << ": " <<
      fGlobal <<
      std::endl <<

    std::setw (fieldWidth) << "fDisplayMusic" << ": " <<
      fDisplayMusic <<
      std::endl <<

    std::setw (fieldWidth) << "fNoLilypondCode" << ": " <<
      fNoLilypondCode <<
      std::endl <<

    std::setw (fieldWidth) << "fNoLilypondLyrics" << ": " <<
      fNoLilypondLyrics <<
      std::endl <<

    std::setw (fieldWidth) << "fLilypondRunDate" << ": " <<
      fLilypondRunDate <<
      std::endl <<

    std::setw (fieldWidth) << "fDraftMode" << ": " <<
      fDraftMode <<
      std::endl <<

    std::setw (fieldWidth) << "fPointAndClickOff" << ": " <<
      fPointAndClickOff <<
      std::endl <<

    std::setw (fieldWidth) << "fGenerateStemsDirections" << ": " <<
      fGenerateStemsDirections <<
      std::endl <<

    std::setw (fieldWidth) << "fWhiteNoteHeads" << ": " <<
      fWhiteNoteHeads <<
      std::endl <<

    std::setw (fieldWidth) << "fGenerateCommentedOutVariables" << ": " <<
      fGenerateCommentedOutVariables <<
      std::endl <<

//     std::setw (fieldWidth) << "fGenerateMsrVisitingInformation" << ": " <<
//       fGenerateMsrVisitingInformation <<
//       std::endl <<

    std::setw (fieldWidth) << "fGenerateLpsrVisitingInformation" << ": " <<
      fGenerateLpsrVisitingInformation <<
      std::endl;

  --gIndenter;


  // score notation
  // --------------------------------------

  gLog <<
    "Score notation:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fLilyJazz" << ": " <<
      fLilyJazz <<
      std::endl;

  gLog << std::left <<
    std::setw (fieldWidth) << "fJianpu" << ": " <<
      fJianpu <<
      std::endl;

  --gIndenter;


  // midi
  // --------------------------------------
  gLog <<
    "Midi:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fMidiTempo" << ": " <<
      fMidiTempo.asString () <<
      std::endl <<

    std::setw (fieldWidth) << "fNoMidi" << ": " <<
      fNoMidi <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsr2lilypondOahGroup& elt)
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
S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions () && ! gEarlyOptions.getEarlyQuietOption ()) {
    std::stringstream ss;

    ss <<
      "Initializing LilyPond OAH handling";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set<std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable)
{
  lilypondBreakPageAfterMeasureNumberAtom* obj = new
    lilypondBreakPageAfterMeasureNumberAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      fStringSetVariable);
  assert (obj != nullptr);
  return obj;
}

lilypondBreakPageAfterMeasureNumberAtom::lilypondBreakPageAfterMeasureNumberAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set<std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable)
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lilypondBreakPageAfterMeasureNumberAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // musicxmlMeasureNumber
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // lilypondMeasureNumber
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for reset measure number string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "':" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "-breakPageAfterMeasureNumber argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string musicxmlMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    std::stringstream ss;
    ss << sm [2];
    ss >> lilypondMeasureNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os <<
      "musicxmlMeasureNumber: " <<
      musicxmlMeasureNumber <<
      std::endl <<
      "lilypondMeasureNumber: " <<
      lilypondMeasureNumber <<
      std::endl;

  fStringSetVariable [musicxmlMeasureNumber] = lilypondMeasureNumber;
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;
}

void lilypondBreakPageAfterMeasureNumberAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lilypondBreakPageAfterMeasureNumberAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringSetVariable" << ": " <<
    std::endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map<std::string, int>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  os << std::endl;

  --gIndenter;
}

void lilypondBreakPageAfterMeasureNumberAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringSetVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map<std::string, int>::const_iterator
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
        ", fSelected: " <<
        fSelected <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}
*/

