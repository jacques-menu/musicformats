/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "tree_browser.h"

#include "musicxmlWae.h"
#include "mxsr2msrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfTraceOah.h"

#include "msrBarLines.h"
#include "msrMeasureConstants.h"

#include "displayMsrSummaryVisitor.h"

#include "oahOah.h"

#include "mxsrOah.h"
#include "mxsr2msrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "oahEarlyOptions.h"

#include "mxsr2msrSkeletonBuilder.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// K_MF_INPUT_LINE_UNKNOWN_

/*
<!--
	The part-list identifies the different musical parts in
	this movement. Each part has an ID that is used later
	within the musical data. Since parts may be encoded
	separately and combined later, identification elements
	are present at both the score and score-part levels.
	There must be at least one score-part, combined as
	desired with part-group elements that indicate braces
	and brackets. Parts are ordered from top to bottom in
	a score based on the order in which they appear in the
	part-list.

	Each MusicXML part corresponds to a track in a Standard
	MIDI Format 1 file. The score-instrument elements are
	used when there are multiple instruments per track.
	The midi-device element is used to make a MIDI device
	or port assignment for the given track or specific MIDI
	instruments. Initial midi-instrument assignments may be
	made here as well.

	The part-name-display and part-abbreviation-display
	elements are defined in the common.mod file, as they can
	be used within both the score-part and print elements.
-->
<!ELEMENT part-list (part-group*, score-part,
	(part-group | score-part)*)>
<!ELEMENT score-part (identification?,
	part-name, part-name-display?,
	part-abbreviation?, part-abbreviation-display?,
	group*, score-instrument*,
	(midi-device?, midi-instrument?)*)>
<!ATTLIST score-part
    id ID #REQUIRED
>


	The MusicXML part groups can be interleaved,
	so everything is stored in data structures,
	whose analysis and the creation of MSR part groups and parts
	are postponed until the whole part list as been browsed
*/

//______________________________________________________________________________
S_mxsrPartGroup mxsrPartGroup::create (
	int                   startInputLineNumber,
	int                   partGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   identity)
{
  mxsrPartGroup* obj = new
    mxsrPartGroup (
      startInputLineNumber,
      partGroupNumber,
      theMsrPartGroup,
      identity);
  assert (obj != nullptr);
  return obj;
}

mxsrPartGroup::mxsrPartGroup (
	int                   startInputLineNumber,
	int                   partGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   identity)
{
  fPartGroupNumber = partGroupNumber;

  fPartGroupIdentity = identity;

  fStartInputLineNumber = startInputLineNumber;
  fStopInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;

  fMsrPartGroup = theMsrPartGroup;
}

mxsrPartGroup::~mxsrPartGroup ()
{}

bool mxsrPartGroup::comparePartGroupsByIncreasingIdentity (
	const S_mxsrPartGroup& first,
	const S_mxsrPartGroup& second)
{
  return
    first->fPartGroupIdentity
      <
    second->fPartGroupIdentity;
}

bool mxsrPartGroup::comparePartGroupsByDecreasingIdentity (
	const S_mxsrPartGroup& first,
	const S_mxsrPartGroup& second)
{
  return
    first->fPartGroupIdentity
      >=
    second->fPartGroupIdentity;
}

std::string mxsrPartGroup::asString () const
{
  std::stringstream ss;

  ss <<
    '\'' <<
    fPartGroupNumber <<
    "' -=> " <<
    fMsrPartGroup->fetchPartGroupCombinedName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    ss <<
      ", fPartGroupIdentity " << fPartGroupIdentity;
  }
#endif // MF_TRACE_IS_ENABLED

  ss <<
    ", lines " <<
    fStartInputLineNumber << ".." << fStopInputLineNumber;

  return ss.str ();
}

void mxsrPartGroup::print (std::ostream& os) const
{
  const int fieldWidth = 14;

	os <<
		"[mxsrPartGroup" <<
    ", fPartGroupNumber" << ": " <<
    fPartGroupNumber <<
		std::endl;

	++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStartInputLineNumber" << ": " <<
    fStartInputLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStopInputLineNumber" << ": " <<
    fStopInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupIdentity" << ": " <<
    fPartGroupIdentity <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMsrPartGroup" << ": " <<
    std::endl;

	++gIndenter;
  os <<
    fMsrPartGroup <<
    std::endl;
  --gIndenter;

	os << ']' << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mxsrPartGroup& elt)
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
S_mxsrPartGroupsList mxsrPartGroupsList::create ()
{
  mxsrPartGroupsList* obj = new
    mxsrPartGroupsList ();
  assert (obj != nullptr);
  return obj;
}

S_mxsrPartGroupsList mxsrPartGroupsList::create (
	const S_mxsrPartGroup& partGroup)
{
  mxsrPartGroupsList* obj = new
    mxsrPartGroupsList (
    	partGroup);
  assert (obj != nullptr);
  return obj;
}

mxsrPartGroupsList::mxsrPartGroupsList ()
{}

mxsrPartGroupsList::mxsrPartGroupsList (
	const S_mxsrPartGroup& partGroup)
{
  fMxsrPartGroupsStdList.push_back (partGroup);
}

mxsrPartGroupsList::~mxsrPartGroupsList ()
{}

void mxsrPartGroupsList::prependPartGroup (
	const S_mxsrPartGroup& partGroup)
{
  fMxsrPartGroupsStdList.push_front (partGroup);
}

void mxsrPartGroupsList::appendPartGroup (
	const S_mxsrPartGroup& partGroup)
{
  fMxsrPartGroupsStdList.push_back (partGroup);
}

void mxsrPartGroupsList::sortByDecreasingIdentity()
{
  fMxsrPartGroupsStdList.sort (
  	mxsrPartGroup::comparePartGroupsByDecreasingIdentity);
}

void mxsrPartGroupsList::print (std::ostream& os) const
{
  os <<
    "[mxsrPartGroupsList" <<
    ", fMxsrPartGroupListName: " << fMxsrPartGroupListName << ':';

  if (fMxsrPartGroupsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		for (S_mxsrPartGroup partGroup : fMxsrPartGroupsStdList) {
			os <<
				partGroup->asString () <<
				std::endl;
		} // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

	os << ']' << std::endl;
}

void mxsrPartGroupsList::printWithContext (
  const std::string& context,
	char               evidencer,
  std::ostream&      os) const
{
	size_t
		thePartGroupsStdListSize =
			fMxsrPartGroupsStdList.size ();

  os <<
    "[mxsrPartGroupsList" <<
    ", fMxsrPartGroupListName: \"" << fMxsrPartGroupListName << "\"" <<
    ", " <<
    mfSingularOrPlural (
    	thePartGroupsStdListSize, "element", "elements") <<
    " -- " <<
    context <<
    ':';

  if (fMxsrPartGroupsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		int counter = thePartGroupsStdListSize - 1;
    for (S_mxsrPartGroup partGroup : fMxsrPartGroupsStdList) {
      os <<
        counter << ": " << evidencer <<
        std::endl;

			++gIndenter;
      os <<
        partGroup <<
        std::endl;
			--gIndenter;

      --counter;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsrPartGroupsList& elt) {
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsList& elt) {
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxsrPartGroupsStack mxsrPartGroupsStack::create ()
{
  mxsrPartGroupsStack* obj = new
    mxsrPartGroupsStack ();
  assert (obj != nullptr);
  return obj;
}

S_mxsrPartGroupsStack mxsrPartGroupsStack::create (
	const S_mxsrPartGroup& partGroup)
{
  mxsrPartGroupsStack* obj = new
    mxsrPartGroupsStack (
    	partGroup);
  assert (obj != nullptr);
  return obj;
}

mxsrPartGroupsStack::mxsrPartGroupsStack ()
{}

mxsrPartGroupsStack::mxsrPartGroupsStack (
	const S_mxsrPartGroup& partGroup)
{
  fMxsrPartGroupsStdList.push_back (partGroup);
}

mxsrPartGroupsStack::~mxsrPartGroupsStack ()
{}

void mxsrPartGroupsStack::print (std::ostream& os) const
{
  os <<
    "[mxsrPartGroupsStack" <<
    ", fMxsrPartGroupStackName: " << fMxsrPartGroupStackName << ':';

  if (fMxsrPartGroupsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		for (S_mxsrPartGroup partGroup : fMxsrPartGroupsStdList) {
			os <<
				partGroup->asString () <<
				std::endl;
		} // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

	os << ']' << std::endl;
}

void mxsrPartGroupsStack::printWithContext (
  const std::string& context,
	char               evidencer,
  std::ostream&      os) const
{
	size_t
		thePartGroupsStdListSize =
			fMxsrPartGroupsStdList.size ();

  os <<
    "The part groups stack contains " <<
    "[mxsrPartGroupsStack" <<
    ", fMxsrPartGroupStackName: \"" << fMxsrPartGroupStackName << "\"" <<
    ", " <<
    mfSingularOrPlural (
    	thePartGroupsStdListSize, "element", "elements") <<
    " -- " <<
    context <<
    ':';

  if (fMxsrPartGroupsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		int counter = thePartGroupsStdListSize - 1;
    for (S_mxsrPartGroup partGroup : fMxsrPartGroupsStdList) {
      os <<
        counter << ": " << evidencer <<
        std::endl;

			++gIndenter;
      os <<
        partGroup <<
        std::endl;
			--gIndenter;

      --counter;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsrPartGroupsStack& elt) {
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsStack& elt) {
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxsr2msrPendingTupletStop mxsr2msrPendingTupletStop::create (
	int eventSequentialNumber,
	int noteSequentialNumber,
	int staffNumber,
	int voiceNumber,
	int tupletNumber,
	int eventInputStartLineNumber,
	int eventInputEndLineNumber)
{
  mxsr2msrPendingTupletStop* obj = new
    mxsr2msrPendingTupletStop (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      tupletNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsr2msrPendingTupletStop::mxsr2msrPendingTupletStop (
	int eventSequentialNumber,
	int noteSequentialNumber,
	int staffNumber,
	int voiceNumber,
	int tupletNumber,
	int eventInputStartLineNumber,
	int eventInputEndLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;
  fNoteSequentialNumber = noteSequentialNumber;

	fStaffNumber = staffNumber;
  fVoiceNumber = voiceNumber;

  fTupletNumber = tupletNumber;

  fEventInputStartLineNumber = eventInputStartLineNumber;
  fEventInputEndLineNumber   = eventInputEndLineNumber;
}

mxsr2msrPendingTupletStop::~mxsr2msrPendingTupletStop ()
{}

std::string mxsr2msrPendingTupletStop::asString () const
{
  std::stringstream ss;

  ss <<
		"[PendingTupletStop" <<
		", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
// 		", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
		", fEventSequentialNumber: E" << fEventSequentialNumber <<
		", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
		", fStaffNumber: S" << fStaffNumber <<
		", fVoiceNumber:V" << fVoiceNumber <<
		", fTupletNumber: T" << fTupletNumber <<
    ']' <<
    std::endl;

  return ss.str ();
}

void mxsr2msrPendingTupletStop::print (std::ostream& os) const
{
	os <<
		"[PendingTupletStop" <<
		std::endl;

	++gIndenter;

  const int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputStartLineNumber" << ": " <<
    fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << ": " <<
//     fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" <<
    fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" <<
    fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffNumber" << ": S" <<
    fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceNumber" << ": V" <<
    fVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletNumber" << ": T" <<
    fTupletNumber <<
    std::endl;

  --gIndenter;

	os <<
		']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsr2msrPendingTupletStop& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsr2msrPendingTupletStop& elt)
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
mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder (
	mxsrEventsCollection&
		theResultingEventsCollection)
	: fResultingEventsCollection (
			theResultingEventsCollection)
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      "msrScore::create()");

  // parts handling
  fCurrentPartGroupIdentity= -1; // the score part 'start' will set this identity

  // staff handling
  fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // voice handling
  fCurrentNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
  fPreviousNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;

  // measures handling
  fScoreFirstMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;
  fScoreLastMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;

  fScoreMeasuresNumber = 0;
  fPartNumberOfMeasures = 0;

	// notes handling
	fCurrentNoteSequentialNumber = 0;
	fPreviousNoteSequentialNumber = 0;

	fCurrentNoteStartInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;
	fCurrentNoteEndInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;

	fPreviousNoteStartInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;
	fPreviousNoteEndInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;

	// staff changes handling
	fCurrentStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
	fPreviousNoteIsATakeOffCandidate = false;

  // chords handling
  fCurrentNoteBelongsToAChord = false;
  fPreviousNoteBelongsToAChord = false;

  // tuplets handling
//   fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeNone;

  fCurrentTupletNumber  = -1;
  // fPreviousTupletNumber = -1;

  fCurrentNoteBelongsToATuplet = false;

  // lyrics handling
  fCurrentStanzaNumber = K_STANZA_NUMBER_UNKNOWN_; // JMI
  fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_; // JMI

  // harmonies handling
  fHarmoniesVoicesCounter = 0;

  // figured bass handling
  fFiguredBassVoicesCounter = 0;

  // create the implicit outer-most part group,
  // needed for stand-alone parts
	createTheImplicitOuterPartGroupAndAddItToTheMsrScore (
		K_MF_INPUT_LINE_UNKNOWN_);
}

mxsr2msrSkeletonBuilder::~mxsr2msrSkeletonBuilder ()
{}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::browseMxsr (
  const Sxmlelement& theMxsr)
{
  if (theMxsr) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);

    // browse the MXSR
    browser.browse (*theMxsr);

		// sort the fResultingEventsCollection MXSR events list
		fResultingEventsCollection.sortTheMxsrEventsLists ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
    gLog <<
      std::endl <<
      "The MSR skeleton builder collected the following events:" <<
      std::endl;

			++gIndenter;
			fResultingEventsCollection.print (gLog);
			--gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
  }
}

//______________________________________________________________________________
S_mxsrPartGroup mxsr2msrSkeletonBuilder::fetchStartedPartGroupFromMap (
  int partGroupNumber)
{
  S_mxsrPartGroup result;

  if (fStartedPartGroupsMap.count (partGroupNumber)) {
    result =
      fStartedPartGroupsMap [partGroupNumber];
  }

  return result;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPartGroupsMap (
  int inputLineNumber)
{
  gLog <<
    "fPartGroupsMap, " <<
    mfSingularOrPlural (
    	fPartGroupsMap.size (), "element", "elements") <<
    ':';

  if (fPartGroupsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (std::pair <int, S_mxsrPartGroup> thePair : fPartGroupsMap) {
      S_mxsrPartGroup
        partGroup = thePair.second;

      S_msrPartGroup
        theMsrPartGroup =
          partGroup->getMsrPartGroup ();

      gLog <<
        partGroup->asString () <<
        std::endl;

        ++gIndenter;

        theMsrPartGroup->
          displayPartGroupElementsList (
            inputLineNumber);

        --gIndenter;
    } // for

    --gIndenter;
  }

  else {
    gLog <<
      " [EMPTY]" <<
      std::endl;
  }

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPartsMap (
  int inputLineNumber)
{
  gLog <<
    "fPartsMap," <<
    mfSingularOrPlural (
    	fPartsMap.size (), "element", "elements") <<
    ':';

  if (fPartsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (std::pair <std::string, S_msrPart> thePair : fPartsMap) {
    	std::string
    		theString = thePair.first;

      S_msrPart
        part = thePair.second;

      gLog <<
      	theString <<
      	" ---> " <<
        part->asString () <<
        std::endl;

    } // for

    --gIndenter;
  }

  else {
    gLog <<
    	" [EMPTY]" <<
      std::endl;
  }

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayStartedPartGroupsMap (
  int inputLineNumber)
{
  gLog <<
    "fStartedPartGroupsMap, " <<
    mfSingularOrPlural (
    	fStartedPartGroupsMap.size (), "element", "elements") <<
    ':';

  if (fStartedPartGroupsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (std::pair <int, S_mxsrPartGroup> thePair : fStartedPartGroupsMap) {
      S_mxsrPartGroup
        partGroup = thePair.second;

      S_msrPartGroup
        theMsrPartGroup =
          partGroup->getMsrPartGroup ();

      gLog <<
        partGroup->asString () <<
        std::endl;

        ++gIndenter;

        theMsrPartGroup->
          displayPartGroupElementsList (
            inputLineNumber);

        --gIndenter;
    } // for

    --gIndenter;
  }

  else {
    gLog <<
      " [EMPTY]" <<
      std::endl;
  }

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPartGroupsStack (
  int inputLineNumber)
{
  gLog <<
    "fPartGroupsStack, " <<
    mfSingularOrPlural (
    	fPartGroupsStack.size (), "element", "elements") <<
    ':' <<
    std::endl;

  ++gIndenter;

	fPartGroupsStack.printWithContext (
		"displayPartGroupsStack()",
		'v',
		gLog);

  --gIndenter;

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fPartGroupsVector, " <<
    mfSingularOrPlural (
    	fPartGroupsVector.size (), "element", "elements") <<
    ':' <<
    std::endl;

  ++gIndenter;

  for (size_t i = 0; i < fPartGroupsVector.size (); ++i) {
    S_mxsrPartGroup
      partGroup = fPartGroupsVector [i];

    S_msrPartGroup
      theMsrPartGroup =
        partGroup->getMsrPartGroup ();

    gLog <<
      i << ": " <<
			theMsrPartGroup->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  } // for

  --gIndenter;

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayStartedPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fStartedPartGroupsListsVector, " <<
    mfSingularOrPlural (
    	fStartedPartGroupsListsVector.size (), "element", "elements") <<
    ':' <<
    std::endl;

	++gIndenter;

	for (
		size_t identity= 0;
		identity< fStartedPartGroupsListsVector.size ();
		++identity
	) {
		S_mxsrPartGroupsList
			startedMxsrPartGroupsList =
				fStartedPartGroupsListsVector [identity];

		gLog <<
			identity<< ':' <<
			std::endl;

		++gIndenter;
		gLog <<
			startedMxsrPartGroupsList;
		--gIndenter;
	} // for

	--gIndenter;

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayStoppedPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fStoppedPartGroupsListsVector, " <<
    mfSingularOrPlural (
    	fStoppedPartGroupsListsVector.size (), "element", "elements") <<
    ':' <<

	++gIndenter;

	for (
		size_t identity= 0;
		identity< fStoppedPartGroupsListsVector.size ();
		++identity
	) {
		S_mxsrPartGroupsList
			StoppedPartGroupssList =
				fStoppedPartGroupsListsVector [identity];

		gLog <<
			identity<< ':' <<
			std::endl;

		++gIndenter;
		gLog <<
			StoppedPartGroupssList;
		--gIndenter;
	} // for

	--gIndenter;

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPartsVector (
  int inputLineNumber)
{
  gLog <<
    "fPartsVector, " <<
    mfSingularOrPlural (
    	fPartsVector.size (), "element", "elements") <<
    ':';

  if (fPartsVector.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fPartsVector.size (); ++i) {
      S_msrPart
        part = fPartsVector [i];

      gLog <<
        i << ": " <<
        part->fetchPartCombinedName () <<
        ", upLink to: ";

      S_msrPartGroup
        partUpLinkToPartGroup =
          part->
            getPartUpLinkToPartGroup ();

      if (partUpLinkToPartGroup) {
        gLog <<
          partUpLinkToPartGroup->
            fetchPartGroupCombinedName ();
      }
      else {
        gLog <<
          "not yet established";
      }

      gLog <<
        ", line " << inputLineNumber <<
        std::endl;
    } // for

    --gIndenter;
  }

  else {
		gLog <<
			" [EMPTY]" <<
			std::endl;
	}

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayAllCollectedData (
  int                inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "=======> displayAllCollectedData(), " <<
    context <<
    ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity<<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl <<
    ">>> ================================================" <<
    std::endl;

	// part groups stack
	displayPartGroupsStack (
		inputLineNumber);
  gLog << std::endl;

	// part groups map
  displayPartGroupsMap (
    inputLineNumber);
  gLog << std::endl;

	// parts map
  displayPartsMap (
    inputLineNumber);
  gLog << std::endl;

//   displayPartsVector (
//     inputLineNumber);
//   gLog << std::endl;

//   displayStartedPartGroupsMap (
//     inputLineNumber);
//   gLog << std::endl;
//
//   displayPartGroupsVector (
//     inputLineNumber);
//   gLog << std::endl;

	// part groups identitys
//   displayStartedPartGroupsVector (
//     inputLineNumber);
//   gLog << std::endl;
//
//   displayStoppedPartGroupsVector (
//     inputLineNumber);
//   gLog << std::endl;

  gLog <<
    "<<< ================================================" <<
    std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPart (
  int              inputLineNumber,
  const S_msrPart& theMsrPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Registering MSR part " <<
      theMsrPart->fetchPartCombinedName () <<
      " in the parts data" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register theMsrPart in the parts vector
  fPartsVector.push_back (theMsrPart);

  // register it in the parts map
  fPartsMap [theMsrPart->getPartID ()] = theMsrPart;

	// append it to current part group
	S_msrPartGroup
		currentMsrPartGroup =
			fPartGroupsStack.top ()->
				getMsrPartGroup ();

	currentMsrPartGroup->
		appendPartToPartGroup (
			theMsrPart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "AFTER registering MSR part \"" <<
      theMsrPart->fetchPartCombinedName () <<
      "\" in the parts data" <<
      ", line " << inputLineNumber;

    displayAllCollectedData (
      inputLineNumber,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPartGroupStart (
  int                    inputLineNumber,
  const S_mxsrPartGroup& partGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering part group start for " <<
      partGroup->fetchMsrPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE registerPartGroupStart()");
  }
#endif // MF_TRACE_IS_ENABLED

	// pushing partGroup onto the part groups stack,
	// thus making it the new current part group
#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTracePartGroups ()) {
		std::stringstream ss;

		ss <<
			"Pushing part group " <<
			partGroup->asString () <<
			" onto the part groups stack" <<
			", line " << inputLineNumber;

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

	fPartGroupsStack.push (
		partGroup);

  // register partGroup in the part groups vector, i.e. by APPEARANCE order
  fPartGroupsVector.push_back (
    partGroup);

  // register it in fPartGroupsMap under its order as part group,
  // since its part group number may reused later by other part groups
  fPartGroupsMap [fCurrentPartGroupIdentity] =
    partGroup;

  // register in fStartedPartGroupsMap // JMI v0.9.69 BUG ???
  // under its part group number,
  int theMsrPartGroupNumber =
    partGroup->
      getMsrPartGroup ()->
        getPartGroupNumber ();

  fStartedPartGroupsMap [theMsrPartGroupNumber] =
    partGroup;

  // create an empty mxsrPartGroupsList for part groups started at scorePartIdentity // JMI v0.9.69
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
  	std::stringstream ss;

  	ss <<
      "Appending an empty mxsrPartGroupsList to fStartedPartGroupsListsVector" <<
      ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity;

    displayAllCollectedData (
      inputLineNumber,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStartedPartGroupsListsVector.push_back (
    mxsrPartGroupsList::create ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER registerPartGroupStart()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPartGroupStop (
  int                    inputLineNumber,
  const S_mxsrPartGroup& partGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering part group stop for " <<
      partGroup->fetchMsrPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	if (fPartGroupsStack.size () == 0) {
		std::stringstream ss;

		ss <<
			"Cannot stop part group " <<
			partGroup->asString () <<
			", since the part groups groups stack is empty"<<
			", line " << inputLineNumber;

		musicxmlError (
			gServiceRunData->getInputSourceName (),
			inputLineNumber,
			__FILE__, __LINE__,
			ss.str ());
	}

	// fetch the top of the stack, i.e. the current part group
	S_mxsrPartGroup
		partGroupStackTop =
			fPartGroupsStack.top ();

	if (partGroup == partGroupStackTop) {
		// stop the current, top-most part group

		// pop partGroupStackTop from the stack
#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTracePartGroups ()) {
			std::stringstream ss;

			ss <<
				"Popping part group " <<
				partGroupStackTop->
					fetchMsrPartGroupCombinedName () <<
				" from the stack" <<
				", line " << inputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
}
#endif // MF_TRACE_IS_ENABLED

		fPartGroupsStack.pop ();

		// fetch the new top of the stack, i.e. the new current part group
		S_mxsrPartGroup
			newPartGroupStackTop =
				fPartGroupsStack.top ();

		// partGroupStackTop is nested in newPartGroupStackTop,
		handlePartGroupsNesting (
			inputLineNumber,
			partGroup,
			newPartGroupStackTop);
	}

	else {
		// the part group stops are out of oder JMI v0.9.69

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTracePartGroups ()) {
			std::stringstream ss;

			ss <<
				"The part group stop for " <<
				partGroup->asString () <<
				" is out of order, it does not match the part group stack top " <<
				partGroupStackTop->
					fetchMsrPartGroupCombinedName () <<
				", line " << inputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
}
#endif // MF_TRACE_IS_ENABLED
	}

// 	if (fPartGroupsStack.size () == 0) { // JMI v0.9.71
//     std::stringstream ss;
//
//     ss <<
//       "staff number " << fCurrentNoteStaffNumber <<
//       " is not positive" <<
//       ", line " << inputLineNumber;
//
//       mfAssert (
//         __FILE__, __LINE__,
// 				false,
// 				ss.str ());
//   }

//   // fetch the list of part groups stopped at
//   // the same identityas partGroup
//   S_mxsrPartGroupsList
//     stoppedPartGroupsList =
//       fStoppedPartGroupsListsVector [
//         fCurrentPartGroupIdentity];
//
// 	// append partGroup to the stoppedPartGroupsList
// 	stoppedPartGroupsList->appendPartGroup (
// 		partGroup);
//
//   // forget it in fStartedPartGroupsMap,
//   // it remains in fPartGroupsMap
//   fStartedPartGroupsMap.erase (
//     partGroup->
//       getMsrPartGroup ()->
//         getPartGroupNumber ());
//
//   // create an empty mxsrPartGroupsList list for part groups stopped at scorePartIdentity
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePartGroups ()) {
//   	std::stringstream ss;
//
//   	ss <<
//       "Appending an empty mxsrPartGroupsList to fStoppedPartGroupsListsVector" <<
//       ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity;
//
//     displayAllCollectedData (
//       inputLineNumber,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fStoppedPartGroupsListsVector.push_back (
//     mxsrPartGroupsList::create ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handlePartGroupStop()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupStart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Hangling part group start with number '" <<
      fCurrentPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE handlePartGroupStart()");
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentPartGroupIdentity;

	// allocated a new part group identify
  ++fCurrentPartGroupIdentity;

  // create the part group,
  // with the current part group as part group upLink
  S_msrPartGroup
    startedPartGroup =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        fCurrentPartGroupIdentity,
        fCurrentPartGroupName,
        fCurrentPartGroupNameDisplayText,
        fCurrentPartGroupAccidentalText,
        fCurrentPartGroupAbbreviation,
        fCurrentPartGroupSymbolKind,
        fCurrentPartGroupSymbolDefaultX,
        msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo,
        fCurrentPartGroupBarLineKind,
        nullptr, // partGroupUpLinkToPartGroup will be set upon 'stop'
        fMsrScore);

  // startedPartGroup will be appended to the MSR score
  // upon 'stop', once it is complete

  // create the part group
  S_mxsrPartGroup
    partGroup =
      mxsrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        startedPartGroup,
        fCurrentPartGroupIdentity) ;

  // register it in the part groups data
  registerPartGroupStart (
    inputLineNumber,
    partGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handlePartGroupStart()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupStop (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Hangling part group stop with number '" <<
      fCurrentPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED


#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE handlePartGroupStop()");
  }
#endif // MF_TRACE_IS_ENABLED

  // is the part group to be stopped started?
  S_mxsrPartGroup
    partGroupToBeStopped =
      fetchStartedPartGroupFromMap (
        fCurrentPartGroupNumber);

  if (! partGroupToBeStopped) {
    // no, but we should have found it
    std::stringstream ss;

    ss <<
      "no part group '" << fCurrentPartGroupNumber <<
      "' has been started, it cannot be stopped" <<
      ", line " << inputLineNumber;

 // JMI   musicxmlError (
    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      ss.str ());
  }

  else {
    // register partGroupToBeStopped as stopped
    registerPartGroupStop (
      inputLineNumber,
      partGroupToBeStopped);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handlePartGroupStop()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupsNesting (
  int                    inputLineNumber,
  const S_mxsrPartGroup& partGroupToBeStopped,
  const S_mxsrPartGroup& containingPartGroup)
{
  // get the MSR part groups involved
  S_msrPartGroup
    theMsrPartGroupToBeStopped =
      partGroupToBeStopped->
        getMsrPartGroup (),
    theMsrContainingPartGroup =
      containingPartGroup->
        getMsrPartGroup ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Handling the nesting of part group " <<
      theMsrContainingPartGroup->
        fetchPartGroupCombinedName () <<
      "' into " <<
      theMsrContainingPartGroup->
        fetchPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set currentPartGroup's upLink to containingPartGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting the upLink of part group " <<
      theMsrPartGroupToBeStopped->
        fetchPartGroupCombinedName () <<
      "' to " <<
      theMsrContainingPartGroup->
        fetchPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  theMsrPartGroupToBeStopped->
    setPartGroupUpLinkToContainingPartGroup (
    	inputLineNumber,
      theMsrContainingPartGroup);

  // append currentPartGroup to containingPartGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending sub part group " <<
      theMsrPartGroupToBeStopped->
        fetchPartGroupCombinedName () <<
      "' to " <<
      theMsrContainingPartGroup->
        fetchPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  theMsrContainingPartGroup->
    appendNestedPartGroupToPartGroup (
      theMsrPartGroupToBeStopped);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::createTheImplicitOuterPartGroupAndAddItToTheMsrScore (
	int inputLineNumber)
{
  // an implicit outer-most part group has to be created to contain everything,
  // since there can be parts out of any explicit part group in MusicXML

	const int K_MXSR_IMPLICIT_PART_GROUP_NUMBER = -1;

  // create an implicit outer-most part group
  fCurrentPartGroupNumber = K_MXSR_IMPLICIT_PART_GROUP_NUMBER;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating an implicit outer-most part group with number '" <<
      fCurrentPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE createTheImplicitOuterPartGroupAndAddItToTheMsrScore()");
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentPartGroupIdentity;

	// create fImplicitOuterMostMsrPartGroup
  fImplicitOuterMostMsrPartGroup =
		msrPartGroup::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fCurrentPartGroupIdentity,
      "***IMPLICIT OUTER-MOST PART GROUP***", 			// partGroupName
      "",                   								 	 			// PartGroupNameDisplayText
      "",                   								   			// partGroupAccidentalText
      "Impl. PartGroup",           			       			// partGroupAbbreviation
      msrPartGroupSymbolKind::kPartGroupSymbolNone, // partGroupSymbolKind
      0,                                            // partGroupSymbolDefaultX,
      msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes,
      msrPartGroupBarLineKind::kPartGroupBarLineYes,
      nullptr,                                      // partGroupUpLinkToPartGroup
      fMsrScore);

  // append it to the MSR score
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending implicit outer-most part group '" <<
      fImplicitOuterMostMsrPartGroup->getPartGroupNumber () <<
      "' to MSR score" <<
      ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity<<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// add fImplicitOuterMostMsrPartGroup to the score
  fMsrScore->
    addPartGroupToScore (
      fImplicitOuterMostMsrPartGroup);

  // create the MXSR part group for the implicit outer-most part group
  fImplicitOuterMostPartGroup =
    mxsrPartGroup::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fImplicitOuterMostMsrPartGroup,
      fCurrentPartGroupIdentity);

  // register it in the part groups data
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Adding implicit MXSR outer-most part group for '" <<
      fImplicitOuterMostPartGroup->asString () <<
      "' to the part groups data" <<
      ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity<<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // push fImplicitOuterMostPartGroup onto the part groups stack
// 	fPartGroupsStack.appendPartGroup (
// 		fImplicitOuterMostPartGroup);

  // register fImplicitOuterMostPartGroup as started
  registerPartGroupStart (
    inputLineNumber,
    fImplicitOuterMostPartGroup);

//   // create an empty list for part groups started at scorePartIdentity
//   fStartedPartGroupsListsVector.push_back (
//     mxsrPartGroupsList::create (
//     	fImplicitOuterMostPartGroup));
//
//   // create an empty list for part groups stopped at scorePartIdentity
//   fStoppedPartGroupsListsVector.push_back (
//     mxsrPartGroupsList::create (
//     	fImplicitOuterMostPartGroup));

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER createTheImplicitOuterPartGroupAndAddItToTheMsrScore()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "handleBOFPartGroupsNestingBOFAndScorePartsAllocation:";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // this is actually a partial sub-pass of pass 2a,
  // since we run a second time through the contents of <part-list />,
  // stored in the data structures we've built

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE handleBOFPartGroupsNestingBOFAndScorePartsAllocation");
  }
#endif // MF_TRACE_IS_ENABLED

	// sort the contents offStartedPartGroupsListsVector
	// in decreasing identityorder
	for (
		S_mxsrPartGroupsList thePartGroupList :
			fStartedPartGroupsListsVector
	) {

		// sort thePartGroupList in decreasing identityorder
#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTracePartGroups ()) {
			std::stringstream ss;

			ss <<
				"Sorting the started part group list" <<
				", line " << inputLineNumber ;

			thePartGroupList->printWithContext (
				"handleBOFPartGroupsNestingBOFAndScorePartsAllocation()",
				'>',
				ss);

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		thePartGroupList->sortByDecreasingIdentity();
	} // for

	// sort the contents of fStoppedPartGroupsListsVector
	// in decreasing identity order
	for (S_mxsrPartGroupsList thePartGroupList : fStoppedPartGroupsListsVector) {
		// sort thePartGroupList in decreasing identit yorder
#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTracePartGroups ()) {
			std::stringstream ss;

			ss <<
				"Sorting the stopped part group list " <<
				", line " << inputLineNumber <<
				std::endl;

			++gIndenter;
			thePartGroupList->printWithContext (
				"handleBOFPartGroupsNestingBOFAndScorePartsAllocation()",
				'>',
				ss);
			--gIndenter;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		thePartGroupList->sortByDecreasingIdentity();
	} // for

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTracePartGroups ()) {
		displayAllCollectedData (
			inputLineNumber,
			"AFTER sorting started and stopped MSXR part groups lists");
	}
#endif // MF_TRACE_IS_ENABLED

  // handle each identityin turn in increasing order
  for (int identity= 0; identity<= fCurrentPartGroupIdentity; ++identity) {

//     if (identity> 0) {
//       // parts actual identitys start at 1
      // append part to current part group, i.e. to the top of the stack
      S_msrPart
        part =
          fPartsVector [identity];

      // fetch the part group stack top
			if (fPartGroupsStack.size () == 0) {
        std::stringstream ss;

        ss <<
          "Cannot append part " <<
          part->fetchPartCombinedName () <<
          " to any part group " <<
          " at identity" << identity<<
          ", since the part groups groups stack is empty"<<
					", line " << inputLineNumber;

        musicxmlError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      // append part to current part group
      S_msrPartGroup
        currentPartGroup =
          fPartGroupsStack.top ()->
            getMsrPartGroup ();

      currentPartGroup->
        appendPartToPartGroup (
          part);
//     }

    // handle the part groups groups stopped at identityidentity
    if (fStoppedPartGroupsListsVector.size ()) {
      S_mxsrPartGroupsList
        stoppedPartGroupsList =
          fStoppedPartGroupsListsVector [identity];

      if (stoppedPartGroupsList->size ()) {
        ++gIndenter;

    		const std::list <S_mxsrPartGroup>&
    			stoppedPartGroupsStdList =
    				stoppedPartGroupsList->getMxsrPartGroupsStdList ();

        for (S_mxsrPartGroup partGroup : stoppedPartGroupsStdList) {
          int stopInputLineNumber =
            partGroup->getStopInputLineNumber ();

          S_msrPartGroup
            theMsrPartGroup =
              partGroup->getMsrPartGroup ();

          // fetch the part group stack top
					if (fPartGroupsStack.size () == 0) {
            std::stringstream ss;

            ss <<
              "Cannot 'stop' part group " <<
              partGroup->fetchMsrPartGroupCombinedName () <<
              " at identity" << identity<<
              ", since the stack is empty"<<
							", line " << inputLineNumber;

            musicxmlError (
              gServiceRunData->getInputSourceName (),
              stopInputLineNumber,
              __FILE__, __LINE__,
              ss.str ());
          }

          if (fPartGroupsStack.top () == partGroup) {
            // pop partGroupToBeStopped from the stack

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTracePartGroups ()) {
              std::stringstream ss;

              ss <<
                "Popping part group " <<
                partGroup->
                  fetchMsrPartGroupCombinedName () <<
                " from the stack" <<
                ", line " << stopInputLineNumber;

              gWaeHandler->waeTrace (
                __FILE__, __LINE__,
                ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            fPartGroupsStack.pop ();

            // the implicit outer-most part group isn't contained in any other
            if (partGroup != fImplicitOuterMostPartGroup) {
              // fetch new current part group JMI v0.9.69
							if (fPartGroupsStack.size () == 0) {
#ifdef MF_TRACE_IS_ENABLED
								if (gTraceOahGroup->getTracePartGroups ()) {
									displayAllCollectedData (
										inputLineNumber,
									"the implicit outer-most part group isn't contained in any other");
								}
#endif // MF_TRACE_IS_ENABLED
                std::stringstream ss;

                ss <<
                  "there is no part group in the stack to contain part group " <<
                  partGroup->asString () <<
                  fCurrentPartID << "\"" ;

                mxsr2msrInternalError (
                  gServiceRunData->getInputSourceName (),
                  stopInputLineNumber,
                  __FILE__, __LINE__,
                  ss.str ());
              }

// 							S_mxsrPartGroup
// 								newMxsrPartGroupStackTop =
// 									fPartGroupsStack.front ();
//
//               // partGroup is nested in newMxsrPartGroupStackTop,
//               // do the nesting
//               handlePartGroupsNesting (
//                 stopInputLineNumber,
//                 partGroup,
//                 newMxsrPartGroupStackTop);
            }
          }

          else {
#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTracePartGroups ()) {
              displayAllCollectedData (
                stopInputLineNumber,
                "UPON overlapping part groups");
            }
#endif // MF_TRACE_IS_ENABLED

            std::stringstream ss;

            ss <<
              std::endl <<
              "There are overlapping part groups, namely: " <<
              std::endl <<
              gTab << partGroup->asString () <<
              std::endl <<
              "and" <<
              std::endl <<
              gTab << fPartGroupsStack.top ()->asString () <<
              std::endl;

/* JMI
            // fetch the identitys in the intersection
            int
              startOne =
                partGroup->getPartGroupIdentity (),
              startTwo =
                fPartGroupsStack.front ()->getPartGroupIdentity (),
              stopOne =
                partGroup->getStopIdentity(),
              stopTwo =
                fPartGroupsStack.front ()->getStopIdentity();

            int firstCommonPosision = startOne;
            if (startTwo > startOne) {
              firstCommonPosision = startTwo;
            }

            int lastCommonPosision = stopOne;
            if (stopTwo < stopOne) {
              lastCommonPosision = stopTwo;
            }

            ss <<
              std::endl <<
              "The parts they share are:" <<
              std::endl;

            ss <<
              "(identitys range is " <<
              firstCommonPosision << ".." << lastCommonPosision <<
              ")" <<
              std::endl;

            for (int m = firstCommonPosision; m <= lastCommonPosision; ++m) {
              S_msrPart
                part =
                  fPartsVector [m];

              ss <<
                gTab <<
                part->fetchPartCombinedName () <<
                ", line " << part->getInputStartLineNumber () <<
                std::endl;
            } // for
*/

            ss <<
              std::endl <<
              regex_replace (
R"(Please contact the maintainers of MusicFormats (see option '-c, -contact'):
  either you found a bug in the EXECUTABLE translator,
  or this MusicXML data is the first-ever real-world case
  of a score exhibiting overlapping part groups)",
              std::regex ("EXECUTABLE"),
              gOahOahGroup->getOahOahGroupServiceName ()) <<
							", line " << inputLineNumber;

            musicxmlError (
              gServiceRunData->getInputSourceName (),
              stopInputLineNumber,
              __FILE__, __LINE__,
              ss.str ());
          }
        } // for

        --gIndenter;
      }
    }

    // handle the part groups started at identityidentity // JMI v0.9.69
    if (fStartedPartGroupsListsVector.size ()) {
      S_mxsrPartGroupsList
        startedMxsrPartGroupsList =
          fStartedPartGroupsListsVector [identity];

      if (startedMxsrPartGroupsList->size ()) {
        ++gIndenter;

    		const std::list <S_mxsrPartGroup>&
    			startedPartGroupsStdList =
    				startedMxsrPartGroupsList->getMxsrPartGroupsStdList ();

        for (S_mxsrPartGroup partGroup : startedPartGroupsStdList) {
          S_msrPartGroup
            theMsrPartGroup =
              partGroup->getMsrPartGroup ();

          // make it the new current part group
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTracePartGroups ()) {
            std::stringstream ss;

            ss <<
              "Pushing part group " <<
              partGroup->asString () <<
              " onto the part groups stack" <<
              ", line " << inputLineNumber;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fPartGroupsStack.push (
            partGroup);
        } // for

        --gIndenter;
      }
    }

// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTracePartGroups ()) {
//       std::stringstream ss;
//
//       ss <<
//         "AT identity" << identity;
//
//       displayAllCollectedData (
//         inputLineNumber,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handleBOFPartGroupsNestingBOFAndScorePartsAllocation");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
S_msrStaff mxsr2msrSkeletonBuilder::createStaffInCurrentPartIfNotYetDone (
  int inputLineNumber,
  int staffNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "createStaffInCurrentPartIfNotYetDone()" <<
      ", staffNumber: " << staffNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) {
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaffKind::kStaffKindRegular,
          staffNumber);

		// register the current staff number in the service run data
		S_mfServiceRunData
			serviceRunData =
				gServiceRunData;

		serviceRunData->
			setCurrentStaveNumber (
				staffNumber);
	}

  return staff;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrSkeletonBuilder::createRegularVoiceInStaffIfNotYetDone (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
  // the voice number is relative to a part

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "createRegularVoiceInStaffIfNotYetDone()" <<
      ", staffNumber: " << staffNumber <<
      ", voiceNumber: " << voiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the staff if not yet done
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        staffNumber);

  // is voice already present in staff?
  S_msrVoice
    voice =
      staff->
        fetchRegularVoiceFromStaffByItsNumber (
          inputLineNumber,
          voiceNumber);

  if (! voice) {
    // create the voice and append it to the staff
    voice =
      staff->
        createRegularVoiceInStaffByItsNumber (
          inputLineNumber,
          voiceNumber,
          fCurrentMeasureNumber,
          fPartGroupsStack.top ()->getMsrPartGroup ());

		// register the current staff number in the service run data
		S_mfServiceRunData
			serviceRunData =
				gServiceRunData;

		serviceRunData->
			setCurrentVoiceNumber (
				voice->getVoiceNumber ());
  }

  return voice;
}

S_msrVoice mxsr2msrSkeletonBuilder::createPartHarmoniesVoiceIfNotYetDone (
  int              inputLineNumber,
  const S_msrPart& theMsrPart)
{
  // is the harmonies voice already present in theMsrPart?
  S_msrVoice
    partHarmoniesVoice =
      theMsrPart->
        getPartHarmoniesVoice ();

  if (! partHarmoniesVoice) {
    // create the harmonies voice and append it to theMsrPart
    partHarmoniesVoice =
      theMsrPart->
        createPartHarmoniesVoice (
          inputLineNumber,
          fCurrentMeasureNumber);
  }

  return partHarmoniesVoice;
}

S_msrVoice mxsr2msrSkeletonBuilder::createPartFiguredBassVoiceIfNotYetDone (
  int              inputLineNumber,
  const S_msrPart& theMsrPart)
{
  // is the figured bass voice already present in theMsrPart?
  S_msrVoice
    partFiguredBassVoice =
      theMsrPart->
        getPartFiguredBassVoice ();

  if (! partFiguredBassVoice) {
    // create the figured bass voice and append it to theMsrPart
    partFiguredBassVoice =
      theMsrPart->
        createPartFiguredBassVoice (
          inputLineNumber,
          fCurrentMeasureNumber);
  }

  return partFiguredBassVoice;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPendingTupletsStopsMap (
	const std::string title,
	int               inputStartLineNumber) const
{
	gLog <<
		">>>>> fPendingTupletsStopsMap contents - " <<
		title <<
		", line " << inputStartLineNumber <<
		std::endl;

	++gIndenter;

	for (
		std::pair <int, S_mxsr2msrPendingTupletStop>
			thePair : fPendingTupletsStopsMap
	) {
		int
			eventSequentialNumber = thePair.first;
		S_mxsr2msrPendingTupletStop
			tupletStopDescription = thePair.second;

		gLog <<
			eventSequentialNumber << ":" <<
			std::endl;

		++gIndenter;
		gLog <<
			tupletStopDescription <<
			std::endl;
		--gIndenter;
	} // for

	--gIndenter;

	gLog <<
		"<<<<<" <<
		std::endl << std::endl;
}

void mxsr2msrSkeletonBuilder::handePendingTupletsStopsIfAny (
	int inputStartLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
		displayPendingTupletsStopsMap (
			"handePendingTupletsStopsIfAny(): ",
			inputStartLineNumber);
  }
#endif // MF_TRACE_IS_ENABLED

	// register pending tuplet events if any
	for (
		auto it = fPendingTupletsStopsMap.cbegin ();
		it != fPendingTupletsStopsMap.cend ();
		// no increment
	) {
// 		int
// 			eventSequentialNumber =
// 				(*it).first;

		S_mxsr2msrPendingTupletStop
			pendingTupletStop =
				(*it).second;

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
// 		// sanity check
// 		int
// 			pendingTupletStopEventSequentialNumber =
// 				pendingTupletStop->getEventSequentialNumber ();
//
// 		if (eventSequentialNumber != pendingTupletStopEventSequentialNumber) {
// 			std::stringstream ss;
//
// 			ss <<
// 				"eventSequentialNumber mismatch in fPendingTupletsStopsMap: "
// 				"eventSequentialNumber: E" <<
// 				eventSequentialNumber <<
// 				", pendingTupletStopEventSequentialNumber: E" <<
// 				pendingTupletStopEventSequentialNumber;
//
// // 			mfAssert (
// // 				__FILE__, __LINE__,
// // 				false,
// // 				ss.str ());
// 		}
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

		// register the tuplet end event with the corresponding info,
		// except the input start end and end line numbers,
		// which are those of the current note
		fResultingEventsCollection.registerTupletEvent (
			pendingTupletStop->getNoteSequentialNumber (),
			pendingTupletStop->getStaffNumber (),
			pendingTupletStop->getVoiceNumber (),
			mxsrTupletEventKind::kEventTupletEnd,
			pendingTupletStop->getTupletNumber (),
			inputStartLineNumber,
			inputStartLineNumber); // should be inputEndLineNumber JMI v0.9.72

		// forget about the pending tuplet stop
		it = fPendingTupletsStopsMap.erase (it);
	} // for
}

//________________________________________________________________________

void mxsr2msrSkeletonBuilder::visitStart (S_score_partwise& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_partwise" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing the score partwise";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fScoreMeasuresNumber = 0;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_score_partwise& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_partwise" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the first and last measure numbers and measures number in the score JMI v0.9.68
  fMsrScore->
    setScoreFirstMeasureNumber (
      fScoreFirstMeasureNumber);
  fMsrScore->
    setScoreLastMeasureNumber (
      fScoreLastMeasureNumber);

  fMsrScore->
    setScoreMeasuresNumber (
      fScoreMeasuresNumber);

  // register the first and last measure numbers and measures number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setScoreFirstMeasureNumber (
      fScoreFirstMeasureNumber);
  serviceRunData->
    setScoreLastMeasureNumber (
      fScoreLastMeasureNumber);

  serviceRunData->
    setScoreMeasuresNumber (
      fScoreMeasuresNumber);

/* JMI ??? THROW AWAY ??? v0.9.69
  // fetch the identification from the credits if any
  fMsrScore ->
    fetchIdentificationFromCreditsIfAny (
      elt->getInputStartLineNumber ());
      */
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_work_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_number" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationWorkNumber (
      elt->getInputStartLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_work_title& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_title" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string workCreditTypeTitle = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationWorkCreditTypeTitle (
      elt->getInputStartLineNumber (),
      workCreditTypeTitle);
}

void mxsr2msrSkeletonBuilder::visitStart (S_opus& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_opus" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string opus = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationOpus (
      elt->getInputStartLineNumber (),
      opus);
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_number" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationMovementNumber (
      elt->getInputStartLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_title& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_title" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string movementTitle = elt->getValue ();

  // remove HTML entities if any // JMI option for that?
/*
  mfConvertHTMLEntitiesToPlainCharacters (
    movementTitle);
    */

  fMsrScore->getIdentification () ->
    setIdentificationMovementTitle (
      elt->getInputStartLineNumber (),
      movementTitle);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_identification" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitEnd (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_identification" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_creator& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_creator" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string creatorType = elt->getAttributeValue ("type");
  std::string creatorValue = elt->getValue ();

  if      (creatorType == "composer") {
    fMsrScore->getIdentification () ->
      appendComposer (
        elt->getInputStartLineNumber (),
        creatorValue);
  }

  else if (creatorType == "arranger") {
    fMsrScore->getIdentification () ->
      appendArranger (
        elt->getInputStartLineNumber (),
        creatorValue);
  }

  else if (creatorType == "lyricist") {
    fMsrScore->getIdentification () ->
      appendLyricist (
        elt->getInputStartLineNumber (),
        creatorValue);

    // should we use lyricists as poets?
    if (gGlobalMxsr2msrOahGroup->getUseLyricistsAsPoets ()) {
      fMsrScore->getIdentification () ->
        appendPoet (
          elt->getInputStartLineNumber (),
          elt->getValue ());
    }
  }

  else if (creatorType == "poet") {
    fMsrScore->getIdentification () ->
      appendPoet (
        elt->getInputStartLineNumber (),
        elt->getValue ());
  }

  else if (creatorType == "translator") {
    fMsrScore->getIdentification () ->
      appendTranslator (
        elt->getInputStartLineNumber (),
        creatorValue);
  }

  else if (creatorType == "artist") {
    fMsrScore->getIdentification () ->
      appendArtist (
        elt->getInputStartLineNumber (),
        creatorValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "creator type \"" << creatorType <<
      "\" is unknown" <<
      ", line " << elt->getInputStartLineNumber ();

// JMI    musicxmlError (
    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
//      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_rights& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rights" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      elt->getInputStartLineNumber (),
      rightsValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_source& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_source" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  std::string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      elt->getInputStartLineNumber (),
      rightsValue);
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_relation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_relation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  std::string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      elt->getInputStartLineNumber (),
      rightsValue);
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_encoding& elt)
{
/*
<!--
	Encoding contains information about who did the digital
	encoding, when, with what software, and in what aspects.
	Standard type values for the encoder element are music,
	words, and arrangement, but other types may be used. The
	type attribute is only needed when there are multiple
	encoder elements.

	The supports element indicates if the encoding supports
	a particular MusicXML element. This is recommended for
	elements like beam, stem, and accidental, where the
	absence of an element is ambiguous if you do not know
	if the encoding supports that element. For Version 2.0,
	the supports element is expanded to allow programs to
	indicate support for particular attributes or particular
	values. This lets applications communicate, for example,
	that all system and/or page breaks are contained in the
	MusicXML file.
-->
<!ELEMENT encoding ((encoding-date | encoder | software |
	encoding-description | supports)*)>
<!ELEMENT encoding-date %yyyy-mm-dd;>
<!ELEMENT encoder (#PCDATA)>
<!ATTLIST encoder
    type CDATA #IMPLIED
>
<!ELEMENT software (#PCDATA)>
<!ELEMENT encoding-description (#PCDATA)>
<!ELEMENT supports EMPTY>
<!ATTLIST supports
    type %yes-no; #REQUIRED
    element CDATA #REQUIRED
    attribute CDATA #IMPLIED
    value CDATA #IMPLIED
>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "*** Analyzing S_encoding ***" <<
      ", " << elt->getValue ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fScoreMeasuresNumber = 0;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_encoding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_encoding" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_date& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_date" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputStartLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoder& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoder" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputStartLineNumber (),
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_software& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_software" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string softwareValue = elt->getValue ();

  // convert clef to lower case for analysis
  std::string softwareValueToLower = mfStringToLowerCase (softwareValue);

  if (softwareValueToLower.find ("cubase") != std::string::npos) {
    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      std::string ("<software /> contains 'Cubase'")
      	+
      ", line "
      	+
      std::to_string (elt->getInputStartLineNumber ()));

    // is the '-cubase' option set???
    if (gGlobalMxsr2msrOahGroup->getCubase ()) {
      S_oahElement
        cubaseOption =
          gGlobalMxsrOahGroup->
            getUpLinkToHandler ()->
              fetchNameInNamesToElementsMap ("cubase");

      if (
        // combined booleans atom?
        S_oahCombinedBooleansAtom
          combinedBooleanAtom =
            dynamic_cast<oahCombinedBooleansAtom*>(&(*cubaseOption))
      ) {
        // handle it at once
#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            "Setting '-cubase' option";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        combinedBooleanAtom->
          setCombinedBooleanVariables (true);
      }

      gGlobalMxsr2msrOahGroup->setCubase (); // JMI
    }
  }

  fMsrScore->getIdentification () ->
    appendSoftware (
      elt->getInputStartLineNumber (),
      softwareValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_description& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_description" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputStartLineNumber (),
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_supports& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_supports" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputStartLineNumber (),
      elt->getValue ());
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  std::string miscellaneousFielValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (
    miscellaneousFielValue);

  fMsrScore->getIdentification () ->
    setIdentificationMiscellaneousField (
      elt->getInputStartLineNumber (),
      miscellaneousFielValue);
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_miscellaneous" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_miscellaneous_field& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous_field" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string miscellaneousFielValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (
    miscellaneousFielValue);

  fMsrScore->getIdentification () ->
    setIdentificationMiscellaneousField (
      elt->getInputStartLineNumber (),
      miscellaneousFielValue);
}

//________________________________________________________________________
/*
<!--
  Credit elements refer to the title, composer, arranger,
  lyricist, copyright, dedication, and other text, symbols,
  and graphics that commonly appear on the first page of a
  score. The credit-words, credit-symbol, and credit-image
  elements are similar to the words, symbol, and image
  elements for directions. However, since the credit is not
  part of a measure, the default-x and default-y attributes
  adjust the origin relative to the bottom left-hand corner
  of the page. The  enclosure for credit-words and
  credit-symbol is none by default.

  By default, a series of credit-words and credit-symbol
  elements within a single credit element follow one another
  in sequence visually. Non-identityal formatting attributes
  are carried over from the previous element by default.

  The page attribute for the credit element specifies the page
  number where the credit should appear. This is an integer
  value that starts with 1 for the first page. Its value is 1
  by default. Since credits occur before the music, these page
  numbers do not refer to the page numbering specified by the
  print element's page-number attribute.

  The credit-type element indicates the purpose behind a
  credit. Multiple types of data may be combined in a single
  credit, so multiple elements may be used. Standard values
  include page number, title, subtitle, composer, arranger,
  lyricist, rights, and part name.
-->
<!ELEMENT credit
  (credit-type*, link*, bookmark*,
   (credit-image |
    ((credit-words | credit-symbol),
     (link*, bookmark*, (credit-words | credit-symbol))*)))>
<!ATTLIST credit
    page NMTOKEN #IMPLIED
    %optional-unique-id;
>

<!ELEMENT credit-type (#PCDATA)>

<!ELEMENT credit-words (#PCDATA)>
<!ATTLIST credit-words
    %text-formatting;
    %optional-unique-id;
>

<!--
  The credit-symbol element specifies a musical symbol
  using a canonical SMuFL glyph name.
-->
<!ELEMENT credit-symbol (#PCDATA)>
<!ATTLIST credit-symbol
    %symbol-formatting;
    %optional-unique-id;
>

<!ELEMENT credit-image EMPTY>
<!ATTLIST credit-image
    source CDATA #REQUIRED
    type CDATA #REQUIRED
    height %tenths; #IMPLIED
    width %tenths; #IMPLIED
    %identity;
    %halign;
    %valign-image;
    %optional-unique-id;
>
*/

void mxsr2msrSkeletonBuilder::visitStart (S_credit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int creditPageNumber =
    elt->getAttributeIntValue ("page", 0);

  // create the credit
  fCurrentCredit =
    msrCredit::create (
      elt->getInputStartLineNumber (),
      creditPageNumber);

  // append it to the score
  fMsrScore->
    appendCreditToScore (fCurrentCredit);
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string creditTypeValue = elt->getValue ();

  msrCreditTypeKind
    creditTypeKind =
      msrCreditTypeKind::kCreditType_UNKNOWN_; // default value

  if      (creditTypeValue == "page number")
    creditTypeKind = msrCreditTypeKind::kCreditTypeNumber;
  else if (creditTypeValue == "title")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeTitle;
  else if (creditTypeValue == "subtitle")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeSubtitle;
  else if (creditTypeValue == "composer")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeComposer;
  else if (creditTypeValue == "arranger")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeArranger;
  else if (creditTypeValue == "lyricist")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeLyricist;
  else if (creditTypeValue == "rights")
    creditTypeKind =  msrCreditTypeKind::kCreditTypeRights;
  else if (creditTypeValue == "part name")
    creditTypeKind =  msrCreditTypeKind::kCreditTypePartName;
  else {
    creditTypeKind =  msrCreditTypeKind::kCreditTypeOther;
  }

  // set the current credit type kind
  fCurrentCredit->
    setCreditTypeKind (creditTypeKind);
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_symbol& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_symbol" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_image& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_image" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_words& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_words" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  <credit page="1">
    <credit-words default-x="548" default-y="1382" font-family="FreeSerif" font-size="26" font-weight="bold" justify="center" valign="top" xml:space="preserve">"R E Q U I E M"    from    D E A T H N O T E</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="548" default-y="1331" font-family="FreeSerif" font-size="16" font-style="italic" justify="center" valign="top">Theme from L's death. For SATB choir.</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="1046" default-y="1253" font-family="FreeSerif" font-size="12" justify="right" valign="bottom">Yoshihisa Hirano &amp; Hideki Taniuchi</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="51" default-y="1253" font-family="FreeSerif" font-size="12" valign="bottom" xml:lang="fr">arrangement and lyrics by Andrés Rojas</credit-words>
  </credit>
  */

  std::string creditWordsContents =
    elt->getValue ();

  // identity
  float creditWordsDefaultX=
    elt->getAttributeFloatValue ("default-x", 0.0);
  float creditWordsDefaultY=
    elt->getAttributeFloatValue ("default-y", 0.0);

  // font family
  std::string creditWordsFontFamily =
    elt->getAttributeValue ("font-family");

  // font size
  float creditWordsFontSize =
    elt->getAttributeFloatValue ("font-size", 0.0);

  // font weight
  std::string creditWordsFontWeightString =
    elt->getAttributeValue ("font-weight"); // JMI etc

  msrFontWeightKind
    creditWordsFontWeightKind =
      msrFontWeightKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsFontWeightString);

  // font style
  std::string creditWordsFontStyleString =
    elt->getAttributeValue ("font-style");

  msrFontStyleKind
    creditWordsFontStyleKind =
      msrFontStyleKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsFontStyleString);

  // justify
  std::string creditWordsJustifyString =
    elt->getAttributeValue ("justify");

  msrJustifyKind
    creditWordsJustifyKind =
      msrJustifyKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsJustifyString);

  // halign
  std::string creditWordsHAlignString =
    elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    creditWordsHorizontalAlignment =
      msrHorizontalAlignmentKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsHAlignString);

  // valign
  std::string creditWordsVAlignString =
    elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    creditWordsVerticalAlignment =
      msrVerticalAlignmentKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsVAlignString);

  // XMLLang
  std::string creditWordsXMLLangString =
    elt->getAttributeValue ("xml:lang");

  msrXMLLangKind
    creditWordsXMLLang =
      msrXMLLangKindFromString (
        elt->getInputStartLineNumber (),
        creditWordsXMLLangString);

  // create the credit words
  S_msrCreditWords
    creditWords =
      msrCreditWords::create (
        elt->getInputStartLineNumber (),
        creditWordsContents,
        creditWordsDefaultX,
        creditWordsDefaultY,
        creditWordsFontFamily,
        creditWordsFontSize,
        creditWordsFontWeightKind,
        creditWordsFontStyleKind,
        creditWordsJustifyKind,
        creditWordsHorizontalAlignment,
        creditWordsVerticalAlignment,
        creditWordsXMLLang);

  // append it to the current credit
  fCurrentCredit->
    appendCreditWordsToCredit (
      creditWords);
}

void mxsr2msrSkeletonBuilder::visitEnd (S_credit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_credit" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current credit
  fCurrentCredit = nullptr;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_list& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_list" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing part list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_list& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_list" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

//   // do the HARD job...
//   handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
//     elt->getInputStartLineNumber ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      elt->getInputStartLineNumber (),
      "visitEnd (S_part_list& elt)");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_group& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_group" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // part group number

  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);

  // part group type

  std::string partGroupType = elt->getAttributeValue ("type");

  fCurrentPartGroupTypeKind = msrPartGroupTypeKind::kPartGroupTypeNone;

  if      (partGroupType == "start")
    fCurrentPartGroupTypeKind = msrPartGroupTypeKind::kPartGroupTypeStart;
  else if (partGroupType == "stop")
    fCurrentPartGroupTypeKind = msrPartGroupTypeKind::kPartGroupTypeStop;
  else {
    std::stringstream ss;

    ss <<
      "unknown part group type \"" <<
      partGroupType <<
      "\""<<
      ", line " << elt->getInputStartLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // part group print style

  std::string printStyle = elt->getAttributeValue ("print-style"); // JMI

  // part group print object

  std::string printObject = elt->getAttributeValue ("print-object"); // JMI

  // handle part group type

  switch (fCurrentPartGroupTypeKind) {
    case msrPartGroupTypeKind::kPartGroupTypeNone:
      // should not occur
      break;

    case msrPartGroupTypeKind::kPartGroupTypeStart:
      fCurrentPartGroupName = "";
      fCurrentPartGroupNameDisplayText = "";
      fCurrentPartGroupAccidentalText = "";
      fCurrentPartGroupAbbreviation = "";
      fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolNone;
      fCurrentPartGroupSymbolDefaultX = INT_MIN;
      fCurrentPartGroupBarLineKind = msrPartGroupBarLineKind::kPartGroupBarLineYes;
      break;

    case msrPartGroupTypeKind::kPartGroupTypeStop:
      break;
  } // switch
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_name& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_name" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartGroupName = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_name_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  fOnGoingPartGroupNameDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_group_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_group_name_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPartGroupNameDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_display_text& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_text" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPartGroupNameDisplay) {
    fCurrentPartGroupNameDisplayText =
      elt->getValue ();
  }
  else if (fOnGoingPartNameDisplay) {
    fCurrentPartNameDisplayText =
      elt->getValue ();
  }
  else if (fOnGoingPartAbbreviationDisplay) {
    fCurrentPartAbbreviationDisplayText =
      elt->getValue ();
  }
  else {
    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      std::string (
      	"<display-text /> is out of context")
      		+
      	", line "
      		+
      	std::to_string (elt->getInputStartLineNumber ()));
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_accidental_text& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental_text" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_abbreviation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_abbreviation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_symbol& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_symbol" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string groupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolKind =
    msrPartGroupSymbolKind::kPartGroupSymbolNone; // default value

  if      (groupSymbol == "brace")
    fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolBrace;

  else if (groupSymbol == "bracket")
    fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolBracket;

  else if (groupSymbol == "line")
    fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolLine;

  else if (groupSymbol == "square")
    fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolSquare;

  else if (groupSymbol == "none")
    fCurrentPartGroupSymbolKind = msrPartGroupSymbolKind::kPartGroupSymbolNone;

  else {
    if (groupSymbol.size ()) {
      // part group type may be absent
      std::stringstream ss;

      ss <<
        "unknown part group symbol \"" + groupSymbol + "\""<<
				", line " << elt->getInputStartLineNumber ();

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_barline& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_barline" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string groupBarLine = elt->getValue ();

  // check part group barLine
  if      (groupBarLine == "yes")
    fCurrentPartGroupBarLineKind = msrPartGroupBarLineKind::kPartGroupBarLineYes;
  else if (groupBarLine == "no")
    fCurrentPartGroupBarLineKind = msrPartGroupBarLineKind::kPartGroupBarLineNo;
  else {
    std::stringstream ss;

    ss <<
      "unknown part group barLine \"" + groupBarLine + "\""<<
      ", line " << elt->getInputStartLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_part_group& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_group" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Handling part group '" <<
      fCurrentPartGroupNumber <<
      "', type: \"" <<
      msrPartGroupTypeKindAsString (
        fCurrentPartGroupTypeKind) << "\""  <<
      ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity<<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // handle part group type
  switch (fCurrentPartGroupTypeKind) {

    case msrPartGroupTypeKind::kPartGroupTypeStart:
      // handle the part group start
      handlePartGroupStart (
        elt->getInputStartLineNumber ());
      break;

    case msrPartGroupTypeKind::kPartGroupTypeStop:
      // handle the part group stop
      handlePartGroupStop (
        elt->getInputStartLineNumber ());
      break;

    case msrPartGroupTypeKind::kPartGroupTypeNone:
      // should not occur
      break;
  } // switch

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_score_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentScorePartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Found <score-part /> \"" << fCurrentScorePartID << "\"" <<
      " in part list" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is the part id a pure number?
  std::string regularExpression (
    "[[:space:]]*"
    "[[:digit:]]+"
    "[[:space:]]*"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (fCurrentScorePartID, sm, e);

  if (sm.size () == 1) {
    std::stringstream ss;

    ss <<
      "PartID \"" << fCurrentScorePartID << "\"" <<
      " is a pure number" <<
      ", line " << elt->getInputStartLineNumber ();

    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());
  }

  // initialize fields
  fCurrentPartName = "";
  fCurrentPartNameDisplayText = "";

  fCurrentPartAbbreviation = "";
  fCurrentPartAbbreviationDisplayText = "";

  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_name& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_name" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartName = elt->getValue ();

  std::string printStyle = elt->getAttributeValue ("print-style"); // JMI
  if (printStyle == "JMI") {
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  if (printObjectString == "JMI") {
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_name_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPartNameDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_name_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPartNameDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_abbreviation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_abbreviation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartAbbreviation = elt->getValue ();

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_abbreviation_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPartAbbreviationDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_abbreviation_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPartAbbreviationDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_instrument_name& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartInstrumentName = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_instrument_abbreviation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_abbreviation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitEnd (S_score_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
  std::stringstream ss;

  ss <<
    "Incrementing fCurrentPartGroupIdentity to " <<
    fCurrentPartGroupIdentity<<
    ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string scorePartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
  std::stringstream ss;

  ss <<
    "Handling score part with ID \"" << scorePartID << "\"" <<
    ", fCurrentPartGroupIdentity: " << fCurrentPartGroupIdentity<<
    ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create the MSR part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Creating MSR part \"" << scorePartID << "\"" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrPart
    theMsrPart =
      msrPart::create (
        elt->getInputStartLineNumber (),
        scorePartID,
        0); // fPartUpLinkToPartGroup will be set later

  // populate theMsrPart
  theMsrPart->
    setPartName (
      fCurrentPartName);
  theMsrPart->
    setPartNameDisplayText (
      fCurrentPartNameDisplayText);

  theMsrPart->
    setPartAbbreviation (
      fCurrentPartAbbreviation);
  theMsrPart->
    setPartAbbreviationDisplayText (
      fCurrentPartAbbreviationDisplayText);

  theMsrPart->
    setPartInstrumentName (
      fCurrentPartInstrumentName);
  theMsrPart->
    setPartInstrumentAbbreviation (
      fCurrentPartInstrumentAbbreviation);

  // register theMsrPart in the MSR parts data
  registerPart (
    elt->getInputStartLineNumber (),
    theMsrPart);

	// set theMsrPart's part uplink to the part group it belongs to
  theMsrPart->
  	setPartUpLinkToPartGroup (
  		fPartGroupsStack.top ()->
  			getMsrPartGroup ()); // JMI HARD v0.9.69
  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      " partID \"" << fCurrentPartID << "\"" <<
//       "partName \"" << fCurrentPartName << "\"" << JMI from fPartGroupElementsList ??? v0.9.67
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentPartID.size () == 0) {
		std::stringstream ss;

		ss <<
			"part ID is empty in <part />";

		musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // is this part already known?
  if (fPartsMap.count (fCurrentPartID)) {
    fCurrentPart = // used thoughoutfRepeatHasBeenCreatedForCurrentPart
      fPartsMap [fCurrentPartID];
  }

  else {
    std::stringstream ss;

    ss <<
      "part \"" <<
      fCurrentPartID <<
      "\" is not known in the <part-list />" <<
      ", line " << elt->getInputStartLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing part \"" << fCurrentPartID << "\" -- start";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  if (fCurrentPart) {
    serviceRunData->
      setCurrentPartIDAndName (
        fCurrentPart->getPartIDAndName ());
  }

  // measures
  fPartNumberOfMeasures = 0;

  // staves and voices
  fCurrentNoteStaffNumber  = K_STAFF_NUMBER_UNKNOWN_;
  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  fCurrentNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
  fPreviousNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
      "--------------------------------------------";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // handle current part number of measures
  if (fScoreMeasuresNumber == 0) {
    fScoreMeasuresNumber =
      fPartNumberOfMeasures;
  }
  else {
    // is the part number of measures consistent?
    if (fPartNumberOfMeasures != fScoreMeasuresNumber) {
      std::stringstream ss;

      ss <<
        "part " << fCurrentPart->fetchPartCombinedName () <<
        " has " << fPartNumberOfMeasures <<
        " measures while the other ones have " << fScoreMeasuresNumber<<
				", line " << elt->getInputStartLineNumber ();

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
        ss.str ());
    }
  }

  // set current part's number of measures
  fCurrentPart->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

//   // sort the part staves by increasing number,
//   // because they can be out of order in the MusicXML data
  fCurrentPart->
    sortStavesByIncreasingNumber ();

/* JMI v0.9.69
  // are there more staves in <staves/> that specified with <staff/>?
  fCurrentPart->
    addAVoiceToStavesThatHaveNone (
      elt->getInputStartLineNumber ());
*/

  // assign sequential numbers to the voices in the part
  fCurrentPart->
    assignSequentialNumbersToRegularVoicesInPart (
      elt->getInputStartLineNumber ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    gLog <<
    	std::endl <<
      "<!--=== "
      "The regular voices map of part \"" <<
      fCurrentPart->getPartName () << "\"" <<
      " contains:" <<
      std::endl;

    ++gIndenter;

		fCurrentPart->displayPartStaffVoicesMap (
			elt->getInputStartLineNumber (),
			"mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)");

		gLog << std::endl << std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
// void mxsr2msrSkeletonBuilder::visitStart (S_staves& elt)
// {
// 	// JMI v0.9.72 should be ignored!
//
// // #ifdef MF_TRACE_IS_ENABLED
// //   if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
// //     std::stringstream ss;
// //
// //     ss <<
// //       "--> Start visiting S_direction" <<
// //       ", line " << elt->getInputStartLineNumber ();
// //
// //     gWaeHandler->waeTrace (
// //       __FILE__, __LINE__,
// //       ss.str ());
// //   }
// // #endif // MF_TRACE_IS_ENABLED
// //
// //   int stavesNumber = int(*elt);
// //
// // #ifdef MF_TRACE_IS_ENABLED
// //   if (gTraceOahGroup->getTraceStaves ()) {
// //     std::stringstream ss;
// //
// //     switch (stavesNumber) {
// //       case 0:
// //         ss <<
// //           "There isn't any explicit staff (hence 1 by default)"; // JMI
// //         break;
// //
// //       case 1:
// //         ss <<
// //           "There is 1 staff";
// //         break;
// //
// //       default:
// //         ss <<
// //           "There are " << stavesNumber << " staves";
// //     } // switch
// //
// //     ss <<
// //       " in part " << fCurrentPart->fetchPartCombinedName();
// //
// //     gWaeHandler->waeTrace (
// //       __FILE__, __LINE__,
// //       ss.str ());
// //   }
// // #endif // MF_TRACE_IS_ENABLED
//
// //   if (stavesNumber > 1) {
// //     // add stavesNumber staves to current part // JMI v0.9.72 which staff numbers...???
// //     int n = 1;
// //
// //     while (n <= stavesNumber) {
// //     /* JMI v0.9.69
// //       fCurrentPart->
// //         addStaffToPartByItsNumber (
// //           elt->getInputStartLineNumber (),
// //           msrStaffKind::kStaffKindRegular,
// //           n);
// //           */
// //       S_msrStaff
// //         dummyStaff = // JMI
// //           createStaffInCurrentPartIfNotYetDone (
// //             elt->getInputStartLineNumber (),
// //             n);
// //
// //       ++n;
// //     } // while
// //   }
// }

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_staff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int staffNumber = int(*elt);

  // the staff number should be positive
  if (staffNumber <= 0) {
    std::stringstream ss;

    ss <<
      "staff number " << staffNumber <<
      " is not positive" <<
      ", line " << elt->getInputStartLineNumber ();

      mfAssert (
        __FILE__, __LINE__,
				false,
				ss.str ());
  }

	fCurrentNoteStaffNumber = staffNumber;

  if (fCurrentStaffNumber == K_STAFF_NUMBER_UNKNOWN_) {
  	// this is the first <staff /> in the current part // JMI v0.9.72 ???
  	fCurrentStaffNumber = fCurrentNoteStaffNumber;
	}

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      "visitStart (S_staff& elt)" <<
      ", staffNumber: " << staffNumber <<
      ", fCurrentStaffNumber: " << fCurrentStaffNumber <<
      ", fCurrentNoteStaffNumber: " << fPreviousNoteStaffNumber <<
      ", fPreviousNoteStaffNumber: " << fPreviousNoteStaffNumber <<
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) { // JMI v0.9.72
    // regular staff indication in note/rest, fine
  }
  else {
    // JMI ???
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_voice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_voice" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteVoiceNumber = int(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      "visitStart (S_voice& elt)" <<
      ", fCurrentNoteVoiceNumber: " << fCurrentNoteVoiceNumber <<
      ", fPreviousNoteVoiceNumber: " << fPreviousNoteVoiceNumber <<
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) { // JMI
    // regular voice indication in note/rest, fine
  }
  else {
    // JMI ???
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_print& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_print" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPrint = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_print& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_print" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingPrint = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  fCurrentMeasureNumber = // JMI local variable???
    elt->getAttributeValue ("number");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== " <<
      "fCurrentPartName \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID \"" << fCurrentPartID << "\")" <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take this measure into account
  ++fPartNumberOfMeasures;

  if (fScoreFirstMeasureNumber == K_MEASURE_NUMBER_UNKNOWN_) { // JMI v0.9.68
  	fScoreFirstMeasureNumber = fCurrentMeasureNumber;
  }
	fScoreLastMeasureNumber = fCurrentMeasureNumber;

  // register the current measure number in the service run data for use by OAH
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (
      fCurrentMeasureNumber);

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// an end of measure enforces the end of a chord or tuplets  CHORD_TUP
	// the current note is then the one preceding the end of measure

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
		gLog <<
			"===> visitEnd (S_measure& elt)" <<
			", fCurrentNoteStartInputLineNumber: " <<
			fCurrentNoteStartInputLineNumber <<

			", currentEventSequentialNumber: " <<
			fResultingEventsCollection.getCurrentEventSequentialNumber () <<

			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<

			", fCurrentNoteBelongsToAChord: " <<
			fCurrentNoteBelongsToAChord <<
			", fPreviousNoteBelongsToAChord: " <<
			fPreviousNoteBelongsToAChord <<
			", fPreviousNoteIsATakeOffCandidate: " <<
			fPreviousNoteIsATakeOffCandidate <<
      ", line " << elt->getInputStartLineNumber () <<
			std::endl;

		if (gTraceOahGroup->getTraceTupletsBasics ()) {
			gLog <<
				", fCurrentTupletNumber: " <<
				fCurrentTupletNumber <<
				std::endl;

			displayPendingTupletsStopsMap (
				"=====> visitEnd (S_measure& elt)",
				fCurrentNoteStartInputLineNumber);
		}
	}
#endif // MF_TRACE_IS_ENABLED

 	// Q_MEASURE

	// register the chord end event if any, before pending tuplets stops if any
	if (fCurrentNoteBelongsToAChord) {
		// the note before the measure end is the last one of the chord
		// it is still the current note

		fResultingEventsCollection.registerChordEvent (
			fCurrentNoteSequentialNumber,
			fCurrentNoteStaffNumber,
			fCurrentNoteVoiceNumber,
			mxsrChordEventKind::kEventChordEnd,
			fCurrentNoteStartInputLineNumber,
			fCurrentNoteEndInputLineNumber);
	}

	else {
// 		// is this note the one that follows the last one of a chord?
// 		if (fPreviousNoteBelongsToAChord) {
// 			// this the note after the last one of the chord
// 			 // we're one note late!
// 			fResultingEventsCollection.registerChordEndEvent (
// 				fPreviousNoteSequentialNumber,
// 			fCurrentNoteSequentialNumber,
// 				fCurrentNoteStaffNumber,
// 				fCurrentNoteVoiceNumber,
// 				fPreviousNoteStartInputLineNumber,
// 				fPreviousNoteEndInputLineNumber,
// 		    fCurrentNoteVoiceNumber);
// 		}
// 		else {
// 		}
	}

 	// Q_MEASURE

	// handle pending tuplet stops if any, after the chord end if any
	handePendingTupletsStopsIfAny (
		fPreviousNoteEndInputLineNumber);

// 	switch (fCurrentTupletTypeKind) {
//     case msrTupletTypeKind::kTupletTypeNone:
//     	// should not occur
//       break;
//
//     case msrTupletTypeKind::kTupletTypeStart:
//     	// should not occur
//       break;
//
//     case msrTupletTypeKind::kTupletTypeContinue:
//     	// nothing to do
//       break;
//
//     case msrTupletTypeKind::kTupletTypeStop:
//     	// nothing to do
//       break;
//
//     case msrTupletTypeKind::kTupletTypeStartAndStopInARow:
//     	// nothing to do
//       break;
// 	} // switch

  --gIndenter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_backup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// forget about the current take off candidate
	fPreviousNoteIsATakeOffCandidate = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_forward& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// forget about the current take off candidate
	fPreviousNoteIsATakeOffCandidate = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_repeat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // direction

  {
    std::string direction = elt->getAttributeValue ("direction");

		msrBarLineRepeatDirectionKind
			barLineRepeatDirectionKind =
	      msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone;

    if       (direction == "forward") {
      barLineRepeatDirectionKind =
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionForward;
    }
    else  if (direction == "backward") {
      barLineRepeatDirectionKind =
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionBackward;
    }
    else {
      std::stringstream ss;

      ss <<
        "repeat direction \"" << direction <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }

		switch (barLineRepeatDirectionKind) {
			case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone:
				break;

			case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionForward:
				if (! fAFirstForwardRepeatHasBeenMet) {
					// this is the first forward repeat
					fAFirstForwardRepeatHasBeenMet = true;
				}
				break;

			case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionBackward:
				if (! fAFirstForwardRepeatHasBeenMet) {
					// this backward repeat is not preceded by a forward repeat
					// hence there is an implicit initial forward repeat
					fResultingEventsCollection.
						setThereIsAnImplicitInitialForwardRepeat ();
				}
				else {
				}
				break;
		} // switch
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// sequential numbers
	fPreviousNoteSequentialNumber = fCurrentNoteSequentialNumber;
	++fCurrentNoteSequentialNumber;

  // number - the default value is implementation dependent, so:
  fCurrentTupletNumber = 1;

	// note staff and voice numbers
	fCurrentNoteStaffNumber = 1; // default value, unless S_staff states otherwise afterwards
	fCurrentNoteVoiceNumber = 1; // default value, unless S_voice states otherwise afterwards

	// chords
	fPreviousNoteBelongsToAChord = fCurrentNoteBelongsToAChord;
	fCurrentNoteBelongsToAChord = false;

	// tuplets
// 	fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeNone;
  fCurrentNoteBelongsToATuplet = false;

	// staff changes
	fPreviousNoteStartInputLineNumber = fCurrentNoteStartInputLineNumber;
	fPreviousNoteEndInputLineNumber = fCurrentNoteEndInputLineNumber;

	// input line numbers
	fCurrentNoteStartInputLineNumber =
		elt->getInputStartLineNumber ();
	fCurrentNoteEndInputLineNumber =
		elt->getInputEndLineNumber ();

  // lyrics
  fCurrentStanzaNumber = K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;

  fOnGoingNote = true;
}

Bool mxsr2msrSkeletonBuilder::handleStaffChangeIfAny (
	int inputStartLineNumber)
{
	Bool result;

  if (
    fCurrentNoteStaffNumber != fPreviousNoteStaffNumber
      &&
    fCurrentNoteVoiceNumber == fPreviousNoteVoiceNumber
      &&
    fPreviousNoteIsATakeOffCandidate
//     fPreviousNoteStaffNumber != K_STAFF_NUMBER_UNKNOWN_ // JMI STAFF_CHANGE v0.9.72
  ) {
    // yes
		result = true;

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
			std::stringstream ss;

			ss <<
				"===> handleStaffChangeIfAny(): There is a staff change event:" <<
				'\n' <<
				"fCurrentNoteStartInputLineNumber: " <<
				fCurrentNoteStartInputLineNumber <<
	// 			", fCurrentNoteEndInputLineNumber: " <<
	// 			fCurrentNoteEndInputLineNumber <<

				", fPreviousNoteStaffNumber: " <<
				fPreviousNoteStaffNumber <<
				", fCurrentNoteStaffNumber: " <<
				fCurrentNoteStaffNumber <<

				", fCurrentNoteVoiceNumber: " <<
				fCurrentNoteVoiceNumber <<
				", fPreviousNoteVoiceNumber: " <<
				fPreviousNoteVoiceNumber <<

				", fPreviousNoteIsATakeOffCandidate: " <<
				fPreviousNoteIsATakeOffCandidate <<

				", line " << fCurrentNoteStartInputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLE

		int
			takeOffStaffNumber = fPreviousNoteStaffNumber,
			landingStaffNumber = fCurrentNoteStaffNumber,

			changingVoiceNumber = fPreviousNoteVoiceNumber,
			landingVoiceNumber = fCurrentNoteVoiceNumber;

		int
			takeOffNoteStartInputLineNumber = fPreviousNoteStartInputLineNumber,
			landingNoteStartInputLineNumber = fCurrentNoteStartInputLineNumber;

		// create the landing staff if not yet done
		S_msrVoice
			landingVoice =
				createRegularVoiceInStaffIfNotYetDone (
					takeOffNoteStartInputLineNumber,
					landingStaffNumber,
					landingVoiceNumber);

    // register a staff change event
    // taking off from the previous note
    // and landing on the current note

		// the airplane takes off upon the previous note
		// and lands upon the current note

		// let's create two staff changes
		// one for take off upon the previous note
		fResultingEventsCollection.registerStaffChangeTakeOff ( // CHORD_TUP JMI v0.9.72
			fPreviousNoteSequentialNumber,
			fPreviousNoteStaffNumber,
			changingVoiceNumber,							// the voice that changes staff
			takeOffStaffNumber,
			landingStaffNumber,
			takeOffNoteStartInputLineNumber,
			landingNoteStartInputLineNumber,
			fPreviousNoteStartInputLineNumber,
			fPreviousNoteEndInputLineNumber);

		// the other one for landing upon the current note
		fResultingEventsCollection.registerStaffChangeLanding ( // CHORD_TUP JMI v0.9.72
			fCurrentNoteSequentialNumber,
			fCurrentNoteStaffNumber,
			changingVoiceNumber,							// the voice that changes staff
			takeOffStaffNumber,
			landingStaffNumber,
			takeOffNoteStartInputLineNumber,
			landingNoteStartInputLineNumber,
			fCurrentNoteStartInputLineNumber,
			fCurrentNoteEndInputLineNumber);
  }

	return result;
}

void mxsr2msrSkeletonBuilder::registerChordEventIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
    	"-->registerChordEventIfAny()"
      ", fCurrentNoteStartInputLineNumber: " <<
      fCurrentNoteStartInputLineNumber <<

      ", fPreviousNoteSequentialNumber: " <<
      fPreviousNoteSequentialNumber <<
      ", fCurrentNoteSequentialNumber: " <<
      fCurrentNoteSequentialNumber <<

      ", fPreviousNoteBelongsToAChord: " <<
      fPreviousNoteBelongsToAChord <<
      ", fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<

      ", fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<

      ", fCurrentNoteBelongsToATuplet: " <<
      fCurrentNoteBelongsToATuplet <<
      std::endl;

    displayPendingTupletsStopsMap (
    	"=====> visitEnd (S_note& elt) 2",
    	fCurrentNoteStartInputLineNumber);

		gLog <<
// 			", fCurrentTupletTypeKind: " <<
// 			fCurrentTupletTypeKind <<

			", fPreviousNoteIsATakeOffCandidate: " <<
			fPreviousNoteIsATakeOffCandidate <<
      ", line " << fCurrentNoteStartInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 	// Q_NOTE

	if (fCurrentNoteBelongsToAChord) {
		// was the previous note also in the chords?
		if (fPreviousNoteBelongsToAChord) {
			// this note is after the second one of the chord
			// wait and see upon the next note or the measure end
		}
		else {
			// this is the second note of the chord
			// we're one note late, hence the previous note is the chord begin
			fResultingEventsCollection.registerChordEvent (
				fPreviousNoteSequentialNumber,
				fPreviousNoteStaffNumber,
				fPreviousNoteVoiceNumber,
				mxsrChordEventKind::kEventChordBegin,
				fPreviousNoteStartInputLineNumber,
				fPreviousNoteEndInputLineNumber);
		}
	}

	else {
		// is this note the one that follows the last one of a chord?
		if (fPreviousNoteBelongsToAChord) {
			// this the note after the last one of the chord
			// we're one note late, hence the previous note is the chord end
			fResultingEventsCollection.registerChordEvent (
				fPreviousNoteSequentialNumber,
				fPreviousNoteStaffNumber,
				fPreviousNoteVoiceNumber,
				mxsrChordEventKind::kEventChordEnd,
				fPreviousNoteStartInputLineNumber,
				fPreviousNoteEndInputLineNumber);
		}
		else {
			// wait and see upon the next note or the measure end
		}
	}
}

void mxsr2msrSkeletonBuilder::registerTupletEventIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
    	"-->registerTupletEventIfAny()"
      ", fCurrentNoteStartInputLineNumber: " <<
      fCurrentNoteStartInputLineNumber <<

      ", fPreviousNoteSequentialNumber: " <<
      fPreviousNoteSequentialNumber <<
      ", fCurrentNoteSequentialNumber: " <<
      fCurrentNoteSequentialNumber <<

      ", fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<

      ", fCurrentNoteBelongsToATuplet: " <<
      fCurrentNoteBelongsToATuplet <<
      std::endl;

    displayPendingTupletsStopsMap (
    	"=====> visitEnd (S_note& elt) 3",
    	fCurrentNoteStartInputLineNumber);

		gLog <<
// 			", fCurrentTupletTypeKind: " <<
// 			fCurrentTupletTypeKind <<

			", fPreviousNoteIsATakeOffCandidate: " <<
			fPreviousNoteIsATakeOffCandidate <<
      ", line " << fCurrentNoteStartInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	for (
		auto it = fPendingTupletsInformationTuplesList.cbegin ();
		it != fPendingTupletsInformationTuplesList.cend ();
		// no increment
	) {
		// extract the values from the tuple
		int 							noteInputStartLineNumber;
		int 							tupletNumber;
		msrTupletTypeKind tupletKind;

		std::tie (
			noteInputStartLineNumber,
			tupletNumber,
			tupletKind) = (*it);

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceTupletsBasics ()) {
			std::stringstream ss;

			ss <<
				"--> std::tie (tuples):"
				", noteInputStartLineNumber: " <<
				noteInputStartLineNumber <<
				", tupletNumber: " <<
				tupletNumber <<
				", tupletKind: " <<
				tupletKind;

// 			displayPendingTupletsStopsMap (
// 				"=====> visitEnd (S_note& elt) 3",
// 				fCurrentNoteStartInputLineNumber);

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		// handle the values
		switch (tupletKind) {
			case msrTupletTypeKind::kTupletTypeNone:
				// handle pending tuplet stops if any
				handePendingTupletsStopsIfAny (
					fCurrentNoteEndInputLineNumber);
				break;

			case msrTupletTypeKind::kTupletTypeStart:
				// handle pending tuplet stops if any
				handePendingTupletsStopsIfAny (
					fCurrentNoteEndInputLineNumber);

				// now register the new tuplet begin event upon the current note
				fResultingEventsCollection.registerTupletEvent (
					fCurrentNoteSequentialNumber,
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber,
					mxsrTupletEventKind::kEventTupletBegin,
					tupletNumber,
					noteInputStartLineNumber,
					fCurrentNoteEndInputLineNumber);
				break;

			case msrTupletTypeKind::kTupletTypeContinue:
				// nothing to do
				break;

			case msrTupletTypeKind::kTupletTypeStop:
				// store the tuplet stop description in fPendingTupletsStopsMap
				// to be handled later,
				/// since it may occur before the actual last note in the tuplet
				// it event sequential number is temporary,
				// it will be set when the tuplet stop event is created

				// the 0 values are temporary and the actual ones will be used later,
				// when  the tuplet stop event is created by registerTupletEvent()
				fPendingTupletsStopsMap.insert (
					std::make_pair (
						fCurrentNoteSequentialNumber,
						mxsr2msrPendingTupletStop::create (
							0,
							fCurrentNoteSequentialNumber,
							fCurrentNoteStaffNumber,
							fCurrentNoteVoiceNumber,
							tupletNumber,
							0,
							0)));
				break;

			case msrTupletTypeKind::kTupletTypeStartAndStopInARow:
				break;
		} // switch

		// forget about the pending tuplet type kind
		it = fPendingTupletsInformationTuplesList.erase (it);
	} // for
}

void  mxsr2msrSkeletonBuilder::visitEnd (S_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 	// Q_NOTE

	// handle staff change if any
	Bool
		thereIsAStaffChange =
			handleStaffChangeIfAny (elt->getInputStartLineNumber ());

	if (! thereIsAStaffChange) {
		// the current note is a regular one

		// create the staff if not yet done
		S_msrStaff
			staff =
				createStaffInCurrentPartIfNotYetDone (
					elt->getInputStartLineNumber (),
					fCurrentNoteStaffNumber);

		// create the voice if not yet done
		S_msrVoice
			noteVoice =
				createRegularVoiceInStaffIfNotYetDone (
					elt->getInputStartLineNumber (),
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber);

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceNotesBasics ()) {
			std::stringstream ss;

			ss <<
				"--> visitEnd (S_note& elt) 1 (all):"
				", fCurrentNoteSequentialNumber: " <<
				fCurrentNoteSequentialNumber <<
				", fPreviousNoteSequentialNumber: " <<
				fPreviousNoteSequentialNumber <<

				", fCurrentNoteStartInputLineNumber: " <<
				fCurrentNoteStartInputLineNumber <<

				", fCurrentNoteStaffNumber: " <<
				fCurrentNoteStaffNumber <<
				", fCurrentNoteVoiceNumber: " <<
				fCurrentNoteVoiceNumber <<

				", getStaffName(): " <<
				staff->getStaffName () <<
				", getVoiceName(): " <<
				noteVoice->getVoiceName () <<

				", fCurrentNoteBelongsToAChord: " <<
				fCurrentNoteBelongsToAChord <<

				", fCurrentNoteBelongsToATuplet: " <<
				fCurrentNoteBelongsToATuplet <<
				std::endl;

			displayPendingTupletsStopsMap (
				"=====> visitEnd (S_note& elt) 1",
				fCurrentNoteStartInputLineNumber);

			gLog <<
	// 			", fCurrentTupletTypeKind: " <<
	// 			fCurrentTupletTypeKind <<

				", fPreviousNoteIsATakeOffCandidate: " <<
				fPreviousNoteIsATakeOffCandidate <<
				", line " << fCurrentNoteStartInputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED
	}

  // are there harmonies attached to the current note?
  if (fThereAreHarmoniesToBeAttachedToCurrentNote) {
    if (gGlobalMxsr2msrOahGroup->getIgnoreHarmonies ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        std::stringstream ss;

        ss <<
          "Ignoring the harmonies" <<
          ", line " <<
          elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
			}
#endif // MF_TRACE_IS_ENABLED
		}
    else {
      // create the part harmonies voice if not yet done
      S_msrVoice
        partHarmoniesVoice =
          createPartHarmoniesVoiceIfNotYetDone (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }

    fThereAreHarmoniesToBeAttachedToCurrentNote = false;
  }

  // are there figured bass attached to the current note?
  if (fThereAreFiguredBassToBeAttachedToCurrentNote) {
    if (gGlobalMxsr2msrOahGroup->getIgnoreFiguredBasses ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceFiguredBasses ()) {
        std::stringstream ss;

        ss <<
          "Ignoring the figured bass elements" <<
          ", line " <<
          elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
    else {
      // create the part figured bass voice if not yet done
      S_msrVoice
        partFiguredBassVoice =
          createPartFiguredBassVoiceIfNotYetDone (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }

    fThereAreFiguredBassToBeAttachedToCurrentNote = false;
  }

 	// Q_NOTE

	// register chord event if any
	registerChordEventIfAny ();

 	// Q_NOTE

	// register tuplets events if any
	registerTupletEventIfAny ();

	// the current note is the new take off candidate
	fPreviousNoteIsATakeOffCandidate = true;

	// set current note staff and voice numbers as previous for the next note
	fPreviousNoteStaffNumber = fCurrentNoteStaffNumber;
	fPreviousNoteVoiceNumber = fCurrentNoteVoiceNumber;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_chord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceChordsBasics ()) {
		std::stringstream ss;

		ss <<
			"--> There is a chord markup" <<
			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<
			", line " << elt->getInputStartLineNumber ();

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteBelongsToAChord = true;
}

void mxsr2msrSkeletonBuilder:: visitEnd (S_chord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_chord" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  // fPreviousTupletNumber = fCurrentTupletNumber;

  // the default value is implementation dependent, so:
  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 1);

  // type

  {
    std::string tupletType = elt->getAttributeValue ("type");

		msrTupletTypeKind tupletTypeKind = msrTupletTypeKind::kTupletTypeNone;

    if      (tupletType == "start") {
      tupletTypeKind = msrTupletTypeKind::kTupletTypeStart;
    }

    else if (tupletType == "continue") {
      tupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;
    }

    else if (tupletType == "stop") {
			tupletTypeKind = msrTupletTypeKind::kTupletTypeStop;
    }

    else {
      std::stringstream ss;

      ss <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceTupletsBasics ()) {
			std::stringstream ss;

			ss <<
				"--> There is a tuplet event " <<
				tupletTypeKind <<
				", fCurrentTupletNumber: " <<
				fCurrentTupletNumber <<
				", fCurrentNoteSequentialNumber: " <<
				fCurrentNoteSequentialNumber <<
				", fPreviousNoteSequentialNumber: " <<
				fPreviousNoteSequentialNumber <<
				", line " << elt->getInputStartLineNumber ();

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		fPendingTupletsInformationTuplesList.push_back (
			std::make_tuple (
				fCurrentNoteStartInputLineNumber,
				fCurrentTupletNumber,
				tupletTypeKind));
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_time_modification& elt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteBelongsToATuplet = true;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_lyric& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_lyric" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.size () == 0) {
      musicxmlWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        std::string (
        	"lyric number is empty, using \"1\" by default")
        		+
        	", line "
        		+
          std::to_string (elt->getInputStartLineNumber ()));

      fCurrentStanzaNumber = "1";
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "--> setting fCurrentStanzaNumber to " <<
          fCurrentStanzaNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // register it as current stanza number, JMI
      // that remains set until another positive value is found,
      // thus allowing a skip syllable to be generated
      // for notes without lyrics
    }
  }

  // name

  {
    fCurrentStanzaName =
      elt->getAttributeValue ("name");

    if (fCurrentStanzaName.size () == 0) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        // lyrics names are not so frequent after all...
        std::stringstream ss;

        ss <<
          "lyric name is empty, using \"" <<
          K_STANZA_NAME_UNKNOWN_ <<
          "\" by default" <<
					", line " << elt->getInputStartLineNumber ();

        musicxmlWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "--> setting fCurrentStanzaName to " <<
          fCurrentStanzaName <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // register it as current stanza name, JMI
      // that remains set another positive value is found,
      // thus allowing a skip syllable to be generated
      // for notes without lyrics
    }
  }
}

void mxsr2msrSkeletonBuilder::visitEnd (S_lyric& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_lyric" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "visitEnd (S_lyric&)" <<
			", fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
      ", line: " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalMxsr2msrOahGroup->getIgnoreLyrics ()) {
    // create current voice if need be
    S_msrVoice
      currentVoice =
        createRegularVoiceInStaffIfNotYetDone (
          elt->getInputStartLineNumber (),
          fCurrentNoteStaffNumber,
          fCurrentNoteVoiceNumber);

    // create stanzaNumber in current voice if need be
    S_msrStanza
      stanza =
        currentVoice->
          createStanzaInVoiceIfNotYetDone (
            elt->getInputStartLineNumber (),
            fCurrentStanzaNumber,
            fCurrentStanzaName);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_harmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmony" <<
      ", harmoniesVoicesCounter: " << fHarmoniesVoicesCounter <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /* JMI ???
    several harmonies can be attached to a given note,
    leading to as many harmonies voices in the current part
  */

  // take harmonies voice into account
  ++fHarmoniesVoicesCounter; // UNUSED JMI

  fThereAreHarmoniesToBeAttachedToCurrentNote = true;
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_figured_bass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figured_bass" <<
      ", figuredBassVoicesCounter: " << fFiguredBassVoicesCounter <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /* JMI
    several figured bass elements can be attached to a given note,
    leading to as many figured bass voices in the current part JMI TRUE???
  */

  // take figured bass voice into account
  ++fFiguredBassVoicesCounter;

  fThereAreFiguredBassToBeAttachedToCurrentNote = true;
}


} // namespace
