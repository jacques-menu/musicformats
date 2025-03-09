/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "mxsr2msrPartGroups.h"


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
	int                   inputLineNumber,
	int                   partGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   identity)
{
  mxsrPartGroup* obj = new
    mxsrPartGroup (
      inputLineNumber,
      partGroupNumber,
      theMsrPartGroup,
      identity);
  assert (obj != nullptr);
  return obj;
}

mxsrPartGroup::mxsrPartGroup (
	int                   inputLineNumber,
	int                   partGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   identity)
{
  fPartGroupNumber = partGroupNumber;

  fPartGroupIdentity = identity;

  fInputLineNumber = inputLineNumber;
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
    fInputLineNumber << ".." << fStopInputLineNumber;

  return ss.str ();
}

void mxsrPartGroup::print (std::ostream& os) const
{
  constexpr int fieldWidth = 14;

	os <<
		"[mxsrPartGroup" <<
    ", fPartGroupNumber" << ": " <<
    fPartGroupNumber <<
		std::endl;

	++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fInputLineNumber" << ": " <<
    fInputLineNumber <<
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


} // namespace
