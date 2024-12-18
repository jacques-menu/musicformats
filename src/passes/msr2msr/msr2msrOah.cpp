/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrIgnorePartAtom msrIgnorePartAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <std::string>&  stringSetVariable)
{
  msrIgnorePartAtom* obj = new
    msrIgnorePartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (obj != nullptr);
  return obj;
}

msrIgnorePartAtom::msrIgnorePartAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <std::string>&  stringSetVariable)
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msrIgnorePartAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the name of the part to be ignored

  std::string partName = theString;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partName = \"" << partName << "\", ";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::set <std::string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part \"" << partName << "\" occurs more that once" <<
      " in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }

  fSelected = true;
}

void msrIgnorePartAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrIgnorePartAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrIgnorePartAtom>*
    p =
      dynamic_cast<visitor<S_msrIgnorePartAtom>*> (v)) {
        S_msrIgnorePartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrIgnorePartAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrIgnorePartAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrIgnorePartAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrIgnorePartAtom>*
    p =
      dynamic_cast<visitor<S_msrIgnorePartAtom>*> (v)) {
        S_msrIgnorePartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrIgnorePartAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrIgnorePartAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrIgnorePartAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrIgnorePartAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string msrIgnorePartAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void msrIgnorePartAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrIgnorePartAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "variableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringSetVariable" << ": " <<
    std::endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;

  os << std::endl;
}

void msrIgnorePartAtom::displayAtomWithVariableOptionsValues (
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

    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", selected";

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_msrIgnorePartAtom& elt)
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
S_msrKeepPartAtom msrKeepPartAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <std::string>&  stringSetVariable)
{
  msrKeepPartAtom* obj = new
    msrKeepPartAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (obj != nullptr);
  return obj;
}

msrKeepPartAtom::msrKeepPartAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  std::set <std::string>&  stringSetVariable)
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msrKeepPartAtom'";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the name of the part to be kept

  std::string partName = theString;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "--> partName = \"" << partName << "\", ";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the part renaming map?
  std::set <std::string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Part \"" << partName << "\" occurs more that once" <<
      " in the " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }

  fSelected = true;
}

void msrKeepPartAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrKeepPartAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrKeepPartAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrKeepPartAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrKeepPartAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrKeepPartAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrKeepPartAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrKeepPartAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrKeepPartAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string msrKeepPartAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fStringSetVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void msrKeepPartAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrKeepPartAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "variableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fStringSetVariable" << ": " <<
    std::endl;

  if (! fStringSetVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;

  os << std::endl;
}

void msrKeepPartAtom::displayAtomWithVariableOptionsValues (
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

    std::set <std::string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", selected";

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_msrKeepPartAtom& elt)
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

S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

S_msr2msrOahGroup msr2msrOahGroup::create ()
{
  msr2msrOahGroup* obj = new msr2msrOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msr2msrOahGroup::msr2msrOahGroup ()
  : oahGroup (
      "msr2lpsr",
      "help-msr2msr-to-msr", "hmsr2msr",
R"(These options control the way MSR data is translated to another MSR.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2msrOahGroup ();
}

msr2msrOahGroup::~msr2msrOahGroup ()
{}

#ifdef MF_TRACE_IS_ENABLED
void msr2msrOahGroup::initializMsr2msrTraceOptions ()
{
  // JMI
}
#endif // MF_TRACE_IS_ENABLED

void msr2msrOahGroup::initializMsr2msrBookOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Book",
      "help-msr2msr-book", "hmsrb",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // avoid msr2msr
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "avoid-msr2msr", "am2m",
R"(Avoid the msr2msr pass, for TESTS.)",
        "fAvoidMsr2msr",
        fAvoidMsr2msr));

  // expand to harmony band
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "expand-to-harmony-band", "ethb",
R"(Expand the book to as many scores as needed for a harmony band.)",
        "fExpandToHarmonyBandBook",
        fExpandToHarmonyBandBook));
}

void msr2msrOahGroup::initializMsr2msrStavesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "help-msr2msr-staves", "hm2ms",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MSR staves
  // --------------------------------------

  fIgnoreMsrStavesSetAtom =
    oahStringSetElementAtom::create (
      "ignore-msr-staff", "ims",
R"(Ignore staff STAFF_NAME in the MSR data
when creating a new MSR from an existing one.
An example name is Part_POne_Staff_One.
There can be several occurrences of this option.
When this option is used, any staff not explicitly ignored is kept.)",
      "staff",
      "fIgnoreMsrStavesSet",
      fIgnoreMsrStavesSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreMsrStavesSetAtom);

  // keep MSR staves
  // --------------------------------------

  fKeepMsrStavesSetAtom =
    oahStringSetElementAtom::create (
      "keep-msr-staff", "kms",
R"(Keep staff STAFF_NAME in the MSR data
when creating a new MSR from an existing one.
An example name is Part_POne_Staff_One.
There can be several occurrences of this option.
When this option is used, any staff not explicitly kept is ignored.
Note that multi-staff staves may end up with empty staffs in them
with this option.)", // JMI
      "staff",
      "fKeepMsrStavesSet",
      fKeepMsrStavesSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepMsrStavesSetAtom);
}

void msr2msrOahGroup::initializMsr2msrVoicesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "help-msr2msr-voices", "hm2mv",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MSR voices
  // --------------------------------------

  fIgnoreMsrVoicesSetAtom =
    oahStringSetElementAtom::create (
      "ignore-msr-voice", "imv",
R"(Ignore voice VOICE_NAME in the MSR data
when creating a new MSR from an existing one.
An example name is Part_POne_Staff_One_Voice_Two.
There can be several occurrences of this option.
When this option is used, any voice not explicitly ignored is kept.)",
      "voice",
      "fIgnoreMsrVoicesSet",
      fIgnoreMsrVoicesSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreMsrVoicesSetAtom);

  // keep MSR voices
  // --------------------------------------

  fKeepMsrVoicesSetAtom =
    oahStringSetElementAtom::create (
      "keep-msr-voice", "kmv",
R"(Keep voice VOICE_NAME in the MSR data
when creating a new MSR from an existing one.
An example name is Part_POne_Staff_One_Voice_Two.
There can be several occurrences of this option.
When this option is used, any voice not explicitly kept is ignored.
Note that multi-voice staves may end up with empty voices in them
with this option.)", // JMI
      "voice",
      "fKeepMsrVoicesSet",
      fKeepMsrVoicesSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepMsrVoicesSetAtom);
}

void msr2msrOahGroup::initializMsr2msrBreakOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Breaks",
      "help-msr2msr-breaks", "hmb",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // page and line breaks
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

void msr2msrOahGroup::initializMsr2msrCompressOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Compress",
      "help-msr2msr-compress", "hmsrc",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // compress multi-measure rests
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
and does not handle multi-measure rests.)",
        "fMergeStaffCommonRests",
        fMergeStaffCommonRests));
}

void msr2msrOahGroup::initializMsr2msrRepeatsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "help-msr2msr-repeats", "hmsrr",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // bar lines
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "create-implicit-initial-repeat-barline-in-msr", "ciirbim",
R"(Create an implicit repeat barLine at the beginning of the staff
in the MSR in case there is none, as is usual in scores.
By default, no such barLine is added.)",
        "fCreateImplicitInitialRepeatBarLine",
        fCreateImplicitInitialRepeatBarLine));
}

void msr2msrOahGroup::initializMsr2msrNotesOptions ()
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

void msr2msrOahGroup::initializMsr2msrLyricsOptions ()
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
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2msrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

  // book
  // --------------------------------------
  initializMsr2msrBookOptions ();

  // staves
  initializMsr2msrStavesOptions ();

  // voices
  initializMsr2msrVoicesOptions ();

  // breaks
  initializMsr2msrBreakOptions ();

  // compress
  // --------------------------------------
  initializMsr2msrCompressOptions ();

  // repeats
  // --------------------------------------
  initializMsr2msrRepeatsOptions ();

  // notes
  // --------------------------------------
  initializMsr2msrNotesOptions ();

  // lyrics
  // --------------------------------------
  initializMsr2msrLyricsOptions ();
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2msrOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2msrOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2msrOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2msrOahGroup>*> (v)) {
        S_msr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2msrOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2msrOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msr2msrOahGroup::displayMsr2msrOahValues (int valueFieldWidth)
{
  gLog <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

  // avoid msr2msr? (for TESTS)
  // --------------------------------------

  gLog <<
    "avoid msr2msr? (for TESTS):" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fAvoidMsr2msr" << ": " <<
    fAvoidMsr2msr <<
    std::endl;

  --gIndenter;


  // book
  // --------------------------------------

  gLog <<
    "Book:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fExpandToHarmonyBandBook" << ": " <<
    fExpandToHarmonyBandBook <<
    std::endl;

  --gIndenter;


  // voices
  // --------------------------------------

  gLog <<
    "Voices:" <<
    std::endl;

  ++gIndenter;

  mfDisplayStringSet (
    "fIgnoreMsrVoicesSet",
    fIgnoreMsrVoicesSet,
    gLog);

  mfDisplayStringSet (
    "fKeepMsrVoicesSet",
    fKeepMsrVoicesSet,
    gLog);

  --gIndenter;


  // breaks
  // --------------------------------------

  gLog <<
    "Breaks:" <<
    std::endl;

  ++gIndenter;

  mfDisplayStringSet (
    "fInserPageBreakAfterMeasureSet",
    fInserPageBreakAfterMeasureSet,
    gLog);

  mfDisplayStringSet (
    "fInserLineBreakAfterMeasureSet",
    fInserLineBreakAfterMeasureSet,
    gLog);

  --gIndenter;


  // compress
  // --------------------------------------

  gLog <<
    "Compress" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fCoalesceEmptyMeasures" << ": " <<
    fCoalesceEmptyMeasures <<
    std::endl <<

    std::setw (valueFieldWidth) <<
    "fMergeStaffCommonRests" << ": " <<
    fMergeStaffCommonRests <<
    std::endl;

  --gIndenter;


  // repeats
  // --------------------------------------

  gLog <<
    "Repeats:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fCreateImplicitInitialRepeatBarLine" << ": " <<
    fCreateImplicitInitialRepeatBarLine <<
    std::endl;

  --gIndenter;


  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2msrOahGroup& elt)
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
S_msr2msrOahGroup createGlobalMsr2msrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2msr OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
