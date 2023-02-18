/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "mxsr2msrOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrReplaceClefAtom msrReplaceClefAtom::create (
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::map<msrClefKind, msrClefKind>&
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
  const std::string&     longName,
  const std::string&     shortName,
  const std::string&     description,
  const std::string&     valueSpecification,
  const std::string&     variableName,
  std::map<msrClefKind, msrClefKind>&
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
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
		std::stringstream ss;

    ss <<
      "==> oahAtom is of type 'msrReplaceClefAtom'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // theString contains the replace clef specification
  // decipher it to extract the old and new clef names

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
		std::stringstream ss;

    ss <<
      "--> theString = \"" << theString << "\", " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "(.*)" // originalClefName
    "="
    "(.*)" // destinationClefName
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
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
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
      "-marTransposePart argument '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    originalClefName    = sm [1],
    destinationClefName = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
		std::stringstream ss;

    ss <<
      "--> originalClefName = \"" << originalClefName << "\", " <<
      "--> destinationClefName = \"" << destinationClefName << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is originalClefName in the replace clef map?
  msrClefKind
    originalClefKind =
      msrClefKindFromString (
        K_MF_INPUT_LINE_UNKNOWN,
        originalClefName);

  std::map<msrClefKind, msrClefKind>::iterator
    it =
      fClefKindToClefKindMapVariable.find (originalClefKind);

  if (it != fClefKindToClefKindMapVariable.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "Replace clef value \"" << theString << "\" occurs more that once" <<
      "in  a " <<
      fetchNamesBetweenQuotes () <<
      " option";

    oahError (ss.str ());
  }

  else {
    msrClefKind
      destinationClefKind =
        msrClefKindFromString (
          K_MF_INPUT_LINE_UNKNOWN,
          destinationClefName);

    fClefKindToClefKindMapVariable [originalClefKind] =
      destinationClefKind;
  }

	fSelected = true;
}

void msrReplaceClefAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msrReplaceClefAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrReplaceClefAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msrReplaceClefAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrReplaceClefAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msrReplaceClefAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrReplaceClefAtom::asShortNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fShortName << ' ';

  if (! fClefKindToClefKindMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        msrClefKindAsString ((*i).first) <<
        "=" <<
        msrClefKindAsString ((*i).second);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

std::string msrReplaceClefAtom::asActualLongNamedOptionString () const
{
  std::stringstream ss;

  ss <<
    '-' << fLongName << ' ';

  if (! fClefKindToClefKindMapVariable.size ()) {
    ss << "[EMPTY]";
  }
  else {
    std::map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        msrClefKindAsString ((*i).first) <<
        "=" <<
        msrClefKindAsString ((*i).second);
      if (++i == iEnd) break;
      ss << ",";
    } // for
  }

  return ss.str ();
}

void msrReplaceClefAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrReplaceClefAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "variableName" << ": " <<
    fVariableName <<
    std::setw (fieldWidth) <<
    "fClefKindToClefKindMapVariable" << ": " <<
    std::endl;

  if (! fClefKindToClefKindMapVariable.size ()) {
    os << "[EMPTY]";
  }
  else {
    std::map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << msrClefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << msrClefKindAsString ((*i).second) << "\"";
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  os << std::endl;

  --gIndenter;
}

void msrReplaceClefAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": ";

  if (! fClefKindToClefKindMapVariable.size ()) {
    os <<
      "[EMPTY]" <<
      std::endl;
  }
  else {
    os << std::endl;
    ++gIndenter;

    std::map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << msrClefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << msrClefKindAsString ((*i).second) << "\"" <<
        std::endl;
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_msrReplaceClefAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________

S_mxsr2msrOahGroup gGlobalMxsr2msrOahGroup;

S_mxsr2msrOahGroup mxsr2msrOahGroup::create (
  const S_oahHandler& handler)
{
  mxsr2msrOahGroup* o = new mxsr2msrOahGroup (
    handler);
  assert (o != nullptr);
  return o;
}

mxsr2msrOahGroup::mxsr2msrOahGroup (
  const S_oahHandler& handler)
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
                            const S_oahHandler& handler)
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
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxsr2msrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

  // parts
  // --------------------------------------
  initializePartsOptions ();

  // staves
  // --------------------------------------
  initializeStavesOptions ();

  // voices
  // --------------------------------------
  initializeVoiceOptions ();

  // clefs, keys, time signatures
  // --------------------------------------
  initializeClefsKeysTimesOptions ();

  // page and line breaks
  // --------------------------------------
  initializePageAndLineBreaksOptions ();

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

#ifdef MF_TRACE_IS_ENABLED
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
#endif // MF_TRACE_IS_ENABLED

void mxsr2msrOahGroup::initializePartsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "help-mxsr-to-msr-parts", "hm2mparts",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML part ID
  // --------------------------------------

  fIgnoreMusicXMLPartIDAtom =
    oahStringSetElementAtom::create (
      "ignore-musicxml-part-id", "imxmlpi",
R"(Ignore the part with ID PART_ID, which is a std::string.
There can be several occurrences of this option.
All the parts not ignored are kept.
This option is incompatible with '-mkpi, -msr-keep-musicxml-part-id'.)",
      "PART_ID",
      "fPartsIgnoreIDSet",
      fPartsIgnoreIDSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreMusicXMLPartIDAtom);

  // ignore MusicXML part name
  // --------------------------------------

  fIgnoreMusicXMLPartNameAtom =
    oahStringSetElementAtom::create (
      "ignore-musicxml-part-name", "imxmlpn",
R"(Ignore the part named PART_NAME, which is a std::string.
There can be several occurrences of this option.
All the parts not ignored are kept.
This option is incompatible with '-mkpn, -msr-keep-musicxml-part-name'.)",
      "PART_NAME",
      "fMusicXMLPartsIgnoreNameSet",
      fMusicXMLPartsIgnoreNameSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreMusicXMLPartNameAtom);

  // keep MusicXML part ID
  // --------------------------------------

  fKeepMusicXMLPartIDAtom =
    oahStringSetElementAtom::create (
      "keep-musicxml-part-id", "kmxmlpi",
R"(Keep the part with ID PART_ID, which is a std::string.
There can be several occurrences of this option.
All the parts not kept are ignored.
This option is incompatible with '-mopi, -msr-ignore-musicxml-part-id'.)",
      "PART_ID",
      "fMusicXMLPartsKeepIDSet",
      fMusicXMLPartsKeepIDSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepMusicXMLPartIDAtom);

  // keep MusicXML part name
  // --------------------------------------

  fKeepMusicXMLPartNameAtom =
    oahStringSetElementAtom::create (
      "keep-musicxml-part-name", "kmxmlpn",
R"(Keep the part named PART_NAME, which is a std::string.
There can be several occurrences of this option.
All the parts not kept are ignored.
This option is incompatible with '-mopn, -msr-ignore-musicxml-part-name'.)",
      "PART_NAME",
      "fMusicXMLPartsKeepNameSet",
      fMusicXMLPartsKeepNameSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepMusicXMLPartNameAtom);
}

void mxsr2msrOahGroup::initializeStavesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Staves",
      "help-mxsr-to-msr-staves", "hm2mstaves",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML staff number
  // --------------------------------------

  fIgnoreStaffNumberAtom =
    oahIntSetElementAtom::create (
      "ignore-musicxml-staff-number", "imxmlsn",
R"(Ignore the staff numbered STAFF_NUMBER, which is an integer.
There can be several occurrences of this option.
All the staves not ignored are kept.
This option is incompatible with '-mkpn, -msr-keep-musicxml-staff-number'.)",
      "STAFF_NUMBER",
      "fMusicXMLStavesIgnoreNumberSet",
      fMusicXMLStavesIgnoreNumberSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreStaffNumberAtom);

  // keep MusicXML staff number
  // --------------------------------------

  fKeepStaffNumberAtom =
    oahIntSetElementAtom::create (
      "keep-musicxml-staff-number", "kmxmlsn",
R"(Keep the staff numbered STAFF_NUMBER, which is an integer.
There can be several occurrences of this option.
All the staves not kept are ignored.
This option is incompatible with '-mopn, -msr-ignore-musicxml-staff-number'.)",
      "STAFF_NUMBER",
      "fMusicXMLStavesKeepNumberSet",
      fMusicXMLStavesKeepNumberSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepStaffNumberAtom);
}

void mxsr2msrOahGroup::initializeVoiceOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Voices",
      "help-mxsr-to-msr-voices", "hm2mvoices",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // ignore MusicXML voice number
  // --------------------------------------

  fIgnoreVoiceNumberAtom =
    oahIntSetElementAtom::create (
      "ignore-musicxml-voice-number", "imxmlvn",
R"(Ignore the voice numbered VOICE_NUMBER, which is an integer.
There can be several occurrences of this option.
All the voices not ignored are kept.
This option is incompatible with '-mkpn, -msr-keep-musicxml-voice-number'.)",
      "VOICE_NUMBER",
      "fMusicXMLVoicesIgnoreNumberSet",
      fMusicXMLVoicesIgnoreNumberSet);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreVoiceNumberAtom);

  // keep MusicXML voice number
  // --------------------------------------

  fKeepVoiceNumberAtom =
    oahIntSetElementAtom::create (
      "keep-musicxml-voice-number", "kmxmlvn",
R"(Keep the voice numbered VOICE_NUMBER, which is an integer.
There can be several occurrences of this option.
All the voices not kept are ignored.
This option is incompatible with '-mopn, -msr-ignore-musicxml-voice-number'.)",
      "VOICE_NUMBER",
      "fMusicXMLVoicesKeepNumberSet",
      fMusicXMLVoicesKeepNumberSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepMusicXMLPartNameAtom);
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
            std::regex ("NUMBER"),
            std::to_string (gGlobalClefKindsMap.size ())),
          std::regex ("CLEFS_NAMES"),
//           gIndenter.indentMultiLineString (
//             foundString,
//             os);
          availableClefKindsNames (K_MF_NAMES_LIST_MAX_LENGTH)),
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

void mxsr2msrOahGroup::initializePageAndLineBreaksOptions ()
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

  // ignore MusicXML line breaks
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-musicxml-line-breaks", "imxmllb",
R"(Ignore line breaks in MusicXML data.)",
        "fIgnoreLineBreaksInMusicXML",
        fIgnoreLineBreaksInMusicXML));
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
where MEASURE_NUMBER is a std::string, and NUMBER_OF_MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and NUMBER_OF_MEASURES_TO_ADD should be at least 1, , such as '17:3'.
There can be spaces around the ':', in which case quoting is needed.
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
where MEASURE_NUMBER is a std::string, and NUMBER_OF_MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and NUMBER_OF_MEASURES_TO_ADD should be at least 1, , such as '17:3'.
There can be spaces around the ':', in which case quoting is needed.
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
        "ignore-musicxml-articulations", "imxmlarts",
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
        "ignore-musicxml-ornaments", "iimxmlorns",
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
        "ignore-musicxml-ties", "iimxmlties",
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
        "ignore-musicxml-dynamics", "iimxmldyns",
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
        "ignore-musicxml-slurs", "imxmlslurs",
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
        "ignore-musicxml-wedges", "imxmlwedges",
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
        "ignore-musicxml-lyrics", "imxmllyrics",
R"(Ignore lyrics in MusicXML data.)",
        "fIgnoreLyrics",
        fIgnoreLyrics));


  // ignore harmonies
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-musicxml-harmonies", "imxmlharms",
R"(Ignore harmonies in MusicXML data.)",
        "fIgnoreHarmonies",
        fIgnoreHarmonies));


  // ignore figured bass elements
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-musicxml-figured-bass", "imxmlfigbass",
R"(Ignore figured bass elements in MusicXML data.)",
        "fIgnoreFiguredBasses",
        fIgnoreFiguredBasses));

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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
		std::stringstream ss;

    ss <<
      "Checking the consistency of mxsr2msrOahGroup group \"" <<
      fGroupHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI and if mixed ID and name options are used?

  if (fPartsIgnoreIDSet.size () > 0 && fMusicXMLPartsKeepIDSet.size () > 0) {
    std::stringstream ss;

    ss <<
      "options '" <<
      fIgnoreMusicXMLPartIDAtom->fetchNames () <<
      "' and '" <<
      fKeepMusicXMLPartIDAtom->fetchNames () <<
      "' are incompatible";

    oahError (ss.str ());
  }

  if (fMusicXMLPartsIgnoreNameSet.size () > 0 && fMusicXMLPartsKeepNameSet.size () > 0) {
    std::stringstream ss;

    ss <<
      "options '" <<
      fIgnoreMusicXMLPartNameAtom->fetchNames () <<
      "' and '" <<
      fKeepMusicXMLPartNameAtom->fetchNames () <<
      "' are incompatible";

    oahError (ss.str ());
  }
}

//______________________________________________________________________________
Bool mxsr2msrOahGroup::wordsIsToBeConvertedToBold (
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fBoldWordsSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fItalicWordsSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToBePlacedAboveSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToBePlacedBelowSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToTemposSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToRehearsalMarkSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToSegnoSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoAlFineSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDalSegnoAlCodaSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCodaFirstSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCodaSecondSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToCrescSet.size ()) {
    std::set<std::string>::const_iterator
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
  const std::string& wordsValue)
{
  Bool result;

  // is wordsValue in the string to dal segno kind map?
  if (fWordsToDecrescSet.size ()) {
    std::set<std::string>::const_iterator
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mxsr2msrOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsr2msrOahGroup>*> (v)) {
        S_mxsr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mxsr2msrOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mxsr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mxsr2msrOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsr2msrOahGroup>*> (v)) {
        S_mxsr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mxsr2msrOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mxsr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mxsr2msrOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrOahGroup::printMxsr2msrValues (int valueFieldWidth)
{
  gLog <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

  // parts
  // --------------------------------------

  gLog <<
    "Parts:" <<
    std::endl;

  ++gIndenter;

  // parts ignored IDs

  gLog << std::left <<
    std::setw (valueFieldWidth) << "parts ignored IDs" << ": ";

  if (! fPartsIgnoreIDSet.size ()) {
    gLog <<
      "[NONE]";
  }
  else {
    for (
      std::set<std::string> ::const_iterator i =
        fPartsIgnoreIDSet.begin ();
      i != fPartsIgnoreIDSet.end ();
      ++i
  ) {
        gLog <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept IDs

  gLog << std::left <<
    std::setw (valueFieldWidth) << "parts kept IDs" << ": ";

  if (! fMusicXMLPartsKeepIDSet.size ()) {
    gLog <<
      "[NONE]";
  }
  else {
    for (
      std::set<std::string> ::const_iterator i =
        fMusicXMLPartsKeepIDSet.begin ();
      i != fMusicXMLPartsKeepIDSet.end ();
      ++i
  ) {
        gLog <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts ignored names

  gLog << std::left <<
    std::setw (valueFieldWidth) << "parts ignored names" << ": ";

  if (! fMusicXMLPartsIgnoreNameSet.size ()) {
    gLog <<
      "[NONE]";
  }
  else {
    for (
      std::set<std::string> ::const_iterator i =
        fMusicXMLPartsIgnoreNameSet.begin ();
      i != fMusicXMLPartsIgnoreNameSet.end ();
      ++i
  ) {
        gLog <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept names

  gLog << std::left <<
    std::setw (valueFieldWidth) << "parts kept names" << ": ";

  if (! fMusicXMLPartsKeepNameSet.size ()) {
    gLog <<
      "[NONE]";
  }
  else {
    for (
      std::set<std::string> ::const_iterator i =
        fMusicXMLPartsKeepNameSet.begin ();
      i != fMusicXMLPartsKeepNameSet.end ();
      ++i
  ) {
        gLog <<
          "\"" << (*i) << "\" ";
    } // for
  }

  gLog << std::endl;

  --gIndenter;

  // clefs, keys, time signatures
  // --------------------------------------

  gLog <<
    "clefs, keys, time signatures:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreRedundantClefs" << ": " <<
    fIgnoreRedundantClefs <<
    std::endl <<

    std::setw (valueFieldWidth) << "fIgnoreRedundantKeys" << ": " <<
    fIgnoreRedundantKeys <<
    std::endl <<

    std::setw (valueFieldWidth) << "fIgnoreRedundantTimes" << ": " <<
    fIgnoreRedundantTimes <<
    std::endl;

  --gIndenter;

  // page and line breaks
  // --------------------------------------

  gLog <<
    "Page breaks:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnorePageBreaksInMusicXML" << ": " <<
    fIgnorePageBreaksInMusicXML <<
    std::endl <<

  --gIndenter;

  // line breaks
  // --------------------------------------

  gLog <<
    "Line breaks:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreLineBreaksInMusicXML" << ": " <<
    fIgnoreLineBreaksInMusicXML <<
    std::endl <<

  --gIndenter;

  // measures
  // --------------------------------------

  gLog <<
    "Measures:" <<
    std::endl;

  ++gIndenter;

//     std::map<std::string,int>       fAddEmptyMeasuresStringToIntMap;

/* JMI
  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreRedundantClefs" << ": " <<
    fIgnoreRedundantClefs <<
    std::endl <<

    std::setw (valueFieldWidth) << "fIgnoreRedundantKeys" << ": " <<
    fIgnoreRedundantKeys <<
    std::endl <<

    std::setw (valueFieldWidth) << "fIgnoreRedundantTimes" << ": " <<
    fIgnoreRedundantTimes <<
    std::endl;
*/
  --gIndenter;

  // notes
  // --------------------------------------

  gLog <<
     "Notes:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fDelayRestsDynamics" << ": " <<
    fDelayRestsDynamics <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsWords" << ": " <<
    fDelayRestsWords <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsSlurs" << ": " <<
    fDelayRestsSlurs <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsLigatures" << ": " <<
    fDelayRestsLigatures <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsPedals" << ": " <<
    fDelayRestsPedals <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsSlashes" << ": " <<
    fDelayRestsSlashes <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDelayRestsWedges" << ": " <<
    fDelayRestsWedges <<
    std::endl <<

    std::setw (valueFieldWidth) << "fSlashAllGraceNotes" << ": " <<
    fSlashAllGraceNotes <<
    std::endl <<
    std::setw (valueFieldWidth) << "fSlurAllGraceNotes" << ": " <<
    fSlurAllGraceNotes <<
    std::endl <<
    std::setw (valueFieldWidth) << "fBeamAllGraceNotes" << ": " <<
    fBeamAllGraceNotes <<
    std::endl;

  --gIndenter;

  // articulations
  // --------------------------------------

  gLog <<
     "Articulations:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreArticulations" << ": " <<
    fIgnoreArticulations <<
    std::endl;

  --gIndenter;

  // ornaments
  // --------------------------------------

  gLog <<
     "Ornaments:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreOrnaments" << ": " <<
    fIgnoreOrnaments <<
    std::endl;

  --gIndenter;

  // words
  // --------------------------------------

  gLog <<
     "Words:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreMusicXMLWords" << ": " <<
    fIgnoreMusicXMLWords <<
    std::endl <<

    std::setw (valueFieldWidth) <<
    "fAddMsrWordsFromTheMusicXMLLyrics" << ": " <<
    fAddMsrWordsFromTheMusicXMLLyrics <<
    std::endl;

  --gIndenter;

  // ties
  // --------------------------------------

  gLog <<
     "Ties:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreTies" << ": " <<
    fIgnoreTies <<
    std::endl;

  --gIndenter;

  // dynamics
  // --------------------------------------

  gLog <<
     "Dynamics:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreDynamics" << ": " <<
    fIgnoreDynamics <<
    std::endl;

  --gIndenter;

  // slurs
  // --------------------------------------

  gLog <<
     "Slurs:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreSlurs" << ": " <<
    fIgnoreSlurs <<
    std::endl;

  --gIndenter;

  // wedges
  // --------------------------------------

  gLog <<
     "Wedges:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreWedges" << ": " <<
    fIgnoreWedges <<
    std::endl;

  --gIndenter;

  // lyrics
  // --------------------------------------

  gLog <<
    "Lyrics:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    std::setw (valueFieldWidth) << "fIgnoreLyrics" << ": " <<
    fIgnoreLyrics <<
    std::endl;

  --gIndenter;

  // harmonies
  // --------------------------------------

  gLog <<
    "Harmonies:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreHarmonies" << ": " <<
    fIgnoreHarmonies <<
    std::endl;

  --gIndenter;

  // figured bass
  // --------------------------------------

  gLog <<
    "Figured bass:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fIgnoreFiguredBasses" << ": " <<
    fIgnoreFiguredBasses <<
    std::endl;

  --gIndenter;

  // dynamics and wedges
  // --------------------------------------

  gLog <<
    "Dynamics and wedges:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fAllDynamicsBelow" << ": " <<
    fAllDynamicsBelow <<
    std::endl <<
    std::setw (valueFieldWidth) << "fAllWedgesBelow" << ": " <<
    fAllWedgesBelow <<
    std::endl;

  --gIndenter;

  // combined options, cubase
  // --------------------------------------

  gLog <<
    "Cubase:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fCubase" << ": " <<
    fCubase <<
    std::endl;

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------

  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  // specific trace
  // --------------------------------------

  --gIndenter;
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mxsr2msrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_mxsr2msrOahGroup createGlobalMxsr2msrOahGroup (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
		std::stringstream ss;

    ss <<
      "Creating global mxsr2msr OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
