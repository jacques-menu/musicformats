/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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

#include "msrMeasureConstants.h"

#include "msr2summaryVisitor.h"

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
	int                   theMxsrPartGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   startPosition)
{
  mxsrPartGroup* obj = new
    mxsrPartGroup (
      startInputLineNumber,
      theMxsrPartGroupNumber,
      theMsrPartGroup,
      startPosition);
  assert (obj != nullptr);
  return obj;
}

mxsrPartGroup::mxsrPartGroup (
	int                   startInputLineNumber,
	int                   theMxsrPartGroupNumber,
	const S_msrPartGroup& theMsrPartGroup,
	int                   startPosition)
{
  fStartInputLineNumber   = startInputLineNumber;
  fStopInputLineNumber   = -1;

  fMxsrPartGroupNumber = theMxsrPartGroupNumber;
  fMsrPartGroup = theMsrPartGroup;

  fStartPosition = startPosition;
  fStopPosition = -1;
}

mxsrPartGroup::~mxsrPartGroup ()
{}

std::string mxsrPartGroup::asString () const
{
  std::stringstream ss;

  ss <<
    '\'' <<
    fMxsrPartGroupNumber <<
    "' -=> " <<
    fMsrPartGroup->getPartGroupCombinedName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    ss <<
      ", positions " <<
      fStartPosition << ".." << fStopPosition;
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

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMxsrPartGroupNumber" << ": " <<
    fMxsrPartGroupNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMsrPartGroup" << ": " <<
    fMsrPartGroup <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStartPosition" << ": " <<
    fStartPosition <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStartInputLineNumber" << ": " <<
    fStartInputLineNumber <<
    std::endl <<
    "fStopPosition" << ": " <<
    fStopPosition <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStopInputLineNumber" << ": " <<
    fStopInputLineNumber <<
    std::endl;
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

//_______________________________________________________________________________
mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder ()
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      "msrScore::create()");

  // part groups handling
  fMxsrPartGroupsCounter = 0;

  // parts handling
  fCurrentScorePartPosition = -1; // first MXSR score part will get position 0

  // staff handling
  fCurrentStaffMusicXMLNumber = -1;

  // voice handling
  fCurrentVoiceMusicXMLNumber = -1;

  // measures handling
  fScoreFirstMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;
  fScoreLastMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;

  fScoreMeasuresNumber = 0;
  fPartNumberOfMeasures = 0;

  // lyrics handling
  fCurrentStanzaNumber = msrStanza::K_STANZA_NUMBER_UNKNOWN_; // JMI
  fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_; // JMI

  // harmonies handling
  fHarmoniesVoicesCounter = 0;

  // figured bass handling
  fFiguredBassVoicesCounter = 0;

  // note context
  fCurrentStaffMusicXMLNumber = 0;
  fCurrentVoiceMusicXMLNumber = 0;
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
  }
}

//______________________________________________________________________________
S_mxsrPartGroup mxsr2msrSkeletonBuilder::fetchStartedMxsrPartGroupFromMap (
  int theMxsrPartGroupNumber)
{
  S_mxsrPartGroup result;

  if (fStartedMxsrPartGroupsMap.count (theMxsrPartGroupNumber)) {
    result =
      fStartedMxsrPartGroupsMap [theMxsrPartGroupNumber];
  }

  return result;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayAllMxsrPartGroupsMap (
  int inputLineNumber)
{
  gLog <<
    "fAllMxsrPartGroupsMap:";

  if (fAllMxsrPartGroupsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrPartGroup> thePair : fAllMxsrPartGroupsMap) {
      S_mxsrPartGroup
        theMxsrPartGroup = thePair.second;

      S_msrPartGroup
        theMsrPartGroup =
          theMxsrPartGroup->getMsrPartGroup ();

      gLog <<
        theMxsrPartGroup->asString () <<
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
void mxsr2msrSkeletonBuilder::displayMsrPartsMap (
  int inputLineNumber)
{
  gLog <<
    "fMsrPartsMap:";

  if (fMsrPartsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (std::pair<std::string, S_msrPart> thePair : fMsrPartsMap) {
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
void mxsr2msrSkeletonBuilder::displayStartedMxsrPartGroupsMap (
  int inputLineNumber)
{
  gLog <<
    "fStartedMxsrPartGroupsMap:" <<
    std::endl;

  if (fStartedMxsrPartGroupsMap.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    std::map<int, S_mxsrPartGroup>::const_iterator
      iBegin = fStartedMxsrPartGroupsMap.begin (),
      iEnd   = fStartedMxsrPartGroupsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_mxsrPartGroup
        theMxsrPartGroup = (*i).second;

      S_msrPartGroup
        theMsrPartGroup =
          theMxsrPartGroup->getMsrPartGroup ();

      gLog <<
        theMxsrPartGroup->asString () <<
        std::endl;

        ++gIndenter;

        theMsrPartGroup->
          displayPartGroupElementsList (
            inputLineNumber);

        --gIndenter;

      if (++i == iEnd) break;
      // no std::endl here
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
void mxsr2msrSkeletonBuilder::displayMxsrPartGroupsStack (
  int inputLineNumber)
{
  gLog <<
    "fMxsrPartGroupsStack:";

  if (fMxsrPartGroupsStack.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (S_mxsrPartGroup theMxsrPartGroup : fMxsrPartGroupsStack) {
      gLog <<
        "v " <<
        theMxsrPartGroup->asString () <<
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
void mxsr2msrSkeletonBuilder::displayMxsrPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fMxsrPartGroupsVector:" <<
    std::endl;

  ++gIndenter;

  for (size_t i = 0; i < fMxsrPartGroupsVector.size (); ++i) {
    S_mxsrPartGroup
      theMxsrPartGroup = fMxsrPartGroupsVector [i];

    S_msrPartGroup
      theMsrPartGroup =
        theMxsrPartGroup->getMsrPartGroup ();

    gLog <<
      i << ": " <<
        theMxsrPartGroup->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  } // for

  gLog <<
    "------------------" <<
    std::endl;

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayPositionsOfStartingMxsrPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fPositionsOfStartingMxsrPartGroupsVector:";

  if (fPositionsOfStartingMxsrPartGroupsVector.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (size_t position = 0; position < fPositionsOfStartingMxsrPartGroupsVector.size (); ++position) {
      gLog <<
        position << ": " <<
        std::endl;

      std::list<S_mxsrPartGroup>&
        startingMxsrPartGroupsList =
          fPositionsOfStartingMxsrPartGroupsVector [position];

      if (startingMxsrPartGroupsList.size ()) {
        ++gIndenter;

        std::list<S_mxsrPartGroup>::const_iterator
          iBegin = startingMxsrPartGroupsList.begin (),
          iEnd   = startingMxsrPartGroupsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxsrPartGroup
            theMxsrPartGroup = (*i);

          S_msrPartGroup
            theMsrPartGroup =
              theMxsrPartGroup->getMsrPartGroup ();

          gLog <<
              theMxsrPartGroup->asString () <<
            ", line " << inputLineNumber <<
            std::endl;

          if (++i == iEnd) break;
          // no std::endl here
        } // for

        --gIndenter;
      }

      else {
        gLog <<
          " [EMPTY]" <<
          std::endl;
      }
    }

    --gIndenter;
  }

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayIdentitysOfStartedMxsrPartGroupsVector (
  int inputLineNumber)
{
  gLog <<
    "fPositionsOfStoppingMxsrPartGroupsVector:";

  if (fPositionsOfStoppingMxsrPartGroupsVector.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (size_t position = 0; position < fPositionsOfStoppingMxsrPartGroupsVector.size (); ++position) {
      gLog <<
        position << ": " <<
        std::endl;

      std::list<S_mxsrPartGroup>&
        theList =
          fPositionsOfStoppingMxsrPartGroupsVector [position];

      if (theList.size ()) {
        ++gIndenter;

        std::list<S_mxsrPartGroup>::const_iterator
          iBegin = theList.begin (),
          iEnd   = theList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxsrPartGroup
            theMxsrPartGroup = (*i);

          S_msrPartGroup
            theMsrPartGroup =
              theMxsrPartGroup->getMsrPartGroup ();

          gLog <<
              theMxsrPartGroup->asString () <<
            ", line " << inputLineNumber <<
            std::endl;

          if (++i == iEnd) break;
          // no std::endl here
        } // for

        --gIndenter;
      }

      else {
        gLog <<
          " [EMPTY]" <<
          std::endl;
      }
    }

    --gIndenter;
  }

  gLog <<
    "------------------" <<
    std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayMsrPartsVector (
  int inputLineNumber)
{
  gLog <<
    "fMsrPartsVector:" <<
    std::endl;

  if (fMsrPartsVector.size ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fMsrPartsVector.size (); ++i) {
      S_msrPart
        part = fMsrPartsVector [i];

      gLog <<
        i << ": " <<
        part->getPartCombinedName () <<
        ", upLink to: ";

      S_msrPartGroup
        partUpLinkToPartGroup =
          part->
            getPartUpLinkToPartGroup ();

      if (partUpLinkToPartGroup) {
        gLog <<
          partUpLinkToPartGroup->
            getPartGroupCombinedName ();
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
    ", fCurrentScorePartPosition: " << fCurrentScorePartPosition <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl <<
    ">>> ================================================" <<
    std::endl;

	// parts
  displayMsrPartsMap (
    inputLineNumber);
  gLog << std::endl;

  displayMsrPartsVector (
    inputLineNumber);
  gLog << std::endl;

	// part descrs
  displayAllMxsrPartGroupsMap (
    inputLineNumber);
  gLog << std::endl;

  displayMxsrPartGroupsStack (
    inputLineNumber);
  gLog << std::endl;

  displayStartedMxsrPartGroupsMap (
    inputLineNumber);
  gLog << std::endl;

  displayMxsrPartGroupsVector (
    inputLineNumber);
  gLog << std::endl;

	// part descrs positions
  displayPositionsOfStartingMxsrPartGroupsVector (
    inputLineNumber);
  gLog << std::endl;

  displayIdentitysOfStartedMxsrPartGroupsVector (
    inputLineNumber);
  gLog << std::endl;

  gLog <<
    "<<< ================================================" <<
    std::endl << std::endl;
}

// //________________________________________________________________________
// S_mxsrPartGroup mxsr2msrSkeletonBuilder::fetchMxsrPartGroupsStackTop ()
// {
//   S_mxsrPartGroup result;
//
//   // the current part group is the top of the stack,
//   // i.e. the front for the list used to implement it
//   if (fMxsrPartGroupsStack.size () > 0) {
//    	result =
//     	fMxsrPartGroupsStack.front ();
//   }
//
//   return result;
// }

//________________________________________________________________________
// void mxsr2msrSkeletonBuilder::registerMsrPartGroup (
// 	int                   inputLineNumber,
// 	const S_msrPartGroup& partroup)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceParts ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Registering part " <<
//       part->getPartCombinedName () <<
//       " in the parts data" <<
//       ", partPosition: " << partPosition <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerMsrPart (
  int              inputLineNumber,
  int              scorePartPosition,
  const S_msrPart& theMsrPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Registering MSR part " <<
      theMsrPart->getPartCombinedName () <<
      " in the parts data" <<
      ", scorePartPosition: " << scorePartPosition <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register theMsrPart in the parts vector
  fMsrPartsVector.push_back (theMsrPart);

  // register it in the parts map
  fMsrPartsMap [theMsrPart->getPartID ()] = theMsrPart;

  // create an empty list for MXSR part groups starting at scorePartPosition
  fPositionsOfStartingMxsrPartGroupsVector.push_back (
    std::list<S_mxsrPartGroup> ());

  // create an empty list for MXSR part groups stopping at scorePartPosition
  fPositionsOfStoppingMxsrPartGroupsVector.push_back (
    std::list<S_mxsrPartGroup> ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "AFTER registering MSR part \"" <<
      theMsrPart->getPartCombinedName () <<
      "\" in the parts data" <<
      ", line " << inputLineNumber;

    displayAllCollectedData (
      inputLineNumber,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerMxsrPartGroupAsStarted (
  int                    inputLineNumber,
  const S_mxsrPartGroup& theMxsrPartGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering MXSR part group " <<
      theMxsrPartGroup->getMxsrPartGroupCombinedName () <<
      " as started" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE registering MXSR part group " +
      theMxsrPartGroup->asString () +
      " in the data");
  }
#endif // MF_TRACE_IS_ENABLED

  // register theMxsrPartGroup in the MXSR part groups vector, i.e. by APPEARANCE order
  fMxsrPartGroupsVector.push_back (
    theMxsrPartGroup);

  // register it in fAllMxsrPartGroupsMap under its order as MXSR part group,
  // since its MXSR part group number may reused later by other MXSR part groups
  fAllMxsrPartGroupsMap [fMxsrPartGroupsCounter] =
    theMxsrPartGroup;

  // register it temporarily in fStartedMxsrPartGroupsMap // JMI v0.9.69 BUG ???
  // under its MXSR part group number,
  int theMsrPartGroupNumber =
    theMxsrPartGroup->
      getMsrPartGroup ()->
        getPartGroupNumber ();

  fStartedMxsrPartGroupsMap [theMsrPartGroupNumber] =
    theMxsrPartGroup;

	// register that an MXSR part group has been created
	fAnMxsrPartGroupHasBeenStarted = true;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER registering MXSR part group " +
      theMxsrPartGroup->asString () +
      " in the data");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::insertMxsrPartGroupInStartingListInPositionDecreasingOrder (
	int                         inputLineNumber,
	const S_mxsrPartGroup&      theMxsrPartGroup,
	std::list<S_mxsrPartGroup>& startingMxsrPartGroupsList)
{
  // this list is created in stop position decreasing order,
  // so as to have the outer-most MXSR part group appearing first
  std::list<S_mxsrPartGroup>::iterator
    iBegin = startingMxsrPartGroupsList.begin (),
    iEnd   = startingMxsrPartGroupsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append theMxsrPartGroup to the list
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Appending MXSR part group " <<
          theMxsrPartGroup->asString () <<
          "' to the list of MXSR part groups starting at position " <<
          fCurrentScorePartPosition <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      startingMxsrPartGroupsList.push_back (
        theMxsrPartGroup);
      /*
      std::stringstream ss;
      ss <<
        "MXSR part group " <<
        theMxsrPartGroup->asString () <<
        " could not be inserted in MXSR part groups to be stopped list";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxsrPartGroup
      currentMxsrPartGroup = (*i);

    if (
      theMxsrPartGroup->getStopPosition ()
        >
      currentMxsrPartGroup->getStopPosition ()
    ) {
      // insert theMxsrPartGroup before currentMxsrPartGroup
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Inserting MXSR part group " <<
          currentMxsrPartGroup->asString () <<
          "' in pending list before" <<
          theMxsrPartGroup->asString () <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      startingMxsrPartGroupsList.insert (
        i, theMxsrPartGroup);
      break;
    }

    ++i;
  } // while
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::insertMxsrPartGroupInStoppingListInPositionDecreasingOrder (
  int                         inputLineNumber,
  const S_mxsrPartGroup&      theMxsrPartGroup,
  std::list<S_mxsrPartGroup>& stoppingMxsrPartGroupsList)
{
  // this list is created in start position decreasing order,
  // so as to have the inner-most MXSR part group appearing first
  std::list<S_mxsrPartGroup>::iterator
    iBegin = stoppingMxsrPartGroupsList.begin (),
    iEnd   = stoppingMxsrPartGroupsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append theMxsrPartGroup to the list
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Appending MXSR part group " <<
          theMxsrPartGroup->asString () <<
          "' to the list of MXSR part groups stopping at position " <<
          fCurrentScorePartPosition <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      stoppingMxsrPartGroupsList.push_back (
        theMxsrPartGroup);
      /*
      std::stringstream ss;
      ss <<
        "MXSR part group " <<
        theMxsrPartGroup->asString () <<
        " could not be inserted in part groups to be stopped list";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxsrPartGroup
      currentMxsrPartGroup = (*i);

    if (
      theMxsrPartGroup->getStartPosition ()
        >
      currentMxsrPartGroup->getStartPosition ()
    ) {
      // insert theMxsrPartGroup before currentMxsrPartGroup
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Inserting MXSR part group " <<
          currentMxsrPartGroup->asString () <<
          "' in pending list before" <<
          theMxsrPartGroup->asString () <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      stoppingMxsrPartGroupsList.insert (
        i, theMxsrPartGroup);
      break;
    }

    ++i;
  } // while
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerMxsrPartGroupAsStoppingAtCurrentPosition (
  int                    inputLineNumber,
  const S_mxsrPartGroup& theMxsrPartGroup)
{
  // fetch the list of MXSR part groups stopping at
  // the same position as theMxsrPartGroup
  std::list<S_mxsrPartGroup>&
    stoppingMxsrPartGroupsList =
      fPositionsOfStoppingMxsrPartGroupsVector [
        fCurrentScorePartPosition];

  if (stoppingMxsrPartGroupsList.size () == 0) {
    // first element, insert it directly
    stoppingMxsrPartGroupsList.push_back (
      theMxsrPartGroup);
  }

  else {
    // insert the element at the right place in the stopping list
    insertMxsrPartGroupInStoppingListInPositionDecreasingOrder (
      inputLineNumber,
      theMxsrPartGroup,
      stoppingMxsrPartGroupsList);
  }

  // fetch the list of MXSR part groups starting at
  // the same position as theMxsrPartGroup
  std::list<S_mxsrPartGroup>&
    startingMxsrPartGroupsList =
      fPositionsOfStartingMxsrPartGroupsVector [
        theMxsrPartGroup->
          getStartPosition ()];

  if (startingMxsrPartGroupsList.size () == 0) {
    // first element, insert it directly
    startingMxsrPartGroupsList.push_back (
      theMxsrPartGroup);
  }

  else {
    // insert theMxsrPartGroup at the right place in the starting list
    insertMxsrPartGroupInStartingListInPositionDecreasingOrder (
      inputLineNumber,
      theMxsrPartGroup,
      startingMxsrPartGroupsList);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerMxsrPartGroupAsStopped (
  int                    inputLineNumber,
  const S_mxsrPartGroup& theMxsrPartGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering MXSR part group " <<
      theMxsrPartGroup->getMxsrPartGroupCombinedName () <<
      " as stopped" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register MXSR part group as stopping at current position
  registerMxsrPartGroupAsStoppingAtCurrentPosition (
    inputLineNumber,
    theMxsrPartGroup);

  // forget it in fStartedMxsrPartGroupsMap,
  // it remains in fAllMxsrPartGroupsMap
  fStartedMxsrPartGroupsMap.erase (
    theMxsrPartGroup->
      getMsrPartGroup ()->
        getPartGroupNumber ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER forgetting about MXSR part group " +
        theMxsrPartGroup->asString () +
        " in the data");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleMxsrPartGroupStart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Hangling MXSR part group start with number '" <<
      fCurrentMxsrPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fMxsrPartGroupsCounter;

  // create the part group,
  // with the current part group as part group upLink
  S_msrPartGroup
    startedPartGroup =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentMxsrPartGroupNumber,
        fMxsrPartGroupsCounter,
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

  // create the MXSR part group
  S_mxsrPartGroup
    theMxsrPartGroup =
      mxsrPartGroup::create (
        inputLineNumber,
        fCurrentMxsrPartGroupNumber,
        startedPartGroup,
        fCurrentScorePartPosition) ;

  // register it in the part descrs data
  registerMxsrPartGroupAsStarted (
    inputLineNumber,
    theMxsrPartGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handleMxsrPartGroupStart()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleMxsrPartGroupStop (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Hangling part group stop with number '" <<
      fCurrentMxsrPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is the part group to be stopped started?
  S_mxsrPartGroup
    theMxsrPartGroupToBeStopped =
      fetchStartedMxsrPartGroupFromMap (
        fCurrentMxsrPartGroupNumber);

  if (! theMxsrPartGroupToBeStopped) {
    // no, but we should have found it
    std::stringstream ss;

    ss <<
      "no part group '" << fCurrentMxsrPartGroupNumber <<
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
    // set the stopping position
    theMxsrPartGroupToBeStopped->
      setStopPosition (
        inputLineNumber,
        fCurrentScorePartPosition);

    // register theMxsrPartGroupToBeStopped as stopped
    registerMxsrPartGroupAsStopped (
      inputLineNumber,
      theMxsrPartGroupToBeStopped);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handleMxsrPartGroupStop()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleMxsrPartGroupNestingInItsContainer (
  int                    inputLineNumber,
  const S_mxsrPartGroup& theMxsrPartGroupToBeStopped,
  const S_mxsrPartGroup& theContainingMxsrPartGroup)
{
  // get part groups from descrs
  S_msrPartGroup
    partGroupToBeStopped =
      theMxsrPartGroupToBeStopped->
        getMsrPartGroup (),
    containingPartGroup =
      theContainingMxsrPartGroup->
        getMsrPartGroup ();

  // set currentPartGroup's upLink to theContainingMxsrPartGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting the upLink of part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  partGroupToBeStopped->
    setPartGroupUpLinkToContainingPartGroup (
    	inputLineNumber,
      containingPartGroup);

  // appending currentPartGroup to theContainingMxsrPartGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending sub part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  containingPartGroup->
    appendNestedPartGroupToPartGroup (
      partGroupToBeStopped);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::createAnImplicitOuterMsrPartGroupAndAddItToScore (
	int inputLineNumber)
{
  // an implicit outer-most part group has to be created to contain everything,
  // since there can be parts out of any explicit part group in MusicXML

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fImplicitOuterMostMsrPartGroup == nullptr,
//     "fImplicitOuterMostMsrPartGroup already exists");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  // create an implicit outer-most part group
  fCurrentMxsrPartGroupNumber = 0;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating an implicit outer-most part group with number '" <<
      fCurrentMxsrPartGroupNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fMxsrPartGroupsCounter;

  fImplicitOuterMostMsrPartGroup =
		msrPartGroup::create (
      inputLineNumber,
      fCurrentMxsrPartGroupNumber,
      fMxsrPartGroupsCounter,
      "*** Implicit Outer-Most PartGroup ***", 			// partGroupName
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
      ", fCurrentScorePartPosition: " << fCurrentScorePartPosition <<
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
  fImplicitOuterMostMxsrPartGroup =
    mxsrPartGroup::create (
      inputLineNumber,
      fCurrentMxsrPartGroupNumber,
      fImplicitOuterMostMsrPartGroup,
      fCurrentScorePartPosition);

  // register it in the part groups data
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Adding implicit outer-most MXSR part group for '" <<
      fImplicitOuterMostMsrPartGroup->asString () <<
      "' to the part groups data" <<
      ", fCurrentScorePartPosition: " << fCurrentScorePartPosition <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // push fImplicitOuterMostMxsrPartGroup onto the MXSR part groups stack
	fMxsrPartGroupsStack.push_front (
		fImplicitOuterMostMxsrPartGroup);

  // register it
  registerMxsrPartGroupAsStarted (
    inputLineNumber,
    fImplicitOuterMostMxsrPartGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER creating fImplicitOuterMostMsrPartGroup");
  }
#endif // MF_TRACE_IS_ENABLED
}

// void mxsr2msrSkeletonBuilder::removeImplicitOuterMostMsrPartGroupFromScore ()
// {
//   fMsrScore->
//     removePartGroupFromScore (
//       fImplicitOuterMostMsrPartGroup);
// }

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupsNestingAndScorePartsAllocation (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "handlePartGroupsNestingAndScorePartsAllocation:";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // this is actually a partial subpass of pass 2a,
  // since we run through the contents of <part-list />,
  // stored in the data we've built, a second time

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "BEFORE handlePartGroupsNestingAndScorePartsAllocation");
  }
#endif // MF_TRACE_IS_ENABLED

  // handle each position in turn
  for (int position = 0; position <= fCurrentScorePartPosition; ++position) {

    if (position > 0) {
      // parts actual positions start at 1
      // append part to current part group, i.e. to the top of the stack
      S_msrPart
        part =
          fMsrPartsVector [position];

      // fetch the MXSR part group stack top
			if (fMxsrPartGroupsStack.size () == 0) {
        std::stringstream ss;

        ss <<
          "Cannot append part " <<
          part->getPartCombinedName () <<
          " to any part group " <<
          " at position " << position <<
          ", since the part groups descrs stack is empty"<<
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
          fMxsrPartGroupsStack.front ()->
            getMsrPartGroup ();

      currentPartGroup->
        appendPartToPartGroup (
          part);
    }

    // handle the part groups descrs stopping at position position
    if (fPositionsOfStoppingMxsrPartGroupsVector.size ()) {
      std::list<S_mxsrPartGroup>&
        stoppingMxsrPartGroupsList =
          fPositionsOfStoppingMxsrPartGroupsVector [position];

      if (stoppingMxsrPartGroupsList.size ()) {
        ++gIndenter;

        for (S_mxsrPartGroup theMxsrPartGroup : stoppingMxsrPartGroupsList) {
          int stopInputLineNumber =
            theMxsrPartGroup->getStopInputLineNumber ();

          S_msrPartGroup
            theMsrPartGroup =
              theMxsrPartGroup->getMsrPartGroup ();

          // fetch the MXSR part group stack top
					if (fMxsrPartGroupsStack.size () == 0) {
            std::stringstream ss;

            ss <<
              "Cannot 'stop' MXSR part group " <<
              theMxsrPartGroup->getMxsrPartGroupCombinedName () <<
              " at position " << position <<
              ", since the stack is empty"<<
							", line " << inputLineNumber;

            musicxmlError (
              gServiceRunData->getInputSourceName (),
              stopInputLineNumber,
              __FILE__, __LINE__,
              ss.str ());
          }

          if (fMxsrPartGroupsStack.front () == theMxsrPartGroup) {
            // pop theMxsrPartGroupToBeStopped from the stack

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTracePartGroups ()) {
              std::stringstream ss;

              ss <<
                "Popping part group " <<
                theMxsrPartGroup->
                  getMxsrPartGroupCombinedName () <<
                "' from the stack" <<
                ", line " << stopInputLineNumber <<
                std::endl;

              gWaeHandler->waeTrace (
                __FILE__, __LINE__,
                ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            fMxsrPartGroupsStack.pop_front ();

            // the implicit outer-most part group isn't contained in any other
            if (theMxsrPartGroup != fImplicitOuterMostMxsrPartGroup) {
              // fetch new current part group JMI v0.9.69
							if (fMxsrPartGroupsStack.size () == 0) {
								displayAllCollectedData (
									stopInputLineNumber,
									"the implicit outer-most part group isn't contained in any other");

                std::stringstream ss;

                ss <<
                  "there is no MXSR part group in the stack to contain MXSR part group " <<
                  theMxsrPartGroup->asString () <<
                  fCurrentPartID << "\"" ;

//                 mxsr2msrInternalError (
                mxsr2msrInternalWarning (
                  gServiceRunData->getInputSourceName (),
                  stopInputLineNumber,
//                   __FILE__, __LINE__,
                  ss.str ());
              }

// 							S_mxsrPartGroup
// 								newMxsrPartGroupStackTop =
// 									fMxsrPartGroupsStack.front ();
//
//               // theMxsrPartGroup is nested in newMxsrPartGroupStackTop,
//               // do the nesting
//               handleMxsrPartGroupNestingInItsContainer (
//                 stopInputLineNumber,
//                 theMxsrPartGroup,
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
              gTab << theMxsrPartGroup->asString () <<
              std::endl <<
              "and" <<
              std::endl <<
              gTab << fMxsrPartGroupsStack.front ()->asString () <<
              std::endl;

/* JMI
            // fetch the positions in the intersection
            int
              startOne =
                theMxsrPartGroup->getStartPosition (),
              startTwo =
                fMxsrPartGroupsStack.front ()->getStartPosition (),
              stopOne =
                theMxsrPartGroup->getStopPosition (),
              stopTwo =
                fMxsrPartGroupsStack.front ()->getStopPosition ();

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
              "(positions range is " <<
              firstCommonPosision << ".." << lastCommonPosision <<
              ")" <<
              std::endl;

            for (int m = firstCommonPosision; m <= lastCommonPosision; ++m) {
              S_msrPart
                part =
                  fMsrPartsVector [m];

              ss <<
                gTab <<
                part->getPartCombinedName () <<
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

    // handle the part groups descrs starting at position position
    if (fPositionsOfStartingMxsrPartGroupsVector.size ()) {
      std::list<S_mxsrPartGroup>&
        startingMxsrPartGroupsList =
          fPositionsOfStartingMxsrPartGroupsVector [position];

      if (startingMxsrPartGroupsList.size ()) {
        ++gIndenter;

        std::list<S_mxsrPartGroup>::const_iterator
          iBegin = startingMxsrPartGroupsList.begin (),
          iEnd   = startingMxsrPartGroupsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxsrPartGroup
            theMxsrPartGroup = (*i);

          S_msrPartGroup
            theMsrPartGroup =
              theMxsrPartGroup->getMsrPartGroup ();

          // make it the new current part group
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTracePartGroups ()) {
            std::stringstream ss;

            ss <<
              "Pushing MXSR part group '" <<
              theMxsrPartGroup->asString () <<
              "' onto the part groups stack" <<
              ", line " << inputLineNumber <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fMxsrPartGroupsStack.push_front (
            theMxsrPartGroup);

          if (++i == iEnd) break;
          // no std::endl here
        } // for

        --gIndenter;
      }
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
      std::stringstream ss;

      ss <<
        "AT position " << position;

      displayAllCollectedData (
        inputLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "AFTER handlePartGroupsNestingAndScorePartsAllocation");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
S_msrStaff mxsr2msrSkeletonBuilder::createStaffInCurrentPartIfNotYetDone (
  int inputLineNumber,
  int staffNumber)
{
  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentMsrPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) {
    // no, add it to fCurrentMsrPart
    staff =
      fCurrentMsrPart->
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
  // the voice number is relative to a part,

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
          fCurrentMeasureNumber);

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
      "Analysing the score partwise";

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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_partwise" <<
      ", line " << inputLineNumber;

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

/* JMI ??? THROW AWAY ???
  // fetch the identification from the credits if any
  fMsrScore ->
    fetchIdentificationFromCreditsIfAny (
      inputLineNumber);
      */
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_work_number& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_number" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationWorkNumber (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_work_title& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_title" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string workCreditTypeTitle = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationWorkCreditTypeTitle (
      inputLineNumber,
      workCreditTypeTitle);
}

void mxsr2msrSkeletonBuilder::visitStart (S_opus& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_opus" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string opus = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationOpus (
      inputLineNumber,
      opus);
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_number& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_number" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationMovementNumber (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_title& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_title" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      movementTitle);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_identification" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitEnd (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_identification" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_creator& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_creator" <<
      ", line " << inputLineNumber;

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
        inputLineNumber,
        creatorValue);
  }

  else if (creatorType == "arranger") {
    fMsrScore->getIdentification () ->
      appendArranger (
        inputLineNumber,
        creatorValue);
  }

  else if (creatorType == "lyricist") {
    fMsrScore->getIdentification () ->
      appendLyricist (
        inputLineNumber,
        creatorValue);

    // should we use lyricists as poets?
    if (gGlobalMxsr2msrOahGroup->getUseLyricistsAsPoets ()) {
      fMsrScore->getIdentification () ->
        appendPoet (
          inputLineNumber,
          elt->getValue ());
    }
  }

  else if (creatorType == "poet") {
    fMsrScore->getIdentification () ->
      appendPoet (
        inputLineNumber,
        elt->getValue ());
  }

  else if (creatorType == "translator") {
    fMsrScore->getIdentification () ->
      appendTranslator (
        inputLineNumber,
        creatorValue);
  }

  else if (creatorType == "artist") {
    fMsrScore->getIdentification () ->
      appendArtist (
        inputLineNumber,
        creatorValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "creator type \"" << creatorType <<
      "\" is unknown" <<
      ", line " << inputLineNumber;

// JMI    musicxmlError (
    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
//      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_rights& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rights" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      inputLineNumber,
      rightsValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_source& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_source" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      rightsValue);
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_relation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_relation" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
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
      "*** Analysing S_encoding ***" <<
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_encoding" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_date& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_date" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoder& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoder" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_software& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_software" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      std::string ("<software /> contains 'Cubase'") + ", line " + std::to_string (inputLineNumber));

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
      inputLineNumber,
      softwareValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_description& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_description" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_supports& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_supports" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      miscellaneousFielValue);
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_miscellaneous" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_miscellaneous_field& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous_field" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
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
  in sequence visually. Non-positional formatting attributes
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
    %position;
    %halign;
    %valign-image;
    %optional-unique-id;
>
*/

void mxsr2msrSkeletonBuilder::visitStart (S_credit& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      creditPageNumber);

  // append it to the score
  fMsrScore->
    appendCreditToScore (fCurrentCredit);
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_type& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_type" <<
      ", line " << inputLineNumber;

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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_symbol" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_image& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_image" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_words& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_words" <<
      ", line " << inputLineNumber;

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

  // position
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
        inputLineNumber,
        creditWordsFontWeightString);

  // font style
  std::string creditWordsFontStyleString =
    elt->getAttributeValue ("font-style");

  msrFontStyleKind
    creditWordsFontStyleKind =
      msrFontStyleKindFromString (
        inputLineNumber,
        creditWordsFontStyleString);

  // justify
  std::string creditWordsJustifyString =
    elt->getAttributeValue ("justify");

  msrJustifyKind
    creditWordsJustifyKind =
      msrJustifyKindFromString (
        inputLineNumber,
        creditWordsJustifyString);

  // halign
  std::string creditWordsHAlignString =
    elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    creditWordsHorizontalAlignment =
      msrHorizontalAlignmentKindFromString (
        inputLineNumber,
        creditWordsHAlignString);

  // valign
  std::string creditWordsVAlignString =
    elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    creditWordsVerticalAlignment =
      msrVerticalAlignmentKindFromString (
        inputLineNumber,
        creditWordsVAlignString);

  // XMLLang
  std::string creditWordsXMLLangString =
    elt->getAttributeValue ("xml:lang");

  msrXMLLangKind
    creditWordsXMLLang =
      msrXMLLangKindFromString (
        inputLineNumber,
        creditWordsXMLLangString);

  // create the credit words
  S_msrCreditWords
    creditWords =
      msrCreditWords::create (
        inputLineNumber,
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_credit" <<
      ", line " << inputLineNumber;

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
      "Analysing part list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_list& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_list" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // set implicit outer-most MXSR part group end position
  if (fImplicitOuterMostMxsrPartGroup) {
		fImplicitOuterMostMxsrPartGroup->
			setStopPosition (
				inputLineNumber, // JMI ??? v0.9.69
				INT_MAX);

		// register implicit outer-most MXSR part group as stopped
		registerMxsrPartGroupAsStopped (
			inputLineNumber,
			fImplicitOuterMostMxsrPartGroup);
	}

  // do the job...
  handlePartGroupsNestingAndScorePartsAllocation (
    inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (true || gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      inputLineNumber,
      "Part groups data has been gathered for score skeleton");
  }
#endif // MF_TRACE_IS_ENABLED

  if (gTraceOahGroup->getTracePartGroups ()) {
    gLog <<
      std::endl <<
      "The implicit outer-most part group contains:" <<
      std::endl;

    ++gIndenter;

		if (fImplicitOuterMostMsrPartGroup) {
			fImplicitOuterMostMsrPartGroup->
				displayPartGroupElementsList (
					inputLineNumber);
		}

#ifdef MF_TRACE_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
      gLog << std::endl; // JMI v0.9.67
    }
#endif // MF_TRACE_IS_ENABLED

    --gIndenter;
  }
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

  fCurrentMxsrPartGroupNumber =
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_name_display" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_text" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
      __FILE__, __LINE__,
      std::string ("<display-text /> is out of context") + ", line " + std::to_string (inputLineNumber));
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_symbol" <<
      ", line " << inputLineNumber;

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
				", line " << inputLineNumber;

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_barline& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_barline" <<
      ", line " << inputLineNumber;

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
      ", line " << inputLineNumber;

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
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
      "Handling MXSR part group '" <<
      fCurrentMxsrPartGroupNumber <<
      "', type: \"" <<
      msrPartGroupTypeKindAsString (
        fCurrentPartGroupTypeKind) << "\""  <<
      ", fCurrentScorePartPosition: " << fCurrentScorePartPosition <<
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
      handleMxsrPartGroupStart (
        elt->getInputStartLineNumber ());
      break;

    case msrPartGroupTypeKind::kPartGroupTypeStop:
      // handle the part group stop
      handleMxsrPartGroupStop (
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_part" <<
      ", line " << inputLineNumber;

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
      "Found part name \"" << fCurrentScorePartID << "\"" <<
      " in part list" <<
      ", line " << inputLineNumber;

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
      ", line " << inputLineNumber;

    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());
  }

  // initialize fields
  fCurrentPartName = "";
  fCurrentPartNameDisplayText = "";

  fCurrentPartAbbreviation = "";
  fCurrentPartAbbreviationDisplayText = "";

  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";

	// create an implicit outer-most part group if neeed
	if (! fAnMxsrPartGroupHasBeenStarted) {
		createAnImplicitOuterMsrPartGroupAndAddItToScore (
			inputLineNumber);
	}
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_name& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_name" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_abbreviation" <<
      ", line " << inputLineNumber;

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
      inputLineNumber,
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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_part" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentScorePartPosition;

  std::string scorePartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
  std::stringstream ss;

  ss <<
    "--------------------------------------------" <<
    std::endl <<
    "Handling score part with ID \"" << scorePartID << "\"" <<
    ", fCurrentScorePartPosition: " << fCurrentScorePartPosition <<
    ", line " << inputLineNumber <<
    std::endl;

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
      "--------------------------------------------" <<
      std::endl <<
      "Creating MSR part \"" << scorePartID << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrPart
    theMsrPart =
      msrPart::create (
        inputLineNumber,
        scorePartID,
        0); // partUpLinkToPartGroup will be set later

  // populate it
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
  registerMsrPart (
    inputLineNumber,
    fCurrentScorePartPosition,
    theMsrPart);

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceParts ()
      ||
    gEarlyOptions.getEarlyTracePasses ()
  ) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      " partID \"" << fCurrentPartID << "\"" <<
//       "partName \"" << fCurrentPartName << "\"" << JMI from fPartGroupElementsList ??? v0.9.67
      ", line " << inputLineNumber <<
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
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  // is this part already known?
  if (fMsrPartsMap.count (fCurrentPartID)) {
    fCurrentMsrPart = // used thoughoutfRepeatHasBeenCreatedForCurrentPart
      fMsrPartsMap [fCurrentPartID];
  }

  else {
    std::stringstream ss;

    ss <<
      "part \"" <<
      fCurrentPartID <<
      "\" is not known in the <part-list />" <<
      ", line " << inputLineNumber;

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "--------------------------------------------" <<
      std::endl <<
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

  if (fCurrentMsrPart) {
    serviceRunData->
      setCurrentPartIDAndName (
        fCurrentMsrPart->getPartIDAndName ());
  }

  // measures
  fPartNumberOfMeasures = 0;

  // staves and voices
  fCurrentStaffMusicXMLNumber = 1; // default if there are no <staff> element
  fCurrentVoiceMusicXMLNumber = 1; // default if there are no <voice> element
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part" <<
      ", line " << inputLineNumber;

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
      std::endl <<
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
        "part " << fCurrentMsrPart->getPartCombinedName () <<
        " has " << fPartNumberOfMeasures <<
        " measures while the other ones have " << fScoreMeasuresNumber<<
				", line " << inputLineNumber;

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
      __FILE__, __LINE__,
        ss.str ());
    }
  }

  // set current part's number of measures
  fCurrentMsrPart->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

/* JMI
  // are there more staves in <staves/> that specified with <stave/>?
  fCurrentMsrPart->
    addAVoiceToStavesThatHaveNone (
      inputLineNumber);
*/

  // assign sequential numbers to the voices in the part
  fCurrentMsrPart->
    assignSequentialNumbersToRegularVoicesInPart (
      inputLineNumber);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_staves& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int stavesNumber = int(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    switch (stavesNumber) {
      case 0:
        ss <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;

      case 1:
        ss <<
          "There is 1 staff";
        break;

      default:
        ss <<
          "There are " << stavesNumber << " staves";
    } // switch

    ss <<
      " in part " << fCurrentMsrPart->getPartCombinedName();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (stavesNumber > 1) {
    // add stavesNumber staves to current part
    int n = 1;

    while (n <= stavesNumber) {
    /* JMI
      fCurrentMsrPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaffKind::kStaffKindRegular,
          n);
          */
      S_msrStaff
        dummyStaff = // JMI
          createStaffInCurrentPartIfNotYetDone (
            inputLineNumber,
            n);

      ++n;
    } // while
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_staff& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffMusicXMLNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentStaffMusicXMLNumber == K_STAFF_NUMBER_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "staff number " << fCurrentStaffMusicXMLNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssert (
        __FILE__, __LINE__,
				false,
				ss.str ());
  }

  if (fOnGoingNote) { // JMI
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      "voice \"" << "elt->getVoiceName ()" << "\"" <<
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceMusicXMLNumber = int(*elt);

  if (fOnGoingNote) { // JMI
    // regular voice indication in note/rest, fine
  }
  else {
    // JMI ???
  }

  // don't attempt to create the voice now,
  // it's staff number if any comes later!
  // do it upton visitEnd (S_note& )
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure" <<
      ", line " << inputLineNumber;

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
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gEarlyOptions.getEarlyTracePasses ()
  ) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== " <<
//       "part \"" << fCurrentPartName << "\"" << JMI v0.9.67
      " (partID \"" << fCurrentPartID << "\")" <<
      ", measure \"" << fCurrentMeasureNumber << "\"" <<
      ", line " << inputLineNumber <<
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

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_print& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_print" <<
      ", line " << inputLineNumber;

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

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffMusicXMLNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceMusicXMLNumber = 1;

  // lyrics
  fCurrentStanzaNumber = msrStanza::K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_;

  fOnGoingNote = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_note& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_note" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // should the staff be created?
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        fCurrentStaffMusicXMLNumber);

  // should the voice be created?
  S_msrVoice
    noteVoice =
      createRegularVoiceInStaffIfNotYetDone (
        inputLineNumber,
        fCurrentStaffMusicXMLNumber,
        fCurrentVoiceMusicXMLNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      "--> S_note, fCurrentStaffMusicXMLNumber: " <<
      fCurrentStaffMusicXMLNumber <<
      std::endl <<
      "--> S_note, current staff name: " <<
      staff->getStaffName() <<
      std::endl <<
      "--> S_note, fCurrentVoiceMusicXMLNumber: " <<
      fCurrentVoiceMusicXMLNumber <<
      std::endl <<
      "--> S_note, current noteVoice name: " <<
      noteVoice->getVoiceName();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // are there harmonies attached to the current note?
  if (fThereAreHarmoniesToBeAttachedToCurrentNote) {
    if (gGlobalMxsr2msrOahGroup->getIgnoreHarmonies ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        std::stringstream ss;

        ss <<
          "Ignoring the harmonies" <<
          ", line " <<
          inputLineNumber <<
          std::endl;

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
            inputLineNumber,
            fCurrentMsrPart);
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
          inputLineNumber <<
          std::endl;

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
            inputLineNumber,
            fCurrentMsrPart);
    }

    fThereAreFiguredBassToBeAttachedToCurrentNote = false;
  }

  fOnGoingNote = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_lyric& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_lyric" <<
      ", line " << inputLineNumber;

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
        inputLineNumber,
        std::string ("lyric number is empty, using \"1\" by default") + ", line " + std::to_string (inputLineNumber));

      fCurrentStanzaNumber = "1";
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "--> setting fCurrentStanzaNumber to " <<
          fCurrentStanzaNumber <<
          ", line " << inputLineNumber <<
          std::endl;

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
          msrStanza::K_STANZA_NAME_UNKNOWN_ <<
          "\" by default" <<
					", line " << inputLineNumber;

        musicxmlWarning (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "--> setting fCurrentStanzaName to " <<
          fCurrentStanzaName <<
          ", line " << inputLineNumber <<
          std::endl;

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
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_lyric" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "visitEnd (S_lyric& )" <<
      ", line: " << inputLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    ss <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      const int fieldWidth = 28;

      ss << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
        std::endl;

      --gIndenter;
    }

    --gIndenter;

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
          inputLineNumber,
          fCurrentStaffMusicXMLNumber,
          fCurrentVoiceMusicXMLNumber);

    // create stanzaNumber in current voice if need be
    S_msrStanza
      stanza =
        currentVoice->
          createStanzaInVoiceIfNotYetDone (
            inputLineNumber,
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