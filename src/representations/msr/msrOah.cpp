/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...
#include <sstream>

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "mfConstants.h"
#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constants
const std::string K_MSR_GENERATION_API_KIND_LONG_NAME  = "msr-generation-api-kind";
const std::string K_MSR_GENERATION_API_KIND_SHORT_NAME = "msr-gen-api";

S_msrPitchesLanguageAtom msrPitchesLanguageAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  msrQuarterTonesPitchesLanguageKind&
                     quarterTonesPitchesLanguageKind)
{
  msrPitchesLanguageAtom* obj = new
    msrPitchesLanguageAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      quarterTonesPitchesLanguageKind);
  assert (obj != nullptr);
  return obj;
}

msrPitchesLanguageAtom::msrPitchesLanguageAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msrPitchesLanguageAtom'";

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
      "==> oahAtom is of type 'msrPitchesLanguageAtom'";

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
      "MSR pitches language '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSR pitches languages are:" <<
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

void msrPitchesLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrPitchesLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrPitchesLanguageAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPitchesLanguageAtom>*
    p =
      dynamic_cast<visitor<S_msrPitchesLanguageAtom>*> (v)) {
        S_msrPitchesLanguageAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrPitchesLanguageAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrPitchesLanguageAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrPitchesLanguageAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrPitchesLanguageAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

std::string msrPitchesLanguageAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ' <<
    fMsrQuarterTonesPitchesLanguageKindVariable;

  return ss.str ();
}

void msrPitchesLanguageAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrPitchesLanguageAtom:" <<
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

void msrPitchesLanguageAtom::displayAtomWithVariableOptionsValues (
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

std::ostream& operator << (std::ostream& os, const S_msrPitchesLanguageAtom& elt)
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
S_msrRenamePartAtom msrRenamePartAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map <std::string, std::string>&
                     stringToStringMapVariable)
{
  msrRenamePartAtom* obj = new
    msrRenamePartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringToStringMapVariable);
  assert (obj != nullptr);
  return obj;
}

msrRenamePartAtom::msrRenamePartAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::map <std::string, std::string>&
                     stringToStringMapVariable)
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msrRenamePartAtom'";

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
      "==> oahAtom is of type 'msrRenamePartAtom'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*(.+)[[:space:]]*"
    ":"
    "[[:space:]]*(.+)[[:space:]]*"
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
      "-msrPartRename argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    oldPartName = sm [1],
    newPartName = sm [2];

  mfTrimFromBothEnds (oldPartName);
  mfTrimFromBothEnds (newPartName);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> oldPartName = \"" << oldPartName << "\", " <<
      "--> newPartName = \"" << newPartName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::map <std::string, std::string>::iterator
    it =
      fStringToStringMapVariable.find (oldPartName);

  if (it != fStringToStringMapVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part \"" << oldPartName << "\" occurs more that once in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringToStringMapVariable [oldPartName] = newPartName;
  }

  fSelected = true;
}

void msrRenamePartAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrRenamePartAtom::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrRenamePartAtom::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRenamePartAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrRenamePartAtom::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRenamePartAtom>*
    p =
      dynamic_cast<visitor<S_msrRenamePartAtom>*> (v)) {
        S_msrRenamePartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrRenamePartAtom::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRenamePartAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrRenamePartAtom::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrRenamePartAtom::asShortNamedOptionString () const
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

std::string msrRenamePartAtom::asActualLongNamedOptionString () const
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

void msrRenamePartAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrRenamePartAtom:" <<
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

void msrRenamePartAtom::displayAtomWithVariableOptionsValues (
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
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", selected";

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_msrRenamePartAtom& elt)
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
S_msrOahGroup gMsrOahGroup;

S_msrOahGroup msrOahGroup::create ()
{
  msrOahGroup* obj = new msrOahGroup ();
  assert (obj != nullptr);
  return obj;
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
        "display-part-groups", "dpartgroups",
R"(Write the structure of the part groups to standard error.)",
        "fDisplayPartGroups",
        fDisplayPartGroups));

  // display MSR skeleton

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-skeleton", "dmsr1skel",
R"(Write the contents of the first MSR skeleton data to standard error.)",
        "fDisplayMsrSkeleton",
        fDisplayMsrSkeleton));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-skeleton-full", "dmsr1skelfull",
R"(Write the contents of the first MSR skeleton data, full version, to standard error.)",
        "fDisplayMsrSkeletonFull",
        fDisplayMsrSkeletonFull));

  // display first MSR

  S_oahBooleanAtom
    displayFirstMsrBooleanAtom =
      oahBooleanAtom::create (
        "display-msr1", "dmsr1",
R"(Write the contents of the first MSR data to standard error.)",
        "fDisplayFirstMsr",
        fDisplayFirstMsr);

  subGroup->
    appendAtomToSubGroup (
      displayFirstMsrBooleanAtom);

  S_oahBooleanAtom
    displayFirstMsrFullBooleanAtom =
      oahBooleanAtom::create (
        "display-msr1-full", "dmsr1full",
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
        "display-msr1-measures-slices", "dmsr1measslices",
R"(Write the contents of the first MSR data slices to standard error.)",
        "fDisplayFirstMsrMeasuresSlices",
        fDisplayFirstMsrMeasuresSlices));

  // display first MSR slices details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-measures-slices-details", "dmsr1measslicesd",
R"(Write the contents of the first MSR data slices to standard error.)",
        "fDisplayFirstMsrMeasuresSlicesDetails",
        fDisplayFirstMsrMeasuresSlicesDetails));

  // display second MSR

  S_oahBooleanAtom
    displaySecondMsrBooleanAtom =
      oahBooleanAtom::create (
        "display-msr2", "dmsr2",
R"(Write the contents of the second MSR data to standard error.)",
        "fDisplaySecondMsr",
        fDisplaySecondMsr);

  subGroup->
    appendAtomToSubGroup (
      displaySecondMsrBooleanAtom);

  // display second MSR full

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-full", "dmsr2full",
R"(Write the contents of the second MSR data, full version, to standard error.)",
        "fDisplaySecondMsrFull",
        fDisplaySecondMsrFull));

  // display second MSR slices

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-measures-slices", "dmsr2measslices",
R"(Write the contents of the second MSR data slices to standard error.)",
        "fDisplaySecondMsrMeasuresSlices",
        fDisplaySecondMsrMeasuresSlices));

  // display second MSR slices details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-measures-slices-details", "dmsr2measslicesd",
R"(Write the contents of the second MSR data slices to standard error.)",
        "fDisplaySecondMsrMeasuresSlicesDetails",
        fDisplaySecondMsrMeasuresSlicesDetails));

  // display first MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-names", "dmsr1names",
R"(Only write a view of the names in the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrNames",
        fDisplayFirstMsrNames));

  // display second MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-names", "dmsr2names",
R"(Only write a view of the names in the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrNames",
        fDisplaySecondMsrNames));

  // display first MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-summary", "dmsr1sum",
R"(Only write a summary of the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrSummary",
        fDisplayFirstMsrSummary));

  // display second MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-summary", "dmsr2sum",
R"(Only write a summary of the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrSummary",
        fDisplaySecondMsrSummary));

  // display first MSR flat view

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-voices-flat-view", "dmsr1voicesflatview",
R"(Only write a voices flat view of the first MSR to standard error.
The output looks like:
  s4-unpitched c4-o:5 d4-o:5 e4-o:5
where the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrFlatView",
        fDisplayFirstMsrFlatView));

  // display first MSR flat view details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-voices-flat-view-details", "dmsr1voicesflatviewd",
R"(Only write a voices flat view of the first MSR to standard error,
with more details.
The output looks like:
  @0/1: s4-unpitched, line 52
  @1/4: c4-o:5, line 62
  @1/2: d4-o:5, line 72
  @3/4: e4-o:5, line 82
where '@' indicates the position in the measure
and the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrFlatViewDetails",
        fDisplayFirstMsrFlatViewDetails));

  // display second MSR flat view

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-voices-flat-view", "dmsr2voicesflatview",
R"(Only write a voices flat view of the second MSR to standard error.
The output looks like:
  s4-unpitched c4-o:5 d4-o:5 e4-o:5
where the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrFlatView",
        fDisplaySecondMsrFlatView));

  // display second MSR flat view details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-voices-flat-view-details", "dmsr2voicesflatviewd",
R"(Only write a voices flat view of the second MSR to standard error,
with more details.
The output looks like:
  @0/1: s4-unpitched, line 52
  @1/4: c4-o:5, line 62
  @1/2: d4-o:5, line 72
  @3/4: e4-o:5, line 82
where '@' indicates the position in the measure
and the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrFlatViewDetails",
        fDisplaySecondMsrFlatViewDetails));
}

#ifdef MF_TRACE_IS_ENABLED
void msrOahGroup::initializeMsrTraceOah ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Display",
      "help-msr-trace-and-display", "hmsrtd",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR visiting

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-msr-visitors", "tmsrvis",
R"(Write a trace of the MSR visiting activity to standard error.)",
        "fTraceMsrVisitors",
        fTraceMsrVisitors));

  // display partgroups

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-part-groups", "dpartgroups",
R"(Write the structure of the part groups to standard error.)",
        "fDisplayPartGroups",
        fDisplayPartGroups));

  // display MSR skeleton

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-skeleton", "dmsr1skel",
R"(Write the contents of the first MSR skeleton data to standard error.)",
        "fDisplayMsrSkeleton",
        fDisplayMsrSkeleton));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-skeleton-full", "dmsr1skelfull",
R"(Write the contents of the first MSR skeleton data, full version, to standard error.)",
        "fDisplayMsrSkeletonFull",
        fDisplayMsrSkeletonFull));

  // display first MSR

  S_oahBooleanAtom
    displayFirstMsrBooleanAtom =
      oahBooleanAtom::create (
        "display-msr1", "dmsr1",
R"(Write the contents of the first MSR data to standard error.)",
        "fDisplayFirstMsr",
        fDisplayFirstMsr);

  subGroup->
    appendAtomToSubGroup (
      displayFirstMsrBooleanAtom);

  S_oahBooleanAtom
    displayFirstMsrFullBooleanAtom =
      oahBooleanAtom::create (
        "display-msr1-full", "dmsr1full",
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
        "display-msr1-measures-slices", "dmsr1measslices",
R"(Write the contents of the first MSR data slices to standard error.)",
        "fDisplayFirstMsrMeasuresSlices",
        fDisplayFirstMsrMeasuresSlices));

  // display first MSR slices details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-measures-slices-details", "dmsr1measslicesd",
R"(Write the contents of the first MSR data slices to standard error.)",
        "fDisplayFirstMsrMeasuresSlicesDetails",
        fDisplayFirstMsrMeasuresSlicesDetails));

  // display second MSR

  S_oahBooleanAtom
    displaySecondMsrBooleanAtom =
      oahBooleanAtom::create (
        "display-msr2", "dmsr2",
R"(Write the contents of the second MSR data to standard error.)",
        "fDisplaySecondMsr",
        fDisplaySecondMsr);

  subGroup->
    appendAtomToSubGroup (
      displaySecondMsrBooleanAtom);

  // display second MSR full

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-full", "dmsr2full",
R"(Write the contents of the second MSR data, full version, to standard error.)",
        "fDisplaySecondMsrFull",
        fDisplaySecondMsrFull));

  // display second MSR slices

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-measures-slices", "dmsr2measslices",
R"(Write the contents of the second MSR data slices to standard error.)",
        "fDisplaySecondMsrMeasuresSlices",
        fDisplaySecondMsrMeasuresSlices));

  // display second MSR slices details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-measures-slices-details", "dmsr2measslicesd",
R"(Write the contents of the second MSR data slices to standard error.)",
        "fDisplaySecondMsrMeasuresSlicesDetails",
        fDisplaySecondMsrMeasuresSlicesDetails));

  // display first MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-names", "dmsr1names",
R"(Only write a view of the names in the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrNames",
        fDisplayFirstMsrNames));

  // display second MSR names

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-names", "dmsr2names",
R"(Only write a view of the names in the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrNames",
        fDisplaySecondMsrNames));

  // display first MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-summary", "dmsr1sum",
R"(Only write a summary of the first MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrSummary",
        fDisplayFirstMsrSummary));

  // display second MSR summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-summary", "dmsr2sum",
R"(Only write a summary of the second MSR to standard error.
This implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrSummary",
        fDisplaySecondMsrSummary));

  // display first MSR flat view

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-voices-flat-view", "dmsr1voicesflatview",
R"(Only write a voices flat view of the first MSR to standard error.
The output looks like:
  s4-unpitched c4-o:5 d4-o:5 e4-o:5
where the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrFlatView",
        fDisplayFirstMsrFlatView));

  // display first MSR flat view details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr1-voices-flat-view-details", "dmsr1voicesflatviewd",
R"(Only write a voices flat view of the first MSR to standard error,
with more details.
The output looks like:
  @0/1: s4-unpitched, line 52
  @1/4: c4-o:5, line 62
  @1/2: d4-o:5, line 72
  @3/4: e4-o:5, line 82
where '@' indicates the position in the measure
and the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplayFirstMsrFlatViewDetails",
        fDisplayFirstMsrFlatViewDetails));

  // display second MSR flat view

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-voices-flat-view", "dmsr2voicesflatview",
R"(Only write a voices flat view of the second MSR to standard error.
The output looks like:
  s4-unpitched c4-o:5 d4-o:5 e4-o:5
where the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrFlatView",
        fDisplaySecondMsrFlatView));

  // display second MSR flat view details

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-msr2-voices-flat-view-details", "dmsr2voicesflatviewd",
R"(Only write a voices flat view of the second MSR to standard error,
with more details.
The output looks like:
  @0/1: s4-unpitched, line 52
  @1/4: c4-o:5, line 62
  @1/2: d4-o:5, line 72
  @3/4: e4-o:5, line 82
where '@' indicates the position in the measure
and the number after ':' is the octave number.
This option implies that no LilyPond code is generated.)",
        "fDisplaySecondMsrFlatViewDetails",
        fDisplaySecondMsrFlatViewDetails));
}
#endif // MF_TRACE_IS_ENABLED

void msrOahGroup::initializeMsrLanguagesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "MSR languages",
      "help-msr-languages", "hmsrlang",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // notes pitches

  if (! setMsrQuarterTonesPitchesLanguage ("nederlands")) {
    std::stringstream ss;

    ss <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSR pitches language 'nederlands' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSR pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
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
        "msr-rename-part", "mrenamepart",
        regex_replace (
R"(PART_RENAME_SPEC should be of the form ORIGINAL_NAME:NEW_NAME.
Rename part ORIGINAL_NAME to NEW_NAME, for example after displaying
the names in the score or a summary of the latter in a first run with options
'-dmsrnames, -display-msr-names' or 'dmsrsum, -display-msr-summary'.
There can be spaces around the ':', in which case quoting is needed.
There can be several occurrences of this option.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
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
        "show-harmony-voices", "sharmvoices",
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
        "show-figured-bass-voices", "sfigbassvoices",
R"(Show the figured bass voices in the MSR data
even though they do not contain music.)",
        "fShowFiguredBassVoices",
        fShowFiguredBassVoices));
}

void msrOahGroup::initializeMsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED

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
Bool msrOahGroup::setMsrQuarterTonesPitchesLanguage (const std::string& language)
{
  // is language in the pitches languages map?
  std::map <std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msrOahGroup>*> (v)) {
        S_msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msrOahGroup::displayMsrOahValues (int valueFieldWidth) // JMI SUPERFLOUS??? 0.9.69
{
  gLog <<
    "The MSR options are:" <<
    std::endl;

  ++gIndenter;

  // trace and display
  // --------------------------------------

  gLog <<
    "Trace and display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fTraceMsrVisitors" << ": " <<
    fTraceMsrVisitors <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayPartGroups" << ": " <<
    fDisplayPartGroups <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayFirstMsr" << ": " <<
    fDisplayFirstMsr <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplaySecondMsr" << ": " <<
    fDisplaySecondMsr <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayFirstMsrFull" << ": " <<
    fDisplayFirstMsrFull <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplaySecondMsrFull" << ": " <<
    fDisplaySecondMsrFull <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayFirstMsrNames" << ": " <<
    fDisplayFirstMsrNames <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplaySecondMsrNames" << ": " <<
    fDisplaySecondMsrNames <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayFirstMsrSummary" << ": " <<
    fDisplayFirstMsrSummary <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplaySecondMsrSummary" << ": " <<
    fDisplaySecondMsrSummary <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayFirstMsrFlatView" << ": " <<
    fDisplayFirstMsrFlatView <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayFirstMsrFlatViewDetails" << ": " <<
    fDisplayFirstMsrFlatViewDetails <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplaySecondMsrFlatView" << ": " <<
    fDisplaySecondMsrFlatView <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplaySecondMsrFlatViewDetails" << ": " <<
    fDisplaySecondMsrFlatViewDetails <<
    std::endl;

  --gIndenter;

  // languages
  // --------------------------------------

  gLog <<
     "Languages:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "msrPitchesLanguage" << ": " <<
    fMsrQuarterTonesPitchesLanguageKind <<
    std::endl;

  --gIndenter;

  // parts
  // --------------------------------------

  gLog <<
     "Parts:" <<
    std::endl;

  ++gIndenter;

  // parts renaming

  gLog << std::left <<
    std::setw (valueFieldWidth) << "parts renaming" << ": ";

  if (! fMsrPartsRenamingMap.size ()) {
    gLog <<
      "[EMPTY]";
  }
  else {
    for (
      std::map <std::string, std::string>::const_iterator i =
        fMsrPartsRenamingMap.begin ();
      i != fMsrPartsRenamingMap.end ();
      ++i
  ) {
        gLog <<
          "\"" << ((*i).first) << "\" -> \"" << ((*i).second) << "\"";
    } // for
  }

  gLog << std::endl;

  --gIndenter;

  // staves
  // --------------------------------------

  gLog <<
    "Staves:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fCreateSingleLineStavesAsRythmic" << ": " <<
    fCreateSingleLineStavesAsRythmic <<
    std::endl;

  --gIndenter;

  // voices
  // --------------------------------------

  gLog <<
    "Voices:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fCreateVoicesStaffRelativeNumbers" << ": " <<
    fCreateVoicesStaffRelativeNumbers <<
    std::endl;

  --gIndenter;

  // harmonies
  // --------------------------------------

  gLog <<
    "Harmonies:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fShowHarmoniesVoices" << ": " <<
    fShowHarmoniesVoices <<
    std::endl;

  --gIndenter;

  // figured bass
  // --------------------------------------

  gLog <<
    "Figured bass:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fShowFiguredBassVoices" << ": " <<
    fShowFiguredBassVoices <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrOahGroup& elt)
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
// hidden global OAH OAH group ariable
EXP S_msrOahGroup pGlobalMsrOahGroup;

EXP S_msrOahGroup getGlobalMsrOahGroup ()
{
  return pGlobalMsrOahGroup;
}

//______________________________________________________________________________
S_msrOahGroup createGlobalMsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global MSR OAH group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalMsrOahGroup) {
    // create the global MSR OAH options group
    pGlobalMsrOahGroup =
      msrOahGroup::create ();
    assert (pGlobalMsrOahGroup != 0);
  }

  // return it
  return pGlobalMsrOahGroup;
}


}
