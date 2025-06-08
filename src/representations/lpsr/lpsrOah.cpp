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
#include <string>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

// #include "mfConstants.h"
#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPitchesLanguageAtom lpsrPitchesLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  msrQuarterTonesPitchesLanguageKind&
                    lpsrPitchesLanguageKindVariable)
{
  lpsrPitchesLanguageAtom* obj = new
    lpsrPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrPitchesLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
}

lpsrPitchesLanguageAtom::lpsrPitchesLanguageAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the pitches languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrPitchesLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (
        theString);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map

    std::stringstream ss;

    ss <<
      "LPSR pitches language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known LPSR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setMsrQuarterTonesPitchesLanguageKindVariable (
    (*it).second);

  fSelected = true;
}

void lpsrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrPitchesLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrPitchesLanguageAtom>*> (v)) {
        S_lpsrPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrPitchesLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrPitchesLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lpsrPitchesLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

std::string lpsrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

void lpsrPitchesLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrPitchesLanguageAtom:" <<
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
    "fMsrQuarterTonesPitchesLanguageKindVariable" << ": " <<
    fMsrQuarterTonesPitchesLanguageKindVariable <<
    std::endl;

  --gIndenter;
}

void lpsrPitchesLanguageAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    fMsrQuarterTonesPitchesLanguageKindVariable;
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

std::ostream& operator << (std::ostream& os, const S_lpsrPitchesLanguageAtom& elt)
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
S_lpsrChordsLanguageAtom lpsrChordsLanguageAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  lpsrChordsLanguageKind&
                    lpsrChordsLanguageKindVariable)
{
  lpsrChordsLanguageAtom* obj = new
    lpsrChordsLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      lpsrChordsLanguageKindVariable);
  assert (obj != nullptr);
  return obj;
}

lpsrChordsLanguageAtom::lpsrChordsLanguageAtom (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrChordsLanguageAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <std::string, lpsrChordsLanguageKind>::const_iterator
    it =
      gGlobalLpsrChordsLanguageKindsMap.find (theString);

  if (it == gGlobalLpsrChordsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    std::stringstream ss;

    ss <<
      "LPSR chords language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalLpsrChordsLanguageKindsMap.size () - 1 <<
      " known LPSR chords languages apart from the default Ignatzek are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableLpsrChordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  setLpsrChordsLanguageKindVariable (
    (*it).second);

  fSelected = true;
}

void lpsrChordsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrChordsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrChordsLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrChordsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_lpsrChordsLanguageAtom>*> (v)) {
        S_lpsrChordsLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrChordsLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrChordsLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrChordsLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lpsrChordsLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return ss.str ();
}

std::string lpsrChordsLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    lpsrChordsLanguageKindAsString (fLpsrChordsLanguageKindVariable);

  return ss.str ();
}

void lpsrChordsLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrChordsLanguageAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fLpsrChordsLanguageKindVariable" << ": " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable) <<
    std::endl;

  --gIndenter;
}

void lpsrChordsLanguageAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKindVariable);
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

std::ostream& operator << (std::ostream& os, const S_lpsrChordsLanguageAtom& elt)
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
S_lpsrStaffInstrumentNameAtom lpsrStaffInstrumentNameAtom::create (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  std::map <std::string, std::string>&  stringToStringMapVariable)
{
  lpsrStaffInstrumentNameAtom* obj = new
    lpsrStaffInstrumentNameAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (obj != nullptr);
  return obj;
}

lpsrStaffInstrumentNameAtom::lpsrStaffInstrumentNameAtom (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    valueSpecification,
  const std::string&    variableName,
  std::map <std::string, std::string>&  stringToStringMapVariable)
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

lpsrStaffInstrumentNameAtom::~lpsrStaffInstrumentNameAtom ()
{}

void lpsrStaffInstrumentNameAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrStaffInstrumentNameAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the part rename specification
  // decipher it to extract the old and new part names

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrStaffInstrumentNameAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*(.+)[[:space:]]*"
    ":"
    "[[:space:]]*(.*)[[:space:]]*"   // the new name may be empty
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
      " for part rename string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "-lpsr-staff-instrument-name argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    partName = sm [1],
    partInstrumentName = sm [2];

  mfTrimFromBothEnds (partName);
  mfTrimFromBothEnds (partInstrumentName);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partName = \"" << partName << "\", " <<
      "--> partInstrumentName = \"" << partInstrumentName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::map <std::string, std::string>::iterator
    it =
      fStringToStringMapVariable.find (partName);

  if (it != fStringToStringMapVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part \"" << partName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringToStringMapVariable [partName] = partInstrumentName;
  }

  fSelected = true;
}

void lpsrStaffInstrumentNameAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrStaffInstrumentNameAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrStaffInstrumentNameAtom>*
    p =
      dynamic_cast<visitor<S_lpsrStaffInstrumentNameAtom>*> (v)) {
        S_lpsrStaffInstrumentNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrStaffInstrumentNameAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrStaffInstrumentNameAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrStaffInstrumentNameAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrStaffInstrumentNameAtom>*
    p =
      dynamic_cast<visitor<S_lpsrStaffInstrumentNameAtom>*> (v)) {
        S_lpsrStaffInstrumentNameAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrStaffInstrumentNameAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrStaffInstrumentNameAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrStaffInstrumentNameAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lpsrStaffInstrumentNameAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string lpsrStaffInstrumentNameAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringToStringMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void lpsrStaffInstrumentNameAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrStaffInstrumentNameAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringToStringMapVariable" << ": " <<
    std::endl;

  if (! fStringToStringMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
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

void lpsrStaffInstrumentNameAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fStringToStringMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map <std::string, std::string>::const_iterator
      iBegin = fStringToStringMapVariable.begin (),
      iEnd   = fStringToStringMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      std::string
        staffName           = (*i).first,
        staffInstrumentName = (*i).second;

      os <<
        "\"" <<
        staffName <<
        "\" --> \"";

      for (char ch : staffInstrumentName) {
        if (ch == '\n') {
          os << "EOLN";
        }
        else {
          os << ch;
        }
      }

      os <<
        "\"" <<
        std::endl;

      if (++i == iEnd) break;
    } // for

    os <<
      ", selected";

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_lpsrStaffInstrumentNameAtom& elt)
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
S_lpsrTransposeAtom lpsrTransposeAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  S_msrSemiTonesPitchAndOctave&
                     semiTonesPitchAndOctaveVariable)
{
  lpsrTransposeAtom* obj = new
    lpsrTransposeAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      semiTonesPitchAndOctaveVariable);
  assert (obj != nullptr);
  return obj;
}

lpsrTransposeAtom::lpsrTransposeAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrTransposeAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the language name:
  // is it in the chords languages map?

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'lpsrTransposeAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the semitones pitch and octave from theString
  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave =
      msrSemiTonesPitchAndOctave::createFromString (
        K_MF_INPUT_LINE_UNKNOWN_,
        theString);

  // set the transpose atom variable value
  setSemiTonesPitchAndOctaveVariable (
    semiTonesPitchAndOctave);

  fSelected = true;
}

void lpsrTransposeAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrTransposeAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrTransposeAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrTransposeAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrTransposeAtom>*
    p =
      dynamic_cast<visitor<S_lpsrTransposeAtom>*> (v)) {
        S_lpsrTransposeAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrTransposeAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrTransposeAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrTransposeAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string lpsrTransposeAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

std::string lpsrTransposeAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fSemiTonesPitchAndOctaveVariable->asString ();

  return ss.str ();
}

void lpsrTransposeAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "lpsrTransposeAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void lpsrTransposeAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";
  if (fSemiTonesPitchAndOctaveVariable) {
    os << std::endl;
    ++gIndenter;
    os <<
      fSemiTonesPitchAndOctaveVariable;
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
    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
}

std::ostream& operator << (std::ostream& os, const S_lpsrTransposeAtom& elt)
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
S_lpsrOahGroup lpsrOahGroup::create ()
{
  lpsrOahGroup* obj = new lpsrOahGroup ();
  assert (obj != nullptr);
  return obj;
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

  S_oahBooleanAtom
    displayLpsrBooleanAtom =
      oahBooleanAtom::create (
        "display-lpsr", "dlpsr",
R"(Write the contents of the LPSR data to standard error.)",
        "fDisplayLpsr",
        fDisplayLpsr);

  subGroup->
    appendAtomToSubGroup (
      displayLpsrBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-full", "dlpsrfull",
R"(Write the contents of the LPSR data with its full MSR component to standard error.)",
        "fDisplayLpsrFull",
        fDisplayLpsrFull));

  // display the MSR embeded in the LPSR

  S_oahBooleanAtom
    displayLpsrEmbeddedMsrBooleanAtom =
      oahBooleanAtom::create (
        "display-lpsr-embedded-msr", "dlpsrembedmsr",
R"(Write the contents of the MSR embedded in the LPSR data to standard error.)",
        "fDisplayLpsrEmbeddedMsr",
        fDisplayLpsrEmbeddedMsr);

  subGroup->
    appendAtomToSubGroup (
      displayLpsrEmbeddedMsrBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      oahValueLessAtomAlias::create (
        "display-msr3", "dmsr3",
R"(Write the MSR component of the LPSR data to standard error.
This option is an alias for '-dlpsrembedmsr, -dlpsrembedmsr'.)",
        displayLpsrEmbeddedMsrBooleanAtom));

  // display LPSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-names", "dlpsrnames",
R"(Only write a view of the names in the LPSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrNames",
        fDisplayLpsrNames));

  // display LPSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-summary", "dlpsrsum",
R"(Only write a summary of the LPSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrSummary",
        fDisplayLpsrSummary));

  // display LPSR flat view

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-voices-flat-view", "dlpsrvoicesflatview",
R"(Only write a voices flat view of the LPSR to standard error.
The output looks like:
  s4-unpitched c4-o:5 d4-o:5 e4-o:5
where the number after ':' is the octave number.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrFlatView",
        fDisplayLpsrFlatView));

  // display LPSR flat view Details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-voices-flat-view-details", "dlpsrvoicesflatviewd",
R"(Only write a voices flat view of the LPSR to standard error
with more details.
The output looks like:
  @0/1: s4-unpitched, line 52
  @1/4: c4-o:5, line 62
  @1/2: d4-o:5, line 72
  @3/4: e4-o:5, line 82
where the number after ':' is the octave number.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrFlatViewDetails",
        fDisplayLpsrFlatViewDetails));

  // display LPSR slices

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-measures-slices", "dlpsrmeasslices",
R"(Only write the measures slices of the LPSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrMeasuresSlices",
        fDisplayLpsrMeasuresSlices));

  // display LPSR slices details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-lpsr-measures-slices-details", "dlpsrmeasslicesd",
R"(Only write the measures slices of the LPSR to standard error
with more details.
This implies that no LilyPond code is generated.)",
        "fDisplayLpsrMeasuresSlicesDetails",
        fDisplayLpsrMeasuresSlicesDetails));
}

#ifdef MF_TRACE_IS_ENABLED
void lpsrOahGroup::initializeLpsrTraceOah ()
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

  // trace lpsr

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
#endif // MF_TRACE_IS_ENABLED

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
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsrLengthUnitKindsMap.size ())),
            std::regex ("LENTGTH_UNITS"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableMsrLengthUnitKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
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

  fPaperHorizontalShiftAtom =
    oahLengthAtom::create (
      "horizontal-shift", "hshift",
R"(Set the LilyPond 'horizontal-shift' paper variable to SHIFT in the LilyPond code.
SHIFT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
      "SHIFT",
      "fPaperHorizontalShift",
      fPaperHorizontalShift);

  subGroup->
    appendAtomToSubGroup (
      fPaperHorizontalShiftAtom);

  // indent
  // fPaperIndent is 0.0 mm by default

  fPaperIndentAtom =
    oahLengthAtom::create (
      "indent", "",
R"(Set the LilyPond 'indent' paper variable to INDENT in the LilyPond code.
INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
      "INDENT",
      "fPaperIndent",
      fPaperIndent);

  subGroup->
    appendAtomToSubGroup (
      fPaperIndentAtom);

  // short indent
  // fPaperShortIndent is 0.0 mm by default

  fPaperShortIndentAtom =
    oahLengthAtom::create (
      "short-indent", "",
R"(Set the LilyPond 'short-indent' paper variable to SHORT_INDENT in the LilyPond code.
SHORT_INDENT should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
The default value is 0.0 mm.)",
      "SHORT_INDENT",
      "fPaperShortIndent",
      fPaperShortIndent);

  subGroup->
    appendAtomToSubGroup (
      fPaperShortIndentAtom);

  // fMarkupSystemSpacingPadding is 0.0 mm by default

  fMarkupSystemSpacingPaddingAtom =
    oahLengthAtom::create (
      "markup-system-spacing-padding", "mssp",
R"(Set the LilyPond 'markup-system-spacing.padding' paper variable
to PADDING in the LilyPond code.
PADDING should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
LilyPond's default value is 0.0 mm.)",
      "PADDING",
      "fMarkupSystemSpacingPadding",
      fMarkupSystemSpacingPadding);

  subGroup->
    appendAtomToSubGroup (
      fMarkupSystemSpacingPaddingAtom);



  fBetweenSystemSpaceAtom =
    oahLengthAtom::create (
      "between-system-space", "bss",
R"(Set the LilyPond 'markup-system-spacing.padding' paper variable ??? JMI
to SPACE in the LilyPond code.
SPACE should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
LilyPond's default value is 0.0 mm.)",
      "SPACE",
      "fBetweenSystemSpace",
      fBetweenSystemSpace);

  subGroup->
    appendAtomToSubGroup (
      fBetweenSystemSpaceAtom);


  fPageTopSpacingAtom =
    oahLengthAtom::create (
      "page-top-spacing", "pts",
R"(Set the LilyPond 'top-system-spacing' paper variable ??? JMI
to SPACING in the LilyPond code.
PADDING should be a floating point or integer number,
immediately followed by a unit name, i.e. 'in', 'mm' or 'cm'.
LilyPond's default value is 0.0 mm.)",
      "SPACING",
      "fPageTopSpacing",
      fPageTopSpacing);

  subGroup->
    appendAtomToSubGroup (
      fPageTopSpacingAtom);



  // page count

  fPageCount = 0;

  fPageCountAtom =
    oahIntegerAtom::create (
      "page-count", "",
R"(Set the LilyPond 'page-count' paper variable to PAGE_COUNT in the LilyPond code.
PAGE_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
      "PAGE_COUNT",
      "fPageCount",
      fPageCount);

  subGroup->
    appendAtomToSubGroup (
      fPageCountAtom);

  // system count

  fSystemCount = 0;

  fSystemCountAtom =
    oahIntegerAtom::create (
      "system-count", "",
R"(Set the LilyPond 'system-count' paper variable to SYSTEM_COUNT in the LilyPond code.
SYSTEM_COUNT should be a positive integer.
By default, this is left to LilyPond'.)",
      "SYSTEM_COUNT",
      "fSystemCount",
      fSystemCount);

  subGroup->
    appendAtomToSubGroup (
      fSystemCountAtom);

  // ragged bottom

  fRaggedBottom = mfOnOffKind::kMfOnOffUnknown; // default value

  fRaggedBottomAtom =
    oahOnOffAtom::create (
      "ragged-bottom", "",
R"(Set the LilyPond \"ragged-bottom\" paper variable to '##t' or '##f',
if the value is on or off, respectively.
LilyPond's default value is '##t'.)",
      "on/off",
      "fRaggedBottom",
      fRaggedBottom);

  subGroup->
    appendAtomToSubGroup (
      fRaggedBottomAtom);

  // ragged last

  fRaggedLast = mfOnOffKind::kMfOnOffUnknown; // default value

  fRaggedLastAtom =
    oahOnOffAtom::create (
      "ragged-last", "",
R"(Set the LilyPond \"ragged-last\" paper variable to '##t' or '##f',
if the value is on or off, respectively.
LilyPond's default value is '##t'.)",
      "on/off", // JMI ???
      "fRaggedLast",
      fRaggedLast);

  subGroup->
    appendAtomToSubGroup (
      fRaggedLastAtom);

  // ragged last bottom

  fRaggedLastBottom = mfOnOffKind::kMfOnOffUnknown; // default value

  fRaggedLastBottomAtom =
    oahOnOffAtom::create (
      "ragged-last-bottom", "",
R"(Set the LilyPond \"ragged-last-bottom\" paper variable to '##t' or '##f',
if the value is on or off, respectively.
LilyPond's default value is '##t'.)",
      "on/off",
      "fRaggedLastBottom",
      fRaggedLastBottom);

  subGroup->
    appendAtomToSubGroup (
      fRaggedLastBottomAtom);

  // ragged right

  fRaggedRight = mfOnOffKind::kMfOnOffUnknown; // default value

  fRaggedRightAtom =
    oahOnOffAtom::create (
      "ragged-right", "",
R"(Set the LilyPond \"ragged-right\" paper variable to '##t' or '##f',
if the value is on or off, respectively.
LilyPond's default value is '##t'.)",
      "on/off",
      "fRaggedRight",
      fRaggedRight);

 subGroup->
    appendAtomToSubGroup (
      fRaggedRightAtom);

// // LPSR or LilyPond option?" JMI
//   // tagline
//
//   fTaglineAtom =
//     oahBooleanAtom::create (
//       "tagline", "",
// R"(Set the LilyPond 'tagline' paper variable to '##f' in the LilyPond code.
// LilyPond's default value is '##t'.)",
//       "fTagline",
//       fTagline);
//
//   subGroup->
//     appendAtomToSubGroup (
//       fTaglineAtom);
}

void lpsrOahGroup::initializeLpsrStavesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "help-lpsr-staves", "hlpstaves",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // LPSR instrument staff name

  fLpsrStavesInstrumentsNamesMapAtom =
    lpsrStaffInstrumentNameAtom::create (
      "lpsr-staff-instrument-name", "lpstaffinstrname",
      regex_replace (
R"(STAFF_INSTRUMENT_NAME_SPEC should be of the form STAFF:NAME.
Set the instrument name of staff STAFF to NAME, for example after displaying
the names in the score or a summary of the latter in a first run with options
'-display-msr1-names, -dmsr1names', '-display-msr2-names, -dmsr2names',
'-display-msr1-summary, -dmsr1sum' or '-display-msr2-summary, -dmsr2sum'.
'-display-lpsr-summary, -dlpsrsum' or '-display-lpsr-summary, -dlpsrsum'.
There can be spaces around the ':', in which case quoting is needed.
There can be several occurrences of this option.)",
        std::regex ("EXECUTABLE"),
        gOahOahGroup->getOahOahGroupServiceName ()),
      "STAFF_INSTRUMENT_NAME_SPEC",
      "fLpsrStavesInstrumentsNamesMap",
      fLpsrStavesInstrumentsNamesMap);

  subGroup->
    appendAtomToSubGroup (
      fLpsrStavesInstrumentsNamesMapAtom);
}

void lpsrOahGroup::initializeLpsrVoicesOptions ()
{
/* JMI
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "help-lpsr-voices", "hlpsvoices",
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
where MEASURE_NUMBER is a std::string, and REPLICATES is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and REPLICATES should be at least 1, , such as '17:3'.
There can be spaces around the ':', in which case quoting is needed.
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
        "LPSR languages",
        "help-lpsr-languages", "hlpsrl",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // lpsr pitches language

  if (
    ! setLpsrQuarterTonesPitchesLanguage ("nederlands")
  ) {
    std::stringstream ss;

    ss <<
      "INTERNAL INITIALIZATION ERROR: "
      "LPSR pitches language 'nederlands' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known LPSR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
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
              std::regex ("NUMBER"),
              std::to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            std::regex ("PITCHES_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
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
              std::regex ("NUMBER"),
              std::to_string (gGlobalLpsrChordsLanguageKindsMap.size ())),
            std::regex ("CHORDS_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableLpsrChordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
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
        "fTranspositionSemiTonesPitchAndOctave",
        fTranspositionSemiTonesPitchAndOctave));
}

void lpsrOahGroup::initializeLpsrOahGroup ()
{
  // display
  // --------------------------------------
  initializeLpsrDisplayOptions ();

#ifdef MF_TRACE_IS_ENABLED
  // trace and display
  // --------------------------------------
  initializeLpsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED

  // paper
  // --------------------------------------
  initializeLpsrPaperOptions ();

  // staves
  // --------------------------------------
  initializeLpsrStavesOptions ();

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
  // transposition
  // --------------------------------------
  initializeLpsrTransposeOptions ();
*/
}

//______________________________________________________________________________
Bool lpsrOahGroup::setLpsrQuarterTonesPitchesLanguage (std::string language)
{
  // is language in the note names languages map?
  std::map <std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
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
Bool lpsrOahGroup::setLpsrChordsLanguageKind (std::string language)
{
  // is language in the chords languages map?
  std::map <std::string, lpsrChordsLanguageKind>::const_iterator
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
#ifdef MF_TRACE_IS_ENABLED
  fTraceLpsr = false;
  fTraceLilypondVersion = false;
  fTraceLpsrVisitors = false;
  fTraceLpsrBlocks = false;
  fTraceSchemeFunctions = false;
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void lpsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void lpsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrOahGroup>*
    p =
      dynamic_cast<visitor<S_lpsrOahGroup>*> (v)) {
        S_lpsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching lpsrOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> lpsrOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void lpsrOahGroup::displayLpsrOahValues (int fieldWidth)
{
  gLog <<
    "The LPSR options are:" <<
    std::endl;

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTraceLpsr" << ": " <<
    fTraceLpsr <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceLilypondVersion" << ": " <<
    fTraceLilypondVersion <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceLpsrVisitors" << ": " <<
    fTraceLpsrVisitors <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceLpsrBlocks" << ": " <<
    fTraceLpsrBlocks <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceSchemeFunctions" << ": " <<
    fTraceSchemeFunctions <<
    std::endl;

  --gIndenter;
#endif // MF_TRACE_IS_ENABLED

  // display
  // --------------------------------------
  gLog <<
    "Display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fDisplayLpsr" << ": " <<
    fDisplayLpsr <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplayLpsrFull" << ": " <<
    fDisplayLpsrFull <<
    std::endl <<

    std::setw (fieldWidth) << "fDisplayLpsrNames" << ": " <<
    fDisplayLpsrNames <<
    std::endl <<

    std::setw (fieldWidth) << "fDisplayLpsrSummary" << ": " <<
    fDisplayLpsrSummary <<
    std::endl <<

    std::setw (fieldWidth) << "fDisplayLpsrFlatView" << ": " <<
    fDisplayLpsrFlatView <<
    std::endl <<

    std::setw (fieldWidth) << "fDisplayLpsrMeasuresSlices" << ": " <<
    fDisplayLpsrMeasuresSlices <<
    std::endl;

  --gIndenter;

  // paper
  // --------------------------------------

  gLog <<
    "Paper:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fAllPaperVariables" << ": " <<
    fAllPaperVariables <<
    std::endl <<

    std::setw (fieldWidth) << "fLengthUnitKind" << ": " <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    std::endl <<

    std::setw (fieldWidth) << "fPaperHeight" << ": " <<
    fPaperHeight.asString () <<
    std::endl <<
    std::setw (fieldWidth) << "fPaperWidth" << ": " <<
    fPaperWidth.asString () <<
    std::endl <<

    std::setw (fieldWidth) << "paperfPaperLeftMarginLeftMargin" << ": " <<
    fPaperLeftMargin.asString () <<
    std::endl <<
    std::setw (fieldWidth) << "fPaperRightMargin" << ": " <<
    fPaperRightMargin.asString () <<
    std::endl <<
    std::setw (fieldWidth) << "fPaperTopMargin" << ": " <<
    fPaperTopMargin.asString () <<
    std::endl <<
    std::setw (fieldWidth) << "fPaperBottomMargin" << ": " <<
    fPaperBottomMargin.asString () <<
    std::endl <<

    std::setw (fieldWidth) << "fPaperIndent" << ": " <<
    fPaperIndent.asString () <<
    std::endl <<
    std::setw (fieldWidth) << "fPaperShortIndent" << ": " <<
    fPaperShortIndent.asString () <<
    std::endl <<

    std::setw (fieldWidth) << "fMarkupSystemSpacingPadding" << ": " <<
    fMarkupSystemSpacingPadding.asString () <<
    std::endl <<

    std::setw (fieldWidth) << "fRaggedBottom" << ": " <<
    fRaggedBottom <<
    std::endl <<
    std::setw (fieldWidth) << "fRaggedLast" << ": " <<
    fRaggedLast <<
    std::endl <<
    std::setw (fieldWidth) << "fRaggedLastBottom" << ": " <<
    fRaggedLastBottom <<
    std::endl <<
    std::setw (fieldWidth) << "fRaggedRight" << ": " <<
    fRaggedRight <<
    std::endl <<

    std::setw (fieldWidth) << "fTagline" << ": " <<
    fTagline <<
    std::endl <<

    std::setw (fieldWidth) << "fPageCount" << ": " <<
    fPageCount <<
    std::endl <<
    std::setw (fieldWidth) << "fSystemCount" << ": " <<
    fSystemCount <<
    std::endl;

  --gIndenter;

/* JMI
  // measures
  // --------------------------------------

  gLog <<
    "Measures:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    fetchNamesBetweenQuotes () <<
    ": ";
  if (! fAddEmptyMeasuresStringToIntMap.size ()) {
    gLog << "[EMPTY]";
  }
  else {
    std::map <std::string, int>::const_iterator
      iBegin = fAddEmptyMeasuresStringToIntMap.begin (),
      iEnd   = fAddEmptyMeasuresStringToIntMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      gLog << ",";
    } // for
  }
  gLog << std::endl;

  --gIndenter;
*/

  // tempos
  // --------------------------------------

  gLog <<
    "Tempos:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fConvertMusicXMLTemposToMsrRehearsalMarks" << ": " <<
    fConvertMusicXMLTemposToMsrRehearsalMarks <<
    std::endl << std::endl;

  --gIndenter;

  // words
  // --------------------------------------

  gLog <<
    "Words:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fConvertLpsrWordsToTempo" << ": " <<
    fConvertLpsrWordsToTempo <<
    std::endl <<
    std::setw (fieldWidth) << "fAddLpsrWordsFromTheLyrics" << ": " <<
    fAddLpsrWordsFromTheLyrics <<
    std::endl <<

    std::setw (fieldWidth) << "fConvertLpsrWordsToRehearsalMarks" << ": " <<
    fConvertLpsrWordsToRehearsalMarks <<
    std::endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLog <<
    "Languages:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fLpsrQuarterTonesPitchesLanguageKind" << ": " <<
    fLpsrQuarterTonesPitchesLanguageKind <<
    std::endl <<

    std::setw (fieldWidth) << "fLpsrChordsLanguageKind" << ": " <<
    lpsrChordsLanguageKindAsString (
      fLpsrChordsLanguageKind) <<
    std::endl;

  --gIndenter;

  // transposition
  // --------------------------------------

  gLog <<
    "Transpose:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTranspositionSemiTonesPitchAndOctave" << ": ";

    if (fTranspositionSemiTonesPitchAndOctave) {
      gLog <<
        fTranspositionSemiTonesPitchAndOctave->asString ();
    }
    else {
      gLog <<
        "[NULL]";
    }
  gLog << std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrOahGroup& elt)
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
// hidden global LPSR OAH group ariable
EXP S_lpsrOahGroup pGlobalLpsrOahGroup;

EXP S_lpsrOahGroup getGlobalLpsrOahGroup ()
{
  return pGlobalLpsrOahGroup;
}

//______________________________________________________________________________
S_lpsrOahGroup createGlobalLpsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global LPSR OAH group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalLpsrOahGroup) {
    // create the global options group
    pGlobalLpsrOahGroup =
      lpsrOahGroup::create ();
    assert (pGlobalLpsrOahGroup != 0);
  }

  // return the global OAH group
  return pGlobalLpsrOahGroup;
}


}
