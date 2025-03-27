/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "mfAssert.h"
#include "mfConstants.h"

#include "mfStringsHandling.h"

#include "msrBarLines.h"

#include "mxsr2msrOah.h"

#include "oahEarlyOptions.h"

#include "mxsr2msrPartGroups.h"

#include "mxsr2msrSkeletonBuilder.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder (
	mxsrEventsCollection&
		theResultingEventsCollection)
	: fResultingEventsCollection (
			theResultingEventsCollection),
		fCurrentMeasureInputLineNumber (0),
		fPreviousMeasureInputLineNumber (0),
		fCurrentNoteSequentialNumber (0),
		fPreviousNoteSequentialNumber (0)
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      "msrScore::create()");

  // parts handling
  fCurrentPartGroupSequentialNumber = 0;

  // staff handling
  fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // voice handling
  fCurrentNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
  fPreviousNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;

  // measures handling
  fScoreFirstMeasureBareNumber = K_MEASURE_NUMBER_UNKNOWN_;
  fScoreLastMeasureBareNumber = K_MEASURE_NUMBER_UNKNOWN_;

  fScoreMeasuresNumber = 0;
  fPartNumberOfMeasures = 0;

	fCurrentNoteInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;

	fPreviousNoteInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_;

	// grace notes handling
	fCurrentNoteIsAGraceNote = false;
	fPreviousNoteIsAGraceNote = false;

	// cue notes handling
	fCurrentNoteIsACueNote = false;
	fPreviousNoteIsACueNote = false;

	// staff changes handling
	fPreviousNoteIsATakeOffCandidate = false;

  // chords handling
  fCurrentNoteBelongsToAChord = false;
  fPreviousNoteBelongsToAChord = false;

//   fOnGoingChord = false;

  // tuplets handling
  fCurrentTupletNumber = -1;
  // fPreviousTupletNumber = -1;

  fCurrentNoteBelongsToATuplet = false;
  fPreviousNoteBelongsToATuplet = false;

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

		// sort the fResultingEventsCollection MXSR events lists
		fResultingEventsCollection.sortTheMxsrEventsLists ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getDisplayMxsrEvents ()) {
    gLog <<
      std::endl <<
      "The MSR skeleton builder collected the following MXSR events:" <<
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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fPartGroupsMap, " <<
    mfSingularOrPlural (
    	fPartGroupsMap.size (), "element", "elements") <<
    ':';

  if (! fPartGroupsMap.empty ()) {
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

#ifdef MF_USE_WRAPPED_TYPES
	          inputLineNumber.getBareValue ());
#else
	          inputLineNumber);
#endif // MF_USE_WRAPPED_TYPES

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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fPartsMap," <<
    mfSingularOrPlural (
    	fPartsMap.size (), "element", "elements") <<
    ':';

  if (! fPartsMap.empty ()) {
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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fStartedPartGroupsMap, " <<
    mfSingularOrPlural (
    	fStartedPartGroupsMap.size (), "element", "elements") <<
    ':';

  if (! fStartedPartGroupsMap.empty ()) {
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

#ifdef MF_USE_WRAPPED_TYPES
	          inputLineNumber.getBareValue ());
#else
	          inputLineNumber);
#endif // MF_USE_WRAPPED_TYPES

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
  const mfInputLineNumber& inputLineNumber)
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
  const mfInputLineNumber& inputLineNumber)
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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fStartedPartGroupsListsVector, " <<
    mfSingularOrPlural (
    	fStartedPartGroupsListsVector.size (), "element", "elements") <<
    ':' <<
    std::endl;

	++gIndenter;

	for (
		size_t partGroupSequentialNumber = 0;
		partGroupSequentialNumber< fStartedPartGroupsListsVector.size ();
		++partGroupSequentialNumber
	) {
		S_mxsrPartGroupsList
			startedMxsrPartGroupsList =
				fStartedPartGroupsListsVector [partGroupSequentialNumber];

		gLog <<
			partGroupSequentialNumber<< ':' <<
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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fStoppedPartGroupsListsVector, " <<
    mfSingularOrPlural (
    	fStoppedPartGroupsListsVector.size (), "element", "elements") <<
    ':' <<

	++gIndenter;

	for (
		size_t partGroupSequentialNumber = 0;
		partGroupSequentialNumber< fStoppedPartGroupsListsVector.size ();
		++partGroupSequentialNumber
	) {
		S_mxsrPartGroupsList
			StoppedPartGroupssList =
				fStoppedPartGroupsListsVector [partGroupSequentialNumber];

		gLog <<
			partGroupSequentialNumber<< ':' <<
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
  const mfInputLineNumber& inputLineNumber)
{
  gLog <<
    "fPartsVector, " <<
    mfSingularOrPlural (
    	fPartsVector.size (), "element", "elements") <<
    ':';

  if (! fPartsVector.empty ()) {
  	gLog << std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fPartsVector.size (); ++i) {
      S_msrPart
        part = fPartsVector [i];

      gLog <<
        i << ": " <<
        part->fetchPartNameForTrace () <<
        ", upLink to: ";

      S_msrPartGroup
        partUpLinkToPartGroup =
          part->
            getPartUpLinkToPartGroup ();

      if (partUpLinkToPartGroup) {
        gLog <<
          partUpLinkToPartGroup->
            fetchPartGroupInformationForTrace ();
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
	const mfInputLineNumber& inputLineNumber,
	const std::string&       context)
{
  gLog <<
    std::endl <<
    "=======> displayAllCollectedData(), " <<
    context <<
    ", fCurrentPartGroupSequentialNumber: " <<
    fCurrentPartGroupSequentialNumber<<
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
	const mfInputLineNumber& inputLineNumber,
	const S_msrPart&         thePart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Registering MSR part " <<
      thePart->fetchPartNameForTrace () <<
      " in the parts data" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register thePart in the parts vector
  fPartsVector.push_back (thePart);

  // register it in the parts map
  fPartsMap [thePart->getPartMusicXMLID ()] = thePart;

	// append it to current part group
	S_msrPartGroup
		currentMsrPartGroup =
			fPartGroupsStack.top ()->
				getMsrPartGroup ();

	currentMsrPartGroup->
		appendPartToPartGroup (
			thePart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "AFTER registering MSR part " <<
      thePart->fetchPartNameForTrace () <<
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
	const mfInputLineNumber& inputLineNumber,
	const S_mxsrPartGroup&    partGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering part group start for " <<
      partGroup->fetchMsrPartGroupNameForTrace () <<
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
  fPartGroupsMap [fCurrentPartGroupSequentialNumber] =
    partGroup;

  // register in fStartedPartGroupsMap // JMI 0.9.69 BUG ???
  // under its part group number,
  int theMsrPartGroupNumber =
    partGroup->
      getMsrPartGroup ()->
        getPartGroupNumber ();

  fStartedPartGroupsMap [theMsrPartGroupNumber] =
    partGroup;

  // create an empty mxsrPartGroupsList for part groups started at scorePartIdentity // JMI 0.9.69
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
  	std::stringstream ss;

  	ss <<
      "Appending an empty mxsrPartGroupsList to fStartedPartGroupsListsVector" <<
      ", fCurrentPartGroupSequentialNumber: " << fCurrentPartGroupSequentialNumber;

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
	const mfInputLineNumber& inputLineNumber,
	const S_mxsrPartGroup&    partGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Registering part group stop for " <<
      partGroup->fetchMsrPartGroupNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	if (fPartGroupsStack.empty ()) {
		std::stringstream ss;

		ss <<
			"Cannot stop part group " <<
			partGroup->asString () <<
			", since the part groups groups stack is empty"<<
			", line " << inputLineNumber;

		musicxmlError (
			gServiceRunData->getInputSourceName (),

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

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
					fetchMsrPartGroupNameForTrace () <<
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
		// the part group stops are out of oder JMI 0.9.69

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTracePartGroups ()) {
			std::stringstream ss;

			ss <<
				"The part group stop for " <<
				partGroup->asString () <<
				" is out of order, it does not match the part group stack top " <<
				partGroupStackTop->
					fetchMsrPartGroupNameForTrace () <<
				", line " << inputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
}
#endif // MF_TRACE_IS_ENABLED
	}

// 	if (fPartGroupsStack.empty ()) { // JMI 0.9.71
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
//         fCurrentPartGroupSequentialNumber];
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
//       ", fCurrentPartGroupSequentialNumber: " << fCurrentPartGroupSequentialNumber;
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
  const mfInputLineNumber& inputLineNumber)
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

  ++fCurrentPartGroupSequentialNumber;

  // create the part group,
  // with the current part group as part group upLink
  S_msrPartGroup
    startedPartGroup =
      msrPartGroup::create (

#ifdef MF_USE_WRAPPED_TYPES
				inputLineNumber.getBareValue (),
#else
				inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

        fCurrentPartGroupNumber,
        fCurrentPartGroupSequentialNumber,
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

#ifdef MF_USE_WRAPPED_TYPES
				inputLineNumber.getBareValue (),
#else
				inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

        fCurrentPartGroupNumber,
        startedPartGroup,
        fCurrentPartGroupSequentialNumber);

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
  const mfInputLineNumber& inputLineNumber)
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

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

  //    __FILE__, __LINE__,
      ss.str ());
  }

  else {
    // register partGroupToBeStopped as stopped
    registerPartGroupStop (

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

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
	const mfInputLineNumber& inputLineNumber,
	const S_mxsrPartGroup&   partGroupToBeStopped,
	const S_mxsrPartGroup&   containingPartGroup)
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
        fetchPartGroupInformationForTrace () <<
      "' into " <<
      theMsrContainingPartGroup->
        fetchPartGroupInformationForTrace () <<
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
        fetchPartGroupInformationForTrace () <<
      "' to " <<
      theMsrContainingPartGroup->
        fetchPartGroupInformationForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  theMsrPartGroupToBeStopped->
    setPartGroupUpLinkToContainingPartGroup (

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

      theMsrContainingPartGroup);

  // append currentPartGroup to containingPartGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending sub part group " <<
      theMsrPartGroupToBeStopped->
        fetchPartGroupInformationForTrace () <<
      "' to " <<
      theMsrContainingPartGroup->
        fetchPartGroupInformationForTrace () <<
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
	const mfInputLineNumber& inputLineNumber)
{
  // an implicit outer-most part group has to be created to contain everything,
  // since there can be parts out of any explicit part group in MusicXML

	constexpr int K_MXSR_IMPLICIT_PART_GROUP_NUMBER = -1;

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

  ++fCurrentPartGroupSequentialNumber;

	// create fImplicitOuterMostMsrPartGroup
  fImplicitOuterMostMsrPartGroup =
		msrPartGroup::create (

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

      fCurrentPartGroupNumber,
      fCurrentPartGroupSequentialNumber,
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
      ", fCurrentPartGroupSequentialNumber: " <<
      fCurrentPartGroupSequentialNumber<<
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

#ifdef MF_USE_WRAPPED_TYPES
			inputLineNumber.getBareValue (),
#else
			inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

      fCurrentPartGroupNumber,
      fImplicitOuterMostMsrPartGroup,
      fCurrentPartGroupSequentialNumber);

  // register it in the part groups data
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Adding implicit MXSR outer-most part group for '" <<
      fImplicitOuterMostPartGroup->asString () <<
      "' to the part groups data" <<
      ", fCurrentPartGroupSequentialNumber: " <<
      fCurrentPartGroupSequentialNumber<<
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
void mxsr2msrSkeletonBuilder::sortfStartedPartGroupsListsVector (
	const mfInputLineNumber& inputLineNumber)
{
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
				"sortfStartedPartGroupsListsVector()",
				'>',
				ss);

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		thePartGroupList->sortByDecreasingIdentity();
	} // for
}

void mxsr2msrSkeletonBuilder::sortStoppedPartGroupsListsVector (
	const mfInputLineNumber& inputLineNumber)
{
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
				"sortStoppedPartGroupsListsVector()",
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
}

void mxsr2msrSkeletonBuilder::handlePartGroupsStartAtIdentity (
	const mfInputLineNumber& inputLineNumber,
	int                      partGroupSequentialNumber)
{
	if (! fStartedPartGroupsListsVector.empty ()) {
		S_mxsrPartGroupsList
			startedMxsrPartGroupsList =
				fStartedPartGroupsListsVector [partGroupSequentialNumber];

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
}

void mxsr2msrSkeletonBuilder::handleThePartGroupsStoppedAtIdentity (
	const mfInputLineNumber& inputLineNumber,
	int                      partGroupSequentialNumber)
{
	if (! fStoppedPartGroupsListsVector.empty ()) {
		S_mxsrPartGroupsList
			stoppedPartGroupsList =
				fStoppedPartGroupsListsVector [partGroupSequentialNumber];

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
				if (fPartGroupsStack.empty ()) {
					std::stringstream ss;

					ss <<
						"Cannot 'stop' part group " <<
						partGroup->fetchMsrPartGroupNameForTrace () <<
						" at partGroupSequentialNumber" << partGroupSequentialNumber<<
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
								fetchMsrPartGroupNameForTrace () <<
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
						// fetch new current part group JMI 0.9.69
						if (fPartGroupsStack.empty ()) {
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
								fCurrentPartID;

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
							partGroup->getPartGroupSequentialNumber (),
						startTwo =
							fPartGroupsStack.front ()->getPartGroupSequentialNumber (),
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
							part->fetchPartNameForTrace () <<
							", line " << part->getInputLineNumber () <<
							std::endl;
					} // for
*/

#ifdef MF_TRACE_IS_ENABLED
					if (gTraceOahGroup->getTracePartGroups ()) {
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
#endif // MF_TRACE_IS_ENABLED
				}
			} // for

			--gIndenter;
		}
	}

	// handle the part groups started at partGroupSequentialNumber // JMI 0.9.69
	handlePartGroupsStartAtIdentity (
		inputLineNumber,
		partGroupSequentialNumber);

// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTracePartGroups ()) {
//       std::stringstream ss;
//
//       ss <<
//         "AT partGroupSequentialNumber" << partGroupSequentialNumber;
//
//       displayAllCollectedData (
//         inputLineNumber,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
  const mfInputLineNumber& inputLineNumber)
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

	// sort the contents of fStartedPartGroupsListsVector
	// in decreasing identityorder
	sortfStartedPartGroupsListsVector (
		inputLineNumber);

	// sort the contents of fStoppedPartGroupsListsVector
	// in decreasing partGroupSequentialNumber order
	sortStoppedPartGroupsListsVector (
		inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTracePartGroups ()) {
		displayAllCollectedData (
			inputLineNumber,
			"AFTER sorting started and stopped MSXR part groups lists");
	}
#endif // MF_TRACE_IS_ENABLED

  // handle each partGroupSequentialNumber in turn in increasing order
  for (int partGroupSequentialNumber = 0; partGroupSequentialNumber<= fCurrentPartGroupSequentialNumber; ++partGroupSequentialNumber) {
//     if (partGroupSequentialNumber> 0) { JMI 0.9.72
//       // parts actual identitys start at 1
      // append part to current part group, i.e. to the top of the stack
      S_msrPart
        part =
          fPartsVector [partGroupSequentialNumber];

      // fetch the part group stack top
			if (fPartGroupsStack.empty ()) {
        std::stringstream ss;

        ss <<
          "Cannot append part " <<
          part->fetchPartNameForTrace () <<
          " to any part group " <<
          " at partGroupSequentialNumber" << partGroupSequentialNumber<<
          ", since the part groups groups stack is empty"<<
					", line " << inputLineNumber;

        musicxmlError (
          gServiceRunData->getInputSourceName (),

#ifdef MF_USE_WRAPPED_TYPES
						inputLineNumber.getBareValue (),
#else
						inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

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

    // handle the part groups groups stopped at partGroupSequentialNumber
    handleThePartGroupsStoppedAtIdentity (
    	inputLineNumber,
    	partGroupSequentialNumber);
	}

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
	const mfInputLineNumber& inputLineNumber,
	const mfStaffNumber&     staffNumber)
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

#ifdef MF_USE_WRAPPED_TYPES
        fetchStaffFromPart (staffNumber.getBareValue ());
#else
        fetchStaffFromPart (staffNumber);
#endif // MF_USE_WRAPPED_TYPES

  if (! staff) {
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addRegularStaffToPartByItsNumber (

#ifdef MF_USE_WRAPPED_TYPES
					inputLineNumber.getBareValue (),
#else
					inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

          msrStaffKind::kStaffKindRegular,

#ifdef MF_USE_WRAPPED_TYPES
          staffNumber.getBareValue ());
#else
          staffNumber);
#endif // MF_USE_WRAPPED_TYPES

		// register the current staff number in the service run data
		S_mfServiceRunData
			serviceRunData =
				gServiceRunData;

		serviceRunData->
			setCurrentStaveNumber (

#ifdef MF_USE_WRAPPED_TYPES
          staffNumber.getBareValue ());
#else
          staffNumber);
#endif // MF_USE_WRAPPED_TYPES

	}

  return staff;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrSkeletonBuilder::createRegularVoiceInStaffIfNotYetDone (
	const mfInputLineNumber& inputLineNumber,
	const mfStaffNumber&     staffNumber,
	const mfVoiceNumber&     voiceNumber)
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

#ifdef MF_USE_WRAPPED_TYPES
					inputLineNumber.getBareValue (),
#else
					inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES

#ifdef MF_USE_WRAPPED_TYPES
          voiceNumber.getBareValue ());
#else
          voiceNumber);
#endif // MF_USE_WRAPPED_TYPES

  if (! voice) {
    // create the voice and append it to the staff
    voice =
      staff->
        createRegularVoiceInStaffByItsNumber (

#ifdef MF_USE_WRAPPED_TYPES
					inputLineNumber.getBareValue (),
#else
					inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES


#ifdef MF_USE_WRAPPED_TYPES
          voiceNumber.getBareValue (),
#else
          voiceNumber,
#endif // MF_USE_WRAPPED_TYPES


#ifdef MF_USE_WRAPPED_TYPES
          fCurrentMeasureNumber.getBareValue (),
#else
          fCurrentMeasureNumber,
#endif // MF_USE_WRAPPED_TYPES

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
	mfInputLineNumber inputLineNumber,
	const S_msrPart&  thePart)
{
  // is the harmonies voice already present in thePart?
  S_msrVoice
    partHarmoniesVoice =
      thePart->
        getPartHarmoniesVoice ();

  if (! partHarmoniesVoice) {
    // create the harmonies voice and append it to thePart
    partHarmoniesVoice =
      thePart->
        createPartHarmoniesVoice (

#ifdef MF_USE_WRAPPED_TYPES
          inputLineNumber.getBareValue (),
#else
          inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES


#ifdef MF_USE_WRAPPED_TYPES
          fCurrentMeasureNumber.getBareValue ());
#else
          fCurrentMeasureNumber);
#endif // MF_USE_WRAPPED_TYPES
  }

  return partHarmoniesVoice;
}

S_msrVoice mxsr2msrSkeletonBuilder::createPartFiguredBassVoiceIfNotYetDone (
	mfInputLineNumber inputLineNumber,
	const S_msrPart&  thePart)
{
  // is the figured bass voice already present in thePart?
  S_msrVoice
    partFiguredBassVoice =
      thePart->
        getPartFiguredBassVoice ();

  if (! partFiguredBassVoice) {
    // create the figured bass voice and append it to thePart
    partFiguredBassVoice =
      thePart->
        createPartFiguredBassVoice (

#ifdef MF_USE_WRAPPED_TYPES
          inputLineNumber.getBareValue (),
#else
          inputLineNumber,
#endif // MF_USE_WRAPPED_TYPES


#ifdef MF_USE_WRAPPED_TYPES
          fCurrentMeasureNumber.getBareValue ());
#else
          fCurrentMeasureNumber);
#endif // MF_USE_WRAPPED_TYPES

  }

  return partFiguredBassVoice;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handleChordMemberNoteIfRelevant (
	const mfInputLineNumber& inputStartLineNumber)
{
	// an end of measure enforces the end of a chord or tuplets  CHORD_TUP
	// the current note is then the one preceding the end of measure

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
  	displayGatheredNoteInformations (
  		"handleChordMemberNoteIfRelevant()");

// 		gLog <<
// 			"===> visitEnd (S_measure& elt)" <<
// 			", fCurrentNoteInputLineNumber: " <<
// 			fCurrentNoteInputLineNumber <<
//
// 			", currentEventSequentialNumber: " <<
// 			fResultingEventsCollection.getCurrentEventSequentialNumber () <<
//
// 			", fCurrentNoteSequentialNumber: " <<
// 			fCurrentNoteSequentialNumber <<
// 			", fPreviousNoteSequentialNumber: " <<
// 			fPreviousNoteSequentialNumber <<
//
// 			", fCurrentNoteIsAGraceNote: " <<
// 			fCurrentNoteIsAGraceNote <<
// 			", fPreviousNoteIsAGraceNote: " <<
// 			fPreviousNoteIsAGraceNote <<
//
// 			", fCurrentNoteIsACueNote: " <<
// 			fCurrentNoteIsACueNote <<
// 			", fPreviousNoteIsACueNote: " <<
// 			fPreviousNoteIsACueNote <<
//
// 			", fCurrentNoteBelongsToAChord: " <<
// 			fCurrentNoteBelongsToAChord <<
// 			", fPreviousNoteBelongsToAChord: " <<
// 			fPreviousNoteBelongsToAChord <<
//
// 			", fPreviousNoteIsATakeOffCandidate: " <<
// 			fPreviousNoteIsATakeOffCandidate <<
//       ", line " << inputStartLineNumber <<
// 			std::endl;
//
// // 		if (gTraceOahGroup->getTraceTupletsBasics ()) {
// // 			gLog <<
// // 				", fCurrentTupletNumber: " <<
// // 				fCurrentTupletNumber <<
// // 				std::endl;
// //
// // 			displayPendingTupletBeginEventsMap (
// // 				"=====> visitEnd (S_measure& elt)",
// // 				fCurrentNoteInputLineNumber);
// // 			displayPendingTupletEndEventsMap (
// // 				"=====> visitEnd (S_measure& elt)",
// // 				fCurrentNoteInputLineNumber);
// // 		}
	}
#endif // MF_TRACE_IS_ENABLED

 	// Q_MEASURE

	// register the chord end event if any, before pending tuplets stops if any
	if (false && fCurrentNoteBelongsToAChord) {
		// the note before the measure end is the last one of the chord
		// it is still the current note

		// create a chord end event
		S_mxsrChordEvent
			chordEndEvent	=
				fResultingEventsCollection.createAChordEndEvent (
					fCurrentNoteSequentialNumber,
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber,
					fCurrentNoteInputLineNumber);

		// register it
		fResultingEventsCollection.registerChordEndEvent (
			chordEndEvent);
	}
}

void mxsr2msrSkeletonBuilder::displayPendingTupletBeginEventsList (
	const std::string&       title,
	const mfInputLineNumber& inputStartLineNumber) const
{
	gLog <<
		">>>>> fPendingTupletBeginEventsList contents - " <<
		title <<
		", line " << inputStartLineNumber <<
		std::endl;

	++gIndenter;

	for (S_mxsrTupletEvent tupletEvent : fPendingTupletBeginEventsList) {
		gLog <<
			tupletEvent <<
			std::endl;
	} // for

	--gIndenter;

	gLog <<
		"<<<<<" <<
		std::endl << std::endl;
}

void mxsr2msrSkeletonBuilder::displayPendingTupletEndEventsList (
	const std::string&       title,
	const mfInputLineNumber& inputStartLineNumber) const
{
	gLog <<
		">>>>> displayPendingTupletEndEventsList contents - " <<
		title <<
		", line " << inputStartLineNumber <<
		std::endl;

	++gIndenter;

	for (S_mxsrTupletEvent tupletEvent : fPendingTupletEndEventsList) {
		gLog <<
			tupletEvent <<
			std::endl;
	} // for

	--gIndenter;

	gLog <<
		"<<<<<" <<
		std::endl << std::endl;
}

// //________________________________________________________________________
// void mxsr2msrSkeletonBuilder::displayPendingTupletBeginEventsMap (
// 	const std::string&       title,
// 	const mfInputLineNumber& inputStartLineNumber) const
// {
// 	gLog <<
// 		">>>>> fPendingTupletsBeginEventsMap contents - " <<
// 		title <<
// 		", line " << inputStartLineNumber <<
// 		std::endl;
//
// 	++gIndenter;
//
// 	for (
// 		std::pair <int, S_mxsrTupletEvent>
// 			thePair : fPendingTupletsBeginEventsMap
// 	) {
// 		int
// 			eventSequentialNumber = thePair.first;
// 		S_mxsrTupletEvent
// 			tupletStopDescription = thePair.second;
//
// 		gLog <<
// 			eventSequentialNumber << ":" <<
// 			std::endl;
//
// 		++gIndenter;
// 		gLog <<
// 			tupletStopDescription <<
// 			std::endl;
// 		--gIndenter;
// 	} // for
//
// 	--gIndenter;
//
// 	gLog <<
// 		"<<<<<" <<
// 		std::endl << std::endl;
// }
//
// void mxsr2msrSkeletonBuilder::displayPendingTupletEndEventsMap (
// 	const std::string&       title,
// 	const mfInputLineNumber& inputStartLineNumber) const
// {
// 	gLog <<
// 		">>>>> fPendingTupletsEndEventsMap contents - " <<
// 		title <<
// 		", line " << inputStartLineNumber <<
// 		std::endl;
//
// 	++gIndenter;
//
// 	for (
// 		std::pair <int, S_mxsrTupletEvent>
// 			thePair : fPendingTupletsEndEventsMap
// 	) {
// 		int
// 			eventSequentialNumber = thePair.first;
// 		S_mxsrTupletEvent
// 			tupletStopDescription = thePair.second;
//
// 		gLog <<
// 			eventSequentialNumber << ":" <<
// 			std::endl;
//
// 		++gIndenter;
// 		gLog <<
// 			tupletStopDescription <<
// 			std::endl;
// 		--gIndenter;
// 	} // for
//
// 	--gIndenter;
//
// 	gLog <<
// 		"<<<<<" <<
// 		std::endl << std::endl;
// }

void mxsr2msrSkeletonBuilder::handlePendingTupletBeginEventsAfterANoteIfAny (
	const mfInputLineNumber& inputStartLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
		displayPendingTupletBeginEventsList (
			"handlePendingTupletBeginEventsAfterANoteIfAny(): ",
			inputStartLineNumber);
  }
#endif // MF_TRACE_IS_ENABLED

	// register pending tuplet begin events if any
	if (! fPendingTupletBeginEventsList.empty ()) {
		std::list <S_mxsrTupletEvent>::iterator
			iBegin = fPendingTupletBeginEventsList.begin (),
			iEnd   = fPendingTupletBeginEventsList.end (),
			i      = iBegin;

		for ( ; ; ) {
			S_mxsrTupletEvent
				tupletBeginEvent = (*i);

	#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceTupletsBasics ()) {
				std::stringstream ss;

				ss <<
					"--> handling pending tuplet begin event: " <<
					tupletBeginEvent->asString () <<
					", line " << inputStartLineNumber;

				gWaeHandler->waeTrace (
					__FILE__, __LINE__,
					ss.str ());
			}
	#endif // MF_TRACE_IS_ENABLED

			// register pendingTupletBeginEvent
			fResultingEventsCollection.registerTupletBeginEvent (
				tupletBeginEvent);

			// forget about it
			i = fPendingTupletBeginEventsList.erase (i);

			if (++i == iEnd) break;
		} // for
	}
}

void mxsr2msrSkeletonBuilder::handlePendingTupletEndEventsAfterANoteIfAny (
	const mfInputLineNumber& inputStartLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
		displayPendingTupletEndEventsList (
			"handlePendingTupletEndEventsAfterANoteIfAny(): ",
			inputStartLineNumber);
  }
#endif // MF_TRACE_IS_ENABLED

	// register pending tuplet end events
	if (! fPendingTupletEndEventsList.empty ()) {
		std::list <S_mxsrTupletEvent>::iterator
			iBegin = fPendingTupletEndEventsList.begin (),
			iEnd   = fPendingTupletEndEventsList.end (),
			i      = iBegin;

		for ( ; ; ) {
			S_mxsrTupletEvent
				tupletEndEvent = (*i);

	#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceTupletsBasics ()) {
				std::stringstream ss;

				ss <<
					"--> handling pending tuplet end event: " <<
					tupletEndEvent->asString () <<
					", line " << inputStartLineNumber;

				gWaeHandler->waeTrace (
					__FILE__, __LINE__,
					ss.str ());
			}
	#endif // MF_TRACE_IS_ENABLED

			// register pendingTupletEndEvent
			fResultingEventsCollection.registerTupletEndEvent (
				tupletEndEvent);

			// forget about it
			i = fPendingTupletEndEventsList.erase (i);

			if (++i == iEnd) break;
		} // for
	}
}

// void mxsr2msrSkeletonBuilder::handlePendingTupletEndEventsAtMeasureEndIfAny (
// 	const mfInputLineNumber& inputStartLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsBasics ()) {
// 		displayPendingTupletEndEventsList (
// 			"handlePendingTupletEndEventsAtMeasureEndIfAny(): ",
// 			inputStartLineNumber);
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// 	// register pending tuplet end events
// 	if (! fPendingTupletEndEventsList.empty ()) {
// 		std::list <S_mxsrTupletEvent>::iterator
// 			iBegin = fPendingTupletEndEventsList.begin (),
// 			iEnd   = fPendingTupletEndEventsList.end (),
// 			i      = iBegin;
//
// 		for ( ; ; ) {
// 			S_mxsrTupletEvent
// 				tupletEndEvent = (*i);
//
// 	#ifdef MF_TRACE_IS_ENABLED
// 		if (gTraceOahGroup->getTraceTupletsBasics ()) {
// 				std::stringstream ss;
//
// 				ss <<
// 					"--> handling pending tuplet end event: " <<
// 					tupletEndEvent->asString () <<
// 					", line " << inputStartLineNumber;
//
// 				gWaeHandler->waeTrace (
// 					__FILE__, __LINE__,
// 					ss.str ());
// 			}
// 	#endif // MF_TRACE_IS_ENABLED
//
// 			// register pendingTupletEndEvent
// 			fResultingEventsCollection.registerTupletEndEvent (
// 				tupletEndEvent);
//
// 			// forget about it
// 			i = fPendingTupletEndEventsList.erase (i);
//
// 			if (++i == iEnd) break;
// 		} // for
// 	}
// }

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_score_partwise& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_partwise" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_partwise" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the first and last measure numbers and measures number in the score JMI 0.9.68
  fMsrScore->
    setScoreFirstMeasureNumber (
      fScoreFirstMeasureBareNumber);
  fMsrScore->
    setScoreLastMeasureNumber (
      fScoreLastMeasureBareNumber);

  fMsrScore->
    setScoreMeasuresNumber (
      fScoreMeasuresNumber);

  // register the first and last measure numbers and measures number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setScoreFirstMeasureNumber (
      fScoreFirstMeasureBareNumber);
  serviceRunData->
    setScoreLastMeasureNumber (
      fScoreLastMeasureBareNumber);

  serviceRunData->
    setScoreMeasuresNumber (
      fScoreMeasuresNumber);

/* JMI ??? THROW AWAY ??? 0.9.69
  // fetch the identification from the credits if any
  fMsrScore ->
    fetchIdentificationFromCreditsIfAny (
      elt->getInputLineNumber ());
      */
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_work_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_number" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationWorkNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_work_title& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_work_title" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string workCreditTypeTitle = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationWorkCreditTypeTitle (
      elt->getInputLineNumber (),
      workCreditTypeTitle);
}

void mxsr2msrSkeletonBuilder::visitStart (S_opus& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_opus" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string opus = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationOpus (
      elt->getInputLineNumber (),
      opus);
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_number" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationMovementNumber (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_movement_title& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_movement_title" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      movementTitle);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_identification" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitEnd (S_identification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_identification" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_creator& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_creator" <<
      ", line " << elt->getInputLineNumber ();

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
        elt->getInputLineNumber (),
        creatorValue);
  }

  else if (creatorType == "arranger") {
    fMsrScore->getIdentification () ->
      appendArranger (
        elt->getInputLineNumber (),
        creatorValue);
  }

  else if (creatorType == "lyricist") {
    fMsrScore->getIdentification () ->
      appendLyricist (
        elt->getInputLineNumber (),
        creatorValue);

    // should we use lyricists as poets?
    if (gGlobalMxsr2msrOahGroup->getUseLyricistsAsPoets ()) {
      fMsrScore->getIdentification () ->
        appendPoet (
          elt->getInputLineNumber (),
          elt->getValue ());
    }
  }

  else if (creatorType == "poet") {
    fMsrScore->getIdentification () ->
      appendPoet (
        elt->getInputLineNumber (),
        elt->getValue ());
  }

  else if (creatorType == "translator") {
    fMsrScore->getIdentification () ->
      appendTranslator (
        elt->getInputLineNumber (),
        creatorValue);
  }

  else if (creatorType == "artist") {
    fMsrScore->getIdentification () ->
      appendArtist (
        elt->getInputLineNumber (),
        creatorValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "creator type \"" << creatorType <<
      "\" is unknown" <<
      ", line " << elt->getInputLineNumber ();

// JMI    musicxmlError (
    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
//      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_rights& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rights" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      elt->getInputLineNumber (),
      rightsValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_source& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_source" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      rightsValue);
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_relation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_relation" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_encoding" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_date& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_date" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoder& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoder" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_software& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_software" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      std::string ("<software /> contains 'Cubase'")
      	+
      ", line "
      	+
      std::to_string (elt->getInputLineNumber ()));

    // is the '-cubase' option set???
    if (gGlobalMxsr2msrOahGroup->getCubase ()) {
      S_oahElement
        cubaseOption =
          gGlobalMxsr2msrOahGroup->
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
      elt->getInputLineNumber (),
      softwareValue);
}

void mxsr2msrSkeletonBuilder::visitStart (S_encoding_description& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_encoding_description" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart (S_supports& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_supports" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      elt->getInputLineNumber (),
      elt->getValue ());
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      miscellaneousFielValue);
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_miscellaneous& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_miscellaneous" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_miscellaneous_field" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
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
    %partGroupSequentialNumber;
    %halign;
    %valign-image;
    %optional-unique-id;
>
*/

void mxsr2msrSkeletonBuilder::visitStart (S_credit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      creditPageNumber);

  // append it to the score
  fMsrScore->
    appendCreditToScore (fCurrentCredit);
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_type" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_symbol" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_image& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_image" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_credit_words& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_credit_words" <<
      ", line " << elt->getInputLineNumber ();

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

  // partGroupSequentialNumber
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
        elt->getInputLineNumber (),
        creditWordsFontWeightString);

  // font style
  std::string creditWordsFontStyleString =
    elt->getAttributeValue ("font-style");

  msrFontStyleKind
    creditWordsFontStyleKind =
      msrFontStyleKindFromString (
        elt->getInputLineNumber (),
        creditWordsFontStyleString);

  // justify
  std::string creditWordsJustifyString =
    elt->getAttributeValue ("justify");

  msrJustifyKind
    creditWordsJustifyKind =
      msrJustifyKindFromString (
        elt->getInputLineNumber (),
        creditWordsJustifyString);

  // halign
  std::string creditWordsHAlignString =
    elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    creditWordsHorizontalAlignment =
      msrHorizontalAlignmentKindFromString (
        elt->getInputLineNumber (),
        creditWordsHAlignString);

  // valign
  std::string creditWordsVAlignString =
    elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    creditWordsVerticalAlignment =
      msrVerticalAlignmentKindFromString (
        elt->getInputLineNumber (),
        creditWordsVAlignString);

  // XMLLang
  std::string creditWordsXMLLangString =
    elt->getAttributeValue ("xml:lang");

  msrXMLLangKind
    creditWordsXMLLang =
      msrXMLLangKindFromString (
        elt->getInputLineNumber (),
        creditWordsXMLLangString);

  // create the credit words
  S_msrCreditWords
    creditWords =
      msrCreditWords::create (
        elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_credit" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_list" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_list" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

//   // do the HARD job...
//   handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
//     elt->getInputLineNumber ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayAllCollectedData (
      elt->getInputLineNumber (),
      "visitEnd (S_part_list& elt)");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_group& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_group" <<
      ", line " << elt->getInputLineNumber ();

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
      ", line " << elt->getInputLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_name" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_name_display" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      printObjectString);

  fOnGoingPartGroupNameDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_group_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_group_name_display" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_text" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      std::string (
      	"<display-text /> is out of context")
      		+
      	", line "
      		+
      	std::to_string (elt->getInputLineNumber ()));
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_accidental_text& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental_text" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_abbreviation" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_symbol" <<
      ", line " << elt->getInputLineNumber ();

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
    if (! groupSymbol.empty ()) {
      // part group type may be absent
      std::stringstream ss;

      ss <<
        "unknown part group symbol \"" + groupSymbol + "\""<<
				", line " << elt->getInputLineNumber ();

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_group_barline" <<
      ", line " << elt->getInputLineNumber ();

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
      ", line " << elt->getInputLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_part_group& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_group" <<
      ", line " << elt->getInputLineNumber ();

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
      ", fCurrentPartGroupSequentialNumber: " <<
      fCurrentPartGroupSequentialNumber<<
      ", line " << elt->getInputLineNumber ();

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
        elt->getInputLineNumber ());
      break;

    case msrPartGroupTypeKind::kPartGroupTypeStop:
      // handle the part group stop
      handlePartGroupStop (
        elt->getInputLineNumber ());
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_part" <<
      ", line " << elt->getInputLineNumber ();

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
      "Found <score-part /> \"" << scorePartID << "\"" <<
      " in part list" <<
      ", line " << elt->getInputLineNumber ();

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

  regex_match (scorePartID, sm, e);

  if (sm.size () == 1) {
    std::stringstream ss;

    ss <<
      "scorePartID \"" << scorePartID << "\"" <<
      " is a pure number" <<
      ", line " << elt->getInputLineNumber ();

    musicxmlWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_name" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// part name

  fCurrentPartName = elt->getValue ();

	// print style

  std::string printStyle = elt->getAttributeValue ("print-style"); // JMI
  if (printStyle == "JMI") { // JMI ???
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputLineNumber (),
      printObjectString);

  if (printObjectString == "JMI") { // JMI ???
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_name_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_name_display" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_name_display" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_abbreviation" <<
      ", line " << elt->getInputLineNumber ();

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
      elt->getInputLineNumber (),
      printObjectString);
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_abbreviation_display& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_abbreviation" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_score_part" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePartGroups ()) {
//   std::stringstream ss;
//
//   ss <<
//     "Incrementing fCurrentPartGroupSequentialNumber to " <<
//     fCurrentPartGroupSequentialNumber<<
//     ", line " << elt->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

  std::string scorePartID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
  std::stringstream ss;

  ss <<
    "Handling score part with ID \"" << scorePartID << "\"" <<
    ", fCurrentPartGroupSequentialNumber: " <<
    fCurrentPartGroupSequentialNumber<<
    ", line " << elt->getInputLineNumber ();

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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrPart
    thePart =
      msrPart::create (
        elt->getInputLineNumber (),
        scorePartID); // fPartUpLinkToPartGroup will be set later

  // populate thePart
  thePart->
    setPartName (
      fCurrentPartName);
  thePart->
    setPartNameDisplayText (
      fCurrentPartNameDisplayText);

  thePart->
    setPartAbbreviation (
      fCurrentPartAbbreviation);
  thePart->
    setPartAbbreviationDisplayText (
      fCurrentPartAbbreviationDisplayText);

  thePart->
    setPartInstrumentName (
      fCurrentPartInstrumentName);
  thePart->
    setPartInstrumentAbbreviation (
      fCurrentPartInstrumentAbbreviation);

  // register thePart in the MSR parts data
  registerPart (
    elt->getInputLineNumber (),
    thePart);

	// set theMsrPart's part uplink to the part group it belongs to
  thePart->
  	setPartUpLinkToPartGroup (
  		fPartGroupsStack.top ()->
  			getMsrPartGroup ()); // JMI HARD 0.9.69
  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part" <<
      ", line " << elt->getInputLineNumber ();

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
      " partMusicXMLID \"" << fCurrentPartID << "\"" <<
//       "partName \"" << fCurrentPartName << "\"" << JMI from fPartGroupElementsList ??? 0.9.67
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentPartID.empty ()) {
		std::stringstream ss;

		ss <<
			"part ID is empty in <part />";

		musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
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
      ", line " << elt->getInputLineNumber ();

    musicxmlError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
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
        fCurrentPart->fetchPartIDAndName ());
  }

  // measures
  fPartNumberOfMeasures = 0;

  // staves and voices
  fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  fCurrentNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
  fPreviousNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part" <<
      ", line " << elt->getInputLineNumber ();

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
        "part " << fCurrentPart->fetchPartNameForTrace () <<
        " has " << fPartNumberOfMeasures <<
        " measures while the other ones have " << fScoreMeasuresNumber<<
				", line " << elt->getInputLineNumber ();

      musicxmlError (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
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

/* JMI 0.9.69
  // are there more staves in <staves/> that specified with <staff/>?
  fCurrentPart->
    addAVoiceToStavesThatHaveNone (
      elt->getInputLineNumber ());
*/

  // assign sequential numbers to the voices in the part
  fCurrentPart->
    assignSequentialNumbersToRegularVoicesInPart (
      elt->getInputLineNumber ());

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

		fCurrentPart->displayPartStavesMap (
			elt->getInputLineNumber (),
			"mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)");

		gLog << std::endl << std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
// void mxsr2msrSkeletonBuilder::visitStart (S_staves& elt)
// {
// 	// JMI 0.9.72 should be ignored!
//
// // #ifdef MF_TRACE_IS_ENABLED
// //   if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
// //     std::stringstream ss;
// //
// //     ss <<
// //       "--> Start visiting S_direction" <<
// //       ", line " << elt->getInputLineNumber ();
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
// //       " in part " << fCurrentPart->fetchPartNameForTrace();
// //
// //     gWaeHandler->waeTrace (
// //       __FILE__, __LINE__,
// //       ss.str ());
// //   }
// // #endif // MF_TRACE_IS_ENABLED
//
// //   if (stavesNumber > 1) {
// //     // add stavesNumber staves to current part // JMI 0.9.72 which staff numbers...???
// //     int n = 1;
// //
// //     while (n <= stavesNumber) {
// //     /* JMI 0.9.69
// //       fCurrentPart->
// //         addRegularStaffToPartByItsNumber (
// //           elt->getInputLineNumber (),
// //           msrStaffKind::kStaffKindRegular,
// //           n);
// //           */
// //       S_msrStaff
// //         dummyStaff = // JMI
// //           createStaffInCurrentPartIfNotYetDone (
// //             elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff" <<
      ", line " << elt->getInputLineNumber ();

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
      ", line " << elt->getInputLineNumber ();

      mfAssert (
        __FILE__, __LINE__,
				false,
				ss.str ());
  }

	fCurrentNoteStaffNumber = staffNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== "
      "visitStart (S_staff& elt)" <<
      ", staffNumber: " << staffNumber <<
      ", fCurrentNoteStaffNumber: " << fPreviousNoteStaffNumber <<
      ", fPreviousNoteStaffNumber: " << fPreviousNoteStaffNumber <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) { // JMI 0.9.72
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_voice" <<
      ", line " << elt->getInputLineNumber ();

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
      ", line " << elt->getInputLineNumber () <<
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_print" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_print" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

	// handler various numbers
	fPreviousMeasureInputLineNumber = fCurrentMeasureInputLineNumber;
  fCurrentMeasureInputLineNumber = elt->getInputLineNumber ();

	fPreviousMeasureNumber = fCurrentMeasureNumber;

#ifdef MF_USE_WRAPPED_TYPES
  fCurrentMeasureNumber.setBareValue (elt->getAttributeValue ("number"));
#else
  fCurrentMeasureNumber = elt->getAttributeValue ("number");
#endif // MF_USE_WRAPPED_TYPES

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== visitStart (S_measure& elt)" <<
      ", fCurrentPartName \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID \"" << fCurrentPartID << "\")" <<
      ", fPreviousMeasureNumber \"" << fPreviousMeasureNumber << "\"" <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", fCurrentMultipleMeasureRestNumber \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
      ", fRemainingMultipleMeasureRestNumber \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
      ", line " << fCurrentMeasureInputLineNumber <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take this measure into account
  ++fPartNumberOfMeasures;

  if (fScoreFirstMeasureBareNumber == K_MEASURE_NUMBER_UNKNOWN_) { // JMI 0.9.68
#ifdef MF_USE_WRAPPED_TYPES
  	fScoreFirstMeasureBareNumber = fCurrentMeasureNumber.getBareValue ();
		fScoreLastMeasureBareNumber = fCurrentMeasureNumber.getBareValue ();
  }
#else
  	fScoreFirstMeasureBareNumber = fCurrentMeasureNumber;
		fScoreLastMeasureBareNumber = fCurrentMeasureNumber;
  }
#endif // MF_USE_WRAPPED_TYPES

  // register the current measure number in the service run data for use by OAH
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (

#ifdef MF_USE_WRAPPED_TYPES
          fCurrentMeasureNumber.getBareValue ());
#else
          fCurrentMeasureNumber);
#endif // MF_USE_WRAPPED_TYPES

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "<!--=== visitEnd (S_measure& elt) 1" <<
      ", fCurrentPartName \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID \"" << fCurrentPartID << "\")" <<
      ", fPreviousMeasureNumber \"" << fPreviousMeasureNumber << "\"" <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", fOnGoingMultipleMeasureRest: " << fOnGoingMultipleMeasureRest <<
      ", fOnGoingMeasureRepeat: " << fOnGoingMeasureRepeat <<
      ", fCurrentMultipleMeasureRestNumber \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
      ", fRemainingMultipleMeasureRestNumber \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
      ", line " << fCurrentMeasureInputLineNumber <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// if there is an ongoing multiple measure rest?
	// this done only now in visitEnd (S_measure& elt),
	// because it was not known in visitStart (S_measure& elt) for this measure
	// that is was the first one of a multiple measure rest
	if (fOnGoingMultipleMeasureRest) {
		// yes

		// account for this measure in the multiple measure rests countdown
		--fRemainingMultipleMeasureRestNumber;

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceMeasuresBasics ()) {
			std::stringstream ss;

			ss <<
				"<!--=== visitEnd (S_measure& elt) 2" <<
				", fCurrentPartName \"" << fCurrentPartName << "\"" <<
				" (fCurrentPartID \"" << fCurrentPartID << "\")" <<
				", fPreviousMeasureNumber \"" << fPreviousMeasureNumber << "\"" <<
				", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
				", fOnGoingMultipleMeasureRest: " << fOnGoingMultipleMeasureRest <<
				", fCurrentMultipleMeasureRestNumber \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
				", fRemainingMultipleMeasureRestNumber \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
				", line " << fCurrentMeasureInputLineNumber <<
				" ===-->";

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		if (fRemainingMultipleMeasureRestNumber == 0) {
			// this measure in the last one in the multiple measure rests group
#ifdef MF_TRACE_IS_ENABLED
			if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
				std::stringstream ss;

				ss <<
					"--> There is a multiple measure rest end event" <<
					", fCurrentMeasureNumber: " <<
					fCurrentMeasureNumber <<
					", fCurrentMultipleMeasureRestNumber: " <<
					fCurrentMultipleMeasureRestNumber <<
					", line " << elt->getInputLineNumber ();

				gWaeHandler->waeTrace (
					__FILE__, __LINE__,
					ss.str ());
			}
#endif // MF_TRACE_IS_ENABLED

			// create a multiple measure rest end event
// 			S_mxsrMultipleMeasureRestEvent
// 				multipleMeasureRestEndEvent =
// 					fResultingEventsCollection.createAMultipleMeasureRestEnd (
// 						fCurrentPart->getPartName (),
// 						fCurrentMeasureNumber,
// 						fCurrentMultipleMeasureRestNumber,
// 						elt->getInputLineNumber ());
//
// 			// register it
// 			fResultingEventsCollection.registerMultipleMeasureRestEnd (
// 				multipleMeasureRestEndEvent);

			std::string
				partName =
					fCurrentPart->getPartName ();

			mfInputLineNumber
				inputLineNumber =
					elt->getInputLineNumber ();

			fResultingEventsCollection.createAMultipleMeasureRestEndAndRegisterIt (
				partName,
				fCurrentMeasureNumber,
				fCurrentMultipleMeasureRestNumber,
				inputLineNumber);

			fOnGoingMultipleMeasureRest = false;
		}
	}

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "<!--=== visitEnd (S_measure& elt) 3" <<
      ", fCurrentPartName \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID \"" << fCurrentPartID << "\")" <<
      ", fPreviousMeasureNumber \"" << fPreviousMeasureNumber << "\"" <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", fOnGoingMultipleMeasureRest: " << fOnGoingMultipleMeasureRest <<
      ", fCurrentMultipleMeasureRestNumber \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
      ", fRemainingMultipleMeasureRestNumber \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
      ", line " << fCurrentMeasureInputLineNumber <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// 	handleChordMemberNoteIfRelevant (
// 		elt->getInputLineNumber () );

 	// create the MXSR events that can be detected only upon the next measure or note
 	// because such emd are detected one measure or note too late

	if (fOnGoingMeasureRepeat) {
		switch (fCurrentMeasureRepeatKind) {
			case msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_:
				// should not occur
				break;

			case msrMeasureRepeatKind::kMeasureRepeatStart:
				break;

			case msrMeasureRepeatKind::kMeasureRepeatStop:
				{
					// create a multiple measure rest end event upon the previous measure,
					// since we're one measure too late
// 					S_mxsrMeasureRepeatEvent
// 						multipleMeasureResEndEvent =
// 							fResultingEventsCollection.createAMeasureRepeatEnd (
// 								fCurrentPart->getPartName (),
// 								fPreviousMeasureNumber,
// 								fCurrentMeasureRepeatNumber,
// 								fCurrentMeasureRepeatSlashes,
// 								fPreviousMeasureInputLineNumber);
//
// 					// register it
// 					fResultingEventsCollection.registerMeasureRepeatEnd (
// 						multipleMeasureResEndEvent);

			std::string
				partName =
					fCurrentPart->getPartName ();

// 			mfInputLineNumber
// 				inputLineNumber =
// 					elt->getInputLineNumber ();

					fResultingEventsCollection.createAMeasureRepeatEndAndRegisterIt (
						partName,
						fPreviousMeasureNumber,
						fCurrentMeasureRepeatNumber,
						fCurrentMeasureRepeatSlashes,
						fPreviousMeasureInputLineNumber);
				}
				break;
		} // switch

		fOnGoingMeasureRepeat = false;
	}

	// grace notes
	if (fCurrentNoteIsAGraceNote) {
		// the current note is the last one of a grace notes group
		fResultingEventsCollection.registerGraceEnd (
			fCurrentNoteSequentialNumber,
			fCurrentNoteStaffNumber,
			fCurrentNoteVoiceNumber,
			fCurrentNoteInputLineNumber);
	}

	// cue notes ???
	if (fCurrentNoteIsACueNote) {
		// the current note is the last one of a cue notes group

		// create a cue end event
		S_mxsrCueEvent
			cueEndEvent	=
				fResultingEventsCollection.createACueEndEvent (
				fCurrentNoteSequentialNumber,
				fCurrentNoteStaffNumber,
				fCurrentNoteVoiceNumber,
				fCurrentNoteInputLineNumber);

		// register it
		fResultingEventsCollection.registerCueEndEvent (
			cueEndEvent);
	}

	// chords
	if (fCurrentNoteBelongsToAChord) {
		// the current note is the last one of a chord

		// create a chord end event
		S_mxsrChordEvent
			chordEndEvent	=
				fResultingEventsCollection.createAChordEndEvent (
					fCurrentNoteSequentialNumber,
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber,
					fCurrentNoteInputLineNumber);

		// register it
		fResultingEventsCollection.registerChordEndEvent (
			chordEndEvent);
	}

// 	// handle pending tuplet stops if any, after the chord end if any,
// 	// which occurs on the current notes, i.e. the one at the end of the measure JMI 0.9.72
// 	handlePendingTupletEndEventsAtMeasureEndIfAny (
// 		fPreviousNoteInputLineNumber);

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_multiple_rest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_multiple_rest" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
	The text of the multiple-rest element indicates the number
	of measures in the multiple rest. Multiple rests may use
	the 1-bar / 2-bar / 4-bar rest symbols, or a single shape.
	The use-symbols attribute indicates which to use; it is no
	if not specified.
-->
<!ELEMENT multiple-rest (#PCDATA)>
<!ATTLIST multiple-rest
    use-symbols %yes-no; #IMPLIED
>

v4.0
The stop type indicates the first measure where the repeats are *no longer* displayed.
  Both the start and the stop of the measures being repeated should be specified
  unless the repeats are displayed through the end of the part.

      <attributes>
        <measure-style>
          <multiple-rest>2</multiple-rest>
        </measure-style>
      </attributes>
      <note>
        <rest measure="yes"/>
        <duration>96</duration>
        <voice>1</voice>
      </note>
    </measure>
    <!--=======================================================-->
    <measure number="2" width="0">
      <note>
        <rest measure="yes"/>
        <duration>96</duration>
        <voice>1</voice>
      </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
      </barline>
    </measure>
*/

  // multiple measure rest number

  fCurrentMultipleMeasureRestNumber = (int)(*elt);

  // start counting down the measures in the group:
  // the one containing the <S_multiple_rest /> is the first one in the group,
  // it will be accounted for in visitEnd (S_measure& elt):
  fRemainingMultipleMeasureRestNumber =
  	fCurrentMultipleMeasureRestNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
    std::stringstream ss;

    ss <<
			"--> There is a multiple measure rest begin event" <<
      ", fCurrentPartName: \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID: \"" << fCurrentPartID << "\")" <<
      ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber << "\"" <<
      ", fCurrentMultipleMeasureRestNumber: \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
      ", fRemainingMultipleMeasureRestNumber: \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	// create a multiple measure rest begin event
	S_mxsrMultipleMeasureRestEvent
		multipleMeasureResBegintEvent =
			fResultingEventsCollection.createAMultipleMeasureRestBegin (
				fCurrentPart->getPartName (),
				fCurrentMeasureNumber,
				fCurrentMultipleMeasureRestNumber,
				fCurrentMeasureInputLineNumber);

	// register it
	fResultingEventsCollection.registerMultipleMeasureRestBegin (
		multipleMeasureResBegintEvent);

  fOnGoingMultipleMeasureRest = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_multiple_rest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_multiple_rest" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "<!--=== visitEnd (S_multiple_rest& elt)" <<
//       ", fCurrentPartName: \"" << fCurrentPartName << "\"" <<
//       " (fCurrentPartID: \"" << fCurrentPartID << "\")" <<
//       ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber << "\"" <<
//       ", fCurrentMultipleMeasureRestNumber: \"" << fCurrentMultipleMeasureRestNumber << "\"" <<
//       ", fRemainingMultipleMeasureRestNumber: \"" << fRemainingMultipleMeasureRestNumber << "\"" <<
//       ", line " << elt->getInputLineNumber () <<
//       " ===-->";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_measure_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_repeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
      <attributes>
        <measure-style>
          <measure-repeat slashes="1" type="start">1</measure-repeat>
        </measure-style>
      </attributes>
*/

/*
<!--
	The measure-repeat element is used for both single and
	multiple measure repeats. The text of the element indicates
	the number of measures to be repeated in a single pattern.
	The slashes attribute specifies the number of slashes to
	use in the repeat sign. It is 1 if not specified. Both the
	start and the stop of the measure-repeat must be specified.
-->
<!ELEMENT measure-repeat (#PCDATA)>
<!ATTLIST measure-repeat
    type %start-stop; #REQUIRED
    slashes NMTOKEN #IMPLIED
>
*/

	// repeat number

  int CurrentMeasureRepeatNumber = (int)(*elt);

  // type

	std::string measureRepeatType = elt->getAttributeValue ("type");

	fCurrentMeasureRepeatKind =
  	msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_;

	if      (measureRepeatType == "start") {
		fCurrentMeasureRepeatKind = msrMeasureRepeatKind::kMeasureRepeatStart;
	}

	else if (measureRepeatType == "stop") {
		fCurrentMeasureRepeatKind = msrMeasureRepeatKind::kMeasureRepeatStop;
	}

	else {
		std::stringstream ss;

		ss <<
			"measure repeat type \"" << measureRepeatType <<
			"\" is unknown";

		mxsr2msrError (
			gServiceRunData->getInputSourceName (),
			elt->getInputLineNumber (),
			__FILE__, __LINE__,
			ss.str ());
	}

  // slashes

	int fCurrentMeasureRepeatSlashes = elt->getAttributeIntValue ("slashes", 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureBasics ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== visitStart (S_measure_repeat& elt)" <<
      ", fCurrentPartName \"" << fCurrentPartName << "\"" <<
      " (fCurrentPartID \"" << fCurrentPartID << "\")" <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", fCurrentMeasureRepeatKind \"" << fCurrentMeasureRepeatKind << "\"" <<
      ", fCurrentMeasureRepeatNumber \"" << fCurrentMeasureRepeatNumber << "\"" <<
      ", fCurrentMeasureRepeatSlashes \"" << fCurrentMeasureRepeatSlashes << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentMeasureRepeatKind) {
    case msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_:
      // should not occur
      break;

    case msrMeasureRepeatKind::kMeasureRepeatStart:
    	{
				// the measure repeat end upon the previous measure
				S_mxsrMeasureRepeatEvent
					multipleMeasureResBeginEvent =
						fResultingEventsCollection.createAMeasureRepeatBegin (
							fCurrentPart->getPartName (),
							fPreviousMeasureNumber,
							CurrentMeasureRepeatNumber,
							fCurrentMeasureRepeatSlashes,
							fCurrentMeasureInputLineNumber);

				// register it
				fResultingEventsCollection.registerMeasureRepeatBegin (
					multipleMeasureResBeginEvent);
			}
      break;

    case msrMeasureRepeatKind::kMeasureRepeatStop:
      break;
  } // switch

  fOnGoingMeasureRepeat = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_measure_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure_repeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_backup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_repeat" <<
      ", line " << elt->getInputLineNumber ();

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
        elt->getInputLineNumber (),
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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputLineNumber ();

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

	// rests
	fCurrentNoteIsARest = false;

	// grace notes
	fPreviousNoteIsAGraceNote = fCurrentNoteIsAGraceNote;
	fCurrentNoteIsAGraceNote = false;

	// cue notes
	fPreviousNoteIsACueNote = fCurrentNoteIsACueNote;
	fCurrentNoteIsACueNote = false;

	// chords
	fPreviousNoteBelongsToAChord = fCurrentNoteBelongsToAChord;
	fCurrentNoteBelongsToAChord = false;

	// tuplets
  fCurrentNoteBelongsToATuplet = false;

	// staff changes
	fPreviousNoteInputLineNumber = fCurrentNoteInputLineNumber;

	// input line numbers
	fCurrentNoteInputLineNumber =
		elt->getInputLineNumber ();

  // lyrics
  fCurrentStanzaNumber = K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;

  fOnGoingNote = true;
}

void mxsr2msrSkeletonBuilder::visitStart (S_rest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rest" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	fCurrentNoteIsARest = true;

	// a rest cannot be a grace note
	fCurrentNoteIsAGraceNote = false;

	// a rest cannot be a cue note
	fCurrentNoteIsACueNote = false;

	// a rest cannot be part of a chord
	fCurrentNoteBelongsToAChord = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_grace& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_grace" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
		std::stringstream ss;

		ss <<
			"--> There is a grace event" <<
			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<
			", line " << elt->getInputLineNumber ();

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

	fCurrentNoteIsAGraceNote = true;
}

void mxsr2msrSkeletonBuilder::visitStart (S_cue& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_cue" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceCueNotesBasics ()) {
		std::stringstream ss;

		ss <<
			"--> There is a cue event" <<
			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<
			", line " << elt->getInputLineNumber ();

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

	fCurrentNoteIsACueNote = true;
}

//________________________________________________________________________
Bool mxsr2msrSkeletonBuilder::handleStaffChangeIfAny (
	const mfInputLineNumber& inputStartLineNumber)
{
	Bool result;

  if (
    fCurrentNoteStaffNumber != fPreviousNoteStaffNumber
      &&
    fCurrentNoteVoiceNumber == fPreviousNoteVoiceNumber
      &&
    fPreviousNoteIsATakeOffCandidate
//     fPreviousNoteStaffNumber != K_STAFF_NUMBER_UNKNOWN_ // JMI STAFF_CHANGE 0.9.72
  ) {
    // yes
		result = true;

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
			std::stringstream ss;

			ss <<
				"===> handleStaffChangeIfAny(): "
				"There is a staff change event:" <<
				'\n' <<
				"fCurrentNoteInputLineNumber: " <<
				fCurrentNoteInputLineNumber <<

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

				", line " << fCurrentNoteInputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLE

		mfStaffNumber
			takeOffStaffNumber = fPreviousNoteStaffNumber,
			landingStaffNumber = fCurrentNoteStaffNumber;

		mfVoiceNumber
			changingVoiceNumber = fPreviousNoteVoiceNumber,
			landingVoiceNumber = fCurrentNoteVoiceNumber;

		mfInputLineNumber
			takeOffNoteInputLineNumber = fPreviousNoteInputLineNumber,
			landingNoteInputLineNumber = fCurrentNoteInputLineNumber;

		// create the landing staff if not yet done
		S_msrVoice
			landingVoice =
				createRegularVoiceInStaffIfNotYetDone (
					takeOffNoteInputLineNumber,
					landingStaffNumber,
					landingVoiceNumber);

    // register a staff change event
    // taking off from the previous note
    // and landing on the current note

		// the airplane takes off upon the previous note
		// and lands upon the current note

		// let's create two staff changes
		// one for take off upon the previous note
		fResultingEventsCollection.registerStaffChangeTakeOff ( // CHORD_TUP JMI 0.9.72
			fPreviousNoteSequentialNumber,
			fPreviousNoteStaffNumber,
			changingVoiceNumber,							// the voice that changes staff
			takeOffStaffNumber,
			takeOffNoteInputLineNumber,
			landingStaffNumber,
			landingNoteInputLineNumber,
			fPreviousNoteInputLineNumber);

		// the other one for landing upon the current note
		fResultingEventsCollection.registerStaffChangeLanding ( // CHORD_TUP JMI 0.9.72
			fCurrentNoteSequentialNumber,
			fCurrentNoteStaffNumber,
			changingVoiceNumber,							// the voice that changes staff
			takeOffStaffNumber,
			takeOffNoteInputLineNumber,
			landingStaffNumber,
			landingNoteInputLineNumber,
			fCurrentNoteInputLineNumber);
  }

	return result;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::displayGatheredNoteInformations (
	const std::string& context)
{
	int constexpr fieldWidth = 33;

	gLog << std::left <<
		"--> " <<
		context <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fCurrentNoteInputLineNumber " << ": " <<
		fCurrentNoteInputLineNumber <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteSequentialNumber" << ": " <<
		fPreviousNoteSequentialNumber <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteSequentialNumber" << ": " <<
		fCurrentNoteSequentialNumber <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fCurrentNoteStaffNumber" << ": " <<
		fCurrentNoteStaffNumber <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteVoiceNumber" << ": " <<
		fCurrentNoteVoiceNumber <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteIsATakeOffCandidate" << ": " <<
		fPreviousNoteIsATakeOffCandidate <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteIsAGraceNote" << ": " <<
		fPreviousNoteIsAGraceNote <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteIsAGraceNote" << ": " <<
		fCurrentNoteIsAGraceNote <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteIsACueNote" << ": " <<
		fPreviousNoteIsACueNote <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteIsACueNote" << ": " <<
		fCurrentNoteIsACueNote <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteBelongsToAChord" << ": " <<
		fPreviousNoteBelongsToAChord <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteBelongsToAChord" << ": " <<
		fCurrentNoteBelongsToAChord <<
		std::endl <<

		std::setw (fieldWidth) <<
		"fPreviousNoteBelongsToATuplet" << ": " <<
		fPreviousNoteBelongsToATuplet <<
		std::endl <<
		std::setw (fieldWidth) <<
		"fCurrentNoteBelongsToATuplet" << ": " <<
		fCurrentNoteBelongsToATuplet <<
		std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerGraceEventIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
  	displayGatheredNoteInformations (
  		"registerGraceEventIfAny()");
  }
#endif // MF_TRACE_IS_ENABLED

 	// Q_NOTE

	if (fCurrentNoteIsAGraceNote) {
		// was the previous note also a grace note?
		if (fPreviousNoteIsAGraceNote) {
			// grace notes group continues
			// wait and see upon the next note or the measure end
		}
		else {
			// the current note is the grace notes begin
			fResultingEventsCollection.registerGraceBegin (
				fCurrentNoteSequentialNumber,
				fCurrentNoteStaffNumber,
				fCurrentNoteVoiceNumber,
				fCurrentNoteInputLineNumber);
		}
	}

	else {
		// is this note the one that follows the last one of a grace notes group?
		if (fPreviousNoteIsAGraceNote) {
			// this is the note right after the last one of the grace notes group
			// we're one note late, hence the previous note is the grace notes group end
			fResultingEventsCollection.registerGraceEnd (
				fPreviousNoteSequentialNumber,
				fPreviousNoteStaffNumber,
				fPreviousNoteVoiceNumber,
				fPreviousNoteInputLineNumber);
		}
		else {
			// wait and see upon the next note or the measure end
		}
	}
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerCueEventIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
  	displayGatheredNoteInformations (
  		"registerCueEventIfAny()");
  }
#endif // MF_TRACE_IS_ENABLED

 	// Q_NOTE

	if (fCurrentNoteIsACueNote) {
		// was the previous note also a cue note?
		if (fPreviousNoteIsACueNote) {
			// cue notes group continues
			// wait and see upon the next note or the measure end
		}
		else {
			// the current note is the cue notes begin

			// create a cue begin event
			S_mxsrCueEvent
				cueBeginEvent	=
					fResultingEventsCollection.createACueBeginEvent (
					fCurrentNoteSequentialNumber,
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber,
					fCurrentNoteInputLineNumber);

			// register it
			fResultingEventsCollection.registerCueBeginEvent (
				cueBeginEvent);
		}
	}

	else {
		// is this note the one that follows the last one of a cue notes group?
		if (fPreviousNoteIsACueNote) {
			// this is the note right after the last one of the cue notes group
			// we're one note late, hence the previous note is the cue notes group end

			// create a cue end event
			S_mxsrCueEvent
				cueEndEvent	=
					fResultingEventsCollection.createACueEndEvent (
						fPreviousNoteSequentialNumber,
						fPreviousNoteStaffNumber,
						fPreviousNoteVoiceNumber,
						fPreviousNoteInputLineNumber);

			// register it
			fResultingEventsCollection.registerCueEndEvent (
				cueEndEvent);
		}
		else {
			// wait and see upon the next note or the measure end
		}
	}
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerChordEventIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
  	displayGatheredNoteInformations (
  		"registerChordEventIfAny()");
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

			// create a chord begin event
			S_mxsrChordEvent
				chordBeginEvent	=
					fResultingEventsCollection.createAChordBeginEvent (
					fPreviousNoteSequentialNumber,
					fPreviousNoteStaffNumber,
					fPreviousNoteVoiceNumber,
					fPreviousNoteInputLineNumber);

			// register it
			fResultingEventsCollection.registerChordBeginEvent (
				chordBeginEvent);
		}
	}

	else {
		// is this note the one that follows the last one of a chord?
		if (fPreviousNoteBelongsToAChord) {
			// this is the note right after the last one of the chord
			// we're one note late, hence the previous note is the chord end

			// create a chord end event
			S_mxsrChordEvent
				chordEndEvent	=
					fResultingEventsCollection.createAChordEndEvent (
						fPreviousNoteSequentialNumber,
						fPreviousNoteStaffNumber,
						fPreviousNoteVoiceNumber,
						fPreviousNoteInputLineNumber);

			// register it
			fResultingEventsCollection.registerChordEndEvent (
				chordEndEvent);

// 			fPreviousNoteBelongsToAChord = false;
		}
		else {
			// wait and see upon the next note or the measure end
		}
	}
}

//________________________________________________________________________
void  mxsr2msrSkeletonBuilder::visitEnd (S_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_note" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 	// Q_NOTE

	// handle staff change if any
	Bool
		thereIsAStaffChange =
			handleStaffChangeIfAny (elt->getInputLineNumber ());

	if (! thereIsAStaffChange) {
		// the current note is a regular one

		// create the staff if not yet done
		S_msrStaff
			staff =
				createStaffInCurrentPartIfNotYetDone (
					elt->getInputLineNumber (),
					fCurrentNoteStaffNumber);

		// create the voice if not yet done
		S_msrVoice
			noteVoice =
				createRegularVoiceInStaffIfNotYetDone (
					elt->getInputLineNumber (),
					fCurrentNoteStaffNumber,
					fCurrentNoteVoiceNumber);

#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceNotesBasics ()) {
			displayGatheredNoteInformations (
				"visitEnd (S_note& elt)");

			std::stringstream ss;

			ss <<
				"--> visitEnd (S_note& elt) 1 (all):"
				", getStaffPathLikeName(): " <<
				staff->getStaffPathLikeName () <<
				", getVoiceName(): " <<
				noteVoice->getVoiceName () <<
				", line " << fCurrentNoteInputLineNumber;

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());

// 			displayPendingTupletEventsList (
// 				"=====> visitEnd (S_note& elt)",
// 				fCurrentNoteInputLineNumber);
//
// 			displayPendingTupletBeginEventsMap (
// 				"=====> visitEnd (S_note& elt)",
// 				fCurrentNoteInputLineNumber);
// 			displayPendingTupletEndEventsMap (
// 				"=====> visitEnd (S_note& elt)",
// 				fCurrentNoteInputLineNumber);
		}
#endif // MF_TRACE_IS_ENABLED
	}

	if (fCurrentNoteIsARest) {
#ifdef MF_TRACE_IS_ENABLED
		if (gTraceOahGroup->getTraceNotesBasics ()) {
			std::stringstream ss;

			ss <<
				"Current note is a rest" <<
				", line " <<
				elt->getInputLineNumber ();

			gWaeHandler->waeTrace (
				__FILE__, __LINE__,
				ss.str ());
		}
#endif // MF_TRACE_IS_ENABLED

		if (false && fPreviousNoteBelongsToAChord) {
			// this is the note right after the last one of the chord
			// we're one note late, hence the previous note is the chord end

			// create a chord end event
			S_mxsrChordEvent
				chordEndEvent	=
					fResultingEventsCollection.createAChordEndEvent (
						fPreviousNoteSequentialNumber,
						fPreviousNoteStaffNumber,
						fPreviousNoteVoiceNumber,
						fPreviousNoteInputLineNumber);

			// register it
			fResultingEventsCollection.registerChordEndEvent (
				chordEndEvent);
		}
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
          elt->getInputLineNumber ();

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
            elt->getInputLineNumber (),
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
          elt->getInputLineNumber ();

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
            elt->getInputLineNumber (),
            fCurrentPart);
    }

    fThereAreFiguredBassToBeAttachedToCurrentNote = false;
  }

 	// Q_NOTE

	// register grace note event if any
	registerGraceEventIfAny ();

	// register cue note event if any
	registerCueEventIfAny ();

	// register chord event if any
	registerChordEventIfAny ();

 	// Q_NOTE

	// handle pending tuplet stops from the previous note if any
	handlePendingTupletEndEventsAfterANoteIfAny (
		fPreviousNoteInputLineNumber);

	// handle tuplet begin events if any
	handlePendingTupletBeginEventsAfterANoteIfAny (
		fCurrentNoteInputLineNumber);

	// handle tuplet end events if any
	handlePendingTupletEndEventsAfterANoteIfAny (
		fCurrentNoteInputLineNumber);

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceChordsBasics ()) {
		std::stringstream ss;

		ss <<
			"--> visitStart (S_chord& elt): there is a chord event" <<
			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<
			", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_chord" <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  // fPreviousTupletNumber = fCurrentTupletNumber;

  // the default value is implementation dependent, so:
  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 1);

  // there can be several tuplet starts upon a given note,
  // so we should handle them in a list
  // with the last (hence upper) tuplet number first
//   fPendingTupletsStartsNumbers.push_front (
//     fCurrentTupletNumber);

  // type

  {
    std::string tupletType = elt->getAttributeValue ("type");

		fTupletTypeKind = msrTupletTypeKind::kTupletTypeNone;

    if      (tupletType == "start") {
      fTupletTypeKind = msrTupletTypeKind::kTupletTypeStart;
    }

    else if (tupletType == "continue") {
      fTupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;
    }

    else if (tupletType == "stop") {
			fTupletTypeKind = msrTupletTypeKind::kTupletTypeStop;
    }

    else {
      std::stringstream ss;

      ss <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
}

void mxsr2msrSkeletonBuilder::visitEnd (S_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsBasics ()) {
//     displayGatheredTupletInformations (
//       "visitEnd (S_tuplet& elt)");
//   }
// #endif // MF_TRACE_IS_ENABLED

/*
<!--
	A tuplet element is present when a tuplet is to be displayed
	graphically, in addition to the sound data provided by the
	time-modification elements. The number attribute is used to
	distinguish nested tuplets. The bracket attribute is used
	to indicate the presence of a bracket. If unspecified, the
	results are implementation-dependent. The line-shape
	attribute is used to specify whether the bracket is straight
	or in the older curved or slurred style. It is straight by
	default.

	Whereas a time-modification element shows how the cumulative,
	sounding effect of tuplets and double-note tremolos compare to
	the written note type, the tuplet element describes how this
	is displayed. The tuplet element also provides more detailed
	representation information than the time-modification element,
	and is needed to represent nested tuplets and other complex
	tuplets accurately. The tuplet-actual and tuplet-normal
	elements provide optional full control over tuplet
	specifications. Each allows the number and note type
	(including dots) describing a single tuplet. If any of
	these elements are absent, their values are based on the
	time-modification element.

	The show-number attribute is used to display either the
	number of actual notes, the number of both actual and
	normal notes, or neither. It is actual by default. The
	show-type attribute is used to display either the actual
	type, both the actual and normal types, or neither. It is
	none by default.
-->
<!ELEMENT tuplet (tuplet-actual?, tuplet-normal?)>
<!ATTLIST tuplet
    type %start-stop; #REQUIRED
    number %number-level; #IMPLIED
    bracket %yes-no; #IMPLIED
    show-number (actual | both | none) #IMPLIED
    show-type (actual | both | none) #IMPLIED
    %line-shape;
    %position;
    %placement;
>
<!ELEMENT tuplet-actual (tuplet-number?,
	tuplet-type?, tuplet-dot*)>
<!ELEMENT tuplet-normal (tuplet-number?,
	tuplet-type?, tuplet-dot*)>
<!ELEMENT tuplet-number (#PCDATA)>
<!ATTLIST tuplet-number
    %font;
    %color;
>
<!ELEMENT tuplet-type (#PCDATA)>
<!ATTLIST tuplet-type
    %font;
    %color;
>
<!ELEMENT tuplet-dot EMPTY>
<!ATTLIST tuplet-dot
    %font;
    %color;
>
*/

	// create an tuplet event is relevant
#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceTupletsBasics ()) {
		std::stringstream ss;

		ss <<
			"Creating a tuplet event if relevant" <<
			", fCurrentTupletNumber: " <<
			fCurrentTupletNumber <<
			", fTupletTypeKind: " <<
			fTupletTypeKind <<

// 			", fTupletActualNotesToBeUsed: " <<
// 			fTupletActualNotesToBeUsed <<
// 			", fTupletNormalNotesToBeUsed: " <<
// 			fTupletNormalNotesToBeUsed <<

			", fCurrentNoteSequentialNumber: " <<
			fCurrentNoteSequentialNumber <<
			", fPreviousNoteSequentialNumber: " <<
			fPreviousNoteSequentialNumber <<

			", fCurrentNoteActualNotes: " <<
			fCurrentNoteActualNotes <<
			", fCurrentNoteNormalNotes: " <<
			fCurrentNoteNormalNotes <<
			", line " << elt->getInputLineNumber ();

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, __LINE__,
    fCurrentTupletActualNotes > 0,
    "fCurrentTupletActualNotes is not positive");
  mfAssert (
    __FILE__, __LINE__,
    fCurrentTupletNormalNotes > 0,
    "fCurrentTupletNormalNotes is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

	switch (fTupletTypeKind) {
		case msrTupletTypeKind::kTupletTypeNone:
			// ??? JMI
			break;

		case msrTupletTypeKind::kTupletTypeStart:
			{
				// create a tuplet begin event
				S_mxsrTupletEvent
					tupletBeginEvent =
						fResultingEventsCollection.createATupletBeginEvent (
							fCurrentTupletNumber,
							msrTupletFactor (
								fCurrentTupletActualNotes,
								fCurrentTupletNormalNotes),
							fCurrentNoteSequentialNumber,
							fCurrentNoteStaffNumber,
							fCurrentNoteVoiceNumber,
							fCurrentNoteInputLineNumber);

			// register it in the pending tuplet begins list
			//	push it ahead of fPendingTupletEventsList
			// 	since there can be several tuplet starts upon a given note
#ifdef MF_TRACE_IS_ENABLED
				if (gTraceOahGroup->getTraceTupletsBasics ()) {
					std::stringstream ss;

					ss <<
						"Appending tuplet end event " <<
						tupletBeginEvent->asString () <<
						" to fPendingTupletBeginEventsList" <<
						", line " << elt->getInputLineNumber ();

					gWaeHandler->waeTrace (
						__FILE__, __LINE__,
						ss.str ());
				}
#endif // MF_TRACE_IS_ENABLED

				fPendingTupletBeginEventsList.push_back (
					tupletBeginEvent);

// 					fPendingTupletsBeginEventsMap.insert (
// 						std::make_pair (
// 							fCurrentNoteSequentialNumber,
// 							tupletBeginEvent));
			}
			break;

		case msrTupletTypeKind::kTupletTypeContinue:
			// no event there
			break;

		case msrTupletTypeKind::kTupletTypeStop:
			{
				// create a tuplet end event
				S_mxsrTupletEvent
					tupletEndEvent =
						fResultingEventsCollection.createATupletEndEvent (
							fCurrentTupletNumber,
							msrTupletFactor (
								fCurrentTupletActualNotes,
								fCurrentTupletNormalNotes),
							fCurrentNoteSequentialNumber,
							fCurrentNoteStaffNumber,
							fCurrentNoteVoiceNumber,
							fCurrentNoteInputLineNumber);

				// register it in the pending tuplet ends list
#ifdef MF_TRACE_IS_ENABLED
				if (gTraceOahGroup->getTraceTupletsBasics ()) {
					std::stringstream ss;

					ss <<
						"Appending tuplet end event " <<
						tupletEndEvent->asString () <<
						" to fPendingTupletEndEventsList" <<
						", line " << elt->getInputLineNumber ();

					gWaeHandler->waeTrace (
						__FILE__, __LINE__,
						ss.str ());
				}
#endif // MF_TRACE_IS_ENABLED

				fPendingTupletEndEventsList.push_back (
					tupletEndEvent);

// 					fPendingTupletsEndEventsMap.insert (
// 						std::make_pair (
// 							fCurrentNoteSequentialNumber,
// 							tupletEndEvent));
			}
			break;
	} // switch
}

void mxsr2msrSkeletonBuilder::visitStart (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_tuplet_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_number" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // not handled JMI
  int tupletNumberValue = (int)(*elt);

  if (fOnGoingTupletActual) {
    fCurrentTupletActualNotes = tupletNumberValue;

    gLog <<
    	"===> visitStart (S_tuplet_number& elt)" <<
    	", fCurrentTupletActualNotes: " <<
    	fCurrentTupletActualNotes <<

    	", fCurrentTupletActualNotes: " <<
    	fCurrentTupletActualNotes <<
    	", line " << elt->getInputLineNumber () <<
    	std::endl;
  }

  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalNotes = tupletNumberValue;

    gLog <<
    	"===> visitStart (S_tuplet_number& elt)" <<
    	", fCurrentTupletNormalNotes: " <<
    	fCurrentTupletNormalNotes <<

//     	", fTupletNormalNotesToBeUsed: " <<
//     	fTupletNormalNotesToBeUsed <<
    	", line " << elt->getInputLineNumber () <<
    	std::endl;
  }

//   else if (fOnGoingNote) {
//     fCurrentNoteActualNotes = tupletNumberValue;
//     fTupletActualNotesToBeUsed = fCurrentNoteActualNotes;
//
//     fCurrentNoteNormalNotes = tupletNumberValue;
//     fTupletNormalNotesToBeUsed = fCurrentNoteNormalNotes;
//
//     gLog <<
//     	"===> visitStart (S_tuplet_number& elt)" <<
//     	", fCurrentNoteActualNotes: " <<
//     	fCurrentNoteActualNotes <<
//     	", fCurrentNoteActualNotes: " <<
//     	fCurrentNoteActualNotes <<
//
//     	", fTupletActualNotesToBeUsed: " <<
//     	fTupletActualNotesToBeUsed <<
//
//     	", fCurrentNoteActualNotes: " <<
//     	fCurrentNoteActualNotes <<
//     	", fTupletActualNotesToBeUsed: " <<
//     	fTupletActualNotesToBeUsed <<
//     	", line " << elt->getInputLineNumber () <<
//     	std::endl;
//   }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-number /> out of context");
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_tuplet_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_type" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // not handled JMI
  std::string tupletTypeValue = elt->getValue();

  if (fOnGoingTupletActual) {
    fCurrentTupletActualType = tupletTypeValue;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalType = tupletTypeValue;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-type /> out of context");
  }

  // color JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "tuplet type (not handled): " <<
      tupletTypeValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonBuilder::visitStart (S_tuplet_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_dot" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingTupletActual) {
    ++fCurrentTupletActualDotsNumber;
  }
  else if (fOnGoingTupletNormal) {
    ++fCurrentTupletNormalDotsNumber;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-dot /> out of context");
  }

  // color JMI
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_time_modification& elt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	fPreviousNoteBelongsToATuplet = fCurrentNoteBelongsToATuplet;
  fCurrentNoteBelongsToATuplet = true;
}

void mxsr2msrSkeletonBuilder::visitStart (S_actual_notes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_actual_notes" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int actualNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteActualNotes = actualNotes;
    fCurrentTupletActualNotes = fCurrentNoteActualNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "visitStart (S_actual_notes& elt)(), fCurrentNoteActualNotes: " <<
        fCurrentNoteActualNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     // notes inside a tuplet have no <tuplet/> markup
//     // and 2 actual notes indicate a double tremolo
//     switch (fCurrentNoteActualNotes) {
//       case 2:
//         fCurrentNoteBelongsToADoubleTremolo = true;
//         break;
//       default:
//         fCurrentNoteBelongsToATuplet = true; // JMI 0.9.70 what when both in tuplet and a chord?
//     } // switch
  }

  else if (fOnGoingMetronomeNote) {
//     fCurrentMetronomeNoteActualNotes = actualNotes;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTempos ()) {
//       std::stringstream ss;
//
//       ss <<
//         "fCurrentMetronomeNoteActualNotes: " <<
//         fCurrentMetronomeNoteActualNotes;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<actual-notes /> \"" << actualNotes <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_normal_notes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_notes" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int normalNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteNormalNotes = normalNotes;
		fCurrentTupletNormalNotes = fCurrentNoteNormalNotes;


#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "visitStart (S_normal_notes& elt), fCurrentNoteNormalNotes: " <<
        fCurrentNoteNormalNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // notes inside a tuplet have no <tuplet/> markup
    // and 1 actual note indicates a double tremolo

//     this is superflous since actual_notes has been seen already: // JMI 0.9.70
//     switch (fCurrentNoteNormalNotes) {
//       case 1:
//         fCurrentNoteBelongsToADoubleTremolo = true;
//         break;
//       default:
//         fCurrentNoteBelongsToATuplet = true;
//     } // switch
  }

  else if (fOnGoingMetronomeNote) {
//     fCurrentMetronomeNoteNormalNotes = normalNotes;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTempos ()) {
//       std::stringstream ss;
//
//       ss <<
//         "fCurrentMetronomeNoteNormalNotes: " <<
//         fCurrentMetronomeNoteNormalNotes;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-notes /> \"" << normalNotes <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_normal_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_type" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string normalTypeString = elt->getValue();

  if (fOnGoingNote) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "normalTypeString: " <<
        normalTypeString;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     // the type contains a display duration
//     fCurrentNoteNormalTypeNotesDuration =
//       mfDurationKindFromMusicXMLGraphicNoteType (
//         elt->getInputLineNumber (),
//         normalTypeString);

  /*
    // there can be several <beat-unit/> in a <metronome/> markup,
    // register beat unit in in dotted durations list
    fCurrentMetronomeBeatUnitsVector.push_back (
      mfDottedNotesDuration (
        fCurrentNoteNormalTypeNotesDuration,
        0));
        */
  }

  else if (fOnGoingMetronomeNote) { // JMI ???
//     fCurrentMetronomeNoteNormalType = normalTypeString;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTempos ()) {
//       std::stringstream ss;
//
//       ss <<
//         "fCurrentMetronomeNoteNormalType: " <<
//         fCurrentMetronomeNoteNormalType;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-type /> \"" << normalTypeString <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_metronome_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_note" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeNote = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_metronome_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome_note" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeNote = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_lyric& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_lyric" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.empty ()) {
      musicxmlWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        std::string (
        	"lyric number is empty, using \"1\" by default")
        		+
        	", line "
        		+
          std::to_string (elt->getInputLineNumber ()));

      fCurrentStanzaNumber = "1";
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "--> setting fCurrentStanzaNumber to " <<
          fCurrentStanzaNumber <<
          ", line " << elt->getInputLineNumber ();

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

    if (fCurrentStanzaName.empty ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        // lyrics names are not so frequent after all...
        std::stringstream ss;

        ss <<
          "lyric name is empty, using \"" <<
          K_STANZA_NAME_UNKNOWN_ <<
          "\" by default" <<
					", line " << elt->getInputLineNumber ();

        musicxmlWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
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
          ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_lyric" <<
      ", line " << elt->getInputLineNumber ();

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
      ", line: " << elt->getInputLineNumber ();

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
          elt->getInputLineNumber (),
          fCurrentNoteStaffNumber,
          fCurrentNoteVoiceNumber);

    // create stanzaNumber in current voice if need be
    S_msrStanza
      stanza =
        currentVoice->
          createStanzaInVoiceIfNotYetDone (
            elt->getInputLineNumber (),
            fCurrentStanzaNumber,
            fCurrentStanzaName);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_harmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmony" <<
      ", harmoniesVoicesCounter: " << fHarmoniesVoicesCounter <<
      ", line " << elt->getInputLineNumber ();

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
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figured_bass" <<
      ", figuredBassVoicesCounter: " << fFiguredBassVoicesCounter <<
      ", line " << elt->getInputLineNumber ();

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



//________________________________________________________________________
// void mxsr2msrSkeletonBuilder::handleTupletEventIfAny ()
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsBasics ()) {
//   	displayGatheredNoteInformations (
//   		"handleTupletEventIfAny()");
//
//     displayPendingTupletBeginEventsList (
//     	"=====> handleTupletEventIfAny()",
//     	fCurrentNoteInputLineNumber);
//     displayPendingTupletEndEventsList (
//     	"=====> handleTupletEventIfAny()",
//     	fCurrentNoteInputLineNumber);
//
//     displayPendingTupletBeginEventsMap (
//     	"=====> handleTupletEventIfAny()",
//     	fCurrentNoteInputLineNumber);
//     displayPendingTupletEndEventsMap (
//     	"=====> handleTupletEventIfAny()",
//     	fCurrentNoteInputLineNumber);
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// 	for (
// 		std::list <S_mxsrTupletEvent>::const_iterator it = fPendingTupletEventsList.begin ();
// 		it != fPendingTupletEventsList.end ();
// 		// no increment
// 	) {
// 		// extract the values from the tuplet
// 		S_mxsrTupletEvent tupletEvent = (*it);
//
// #ifdef MF_TRACE_IS_ENABLED
// 		if (gTraceOahGroup->getTraceTupletsBasics ()) {
// 			std::stringstream ss;
//
// // 			ss <<
// // 	    	"--> handleTupletEventIfAny() 2"
// // 				", tupletInputLineNumber: " <<
// // 				tupletEvent->getTupletInputLineNumber () <<
// // 				", tupletNumber: " <<
// // 				tupletEvent->getTupletNumber () <<
// // 				", tupletEventKind: " <<
// // 				tupletEvent->getTupletTypeKind ();
//
// 			gWaeHandler->waeTrace (
// 				__FILE__, __LINE__,
// 				ss.str ());
// 		}
// #endif // MF_TRACE_IS_ENABLED
//
// 		// handle the values
// 		switch (tupletEvent->getTupletEventKind ()) {
// 			case mxsrTupletEventKind::kTupletEvent_NONE:
// 				// should not occur // JMI 0.9.72
// 				break;
//
// 			case mxsrTupletEventKind::kTupletEventBegin:
// 				{
// 					// handle pending tuplet stops if any, to have then §
// 					// occuring before tuplet start on the same note
// 					handlePendingTupletBeginEventsAfterANoteIfAny (
// 						fPreviousNoteInputLineNumber);
//
// 					// create a tuplet end event
// 					S_mxsrTupletEvent
// 						tupletBeginEvent =
// 							fResultingEventsCollection.createATupletBeginEvent (
// 								tupletEvent->getTupletNumber (),
// 								tupletEvent->getNoteSequentialNumber (),
// 								tupletEvent->getNoteStaffNumber (),
// 								tupletEvent->getNoteVoiceNumber (),
// 								tupletEvent->getEventInputLineNumber ());
//
// // 					// register it as pending
// // 					// register the new tuplet begin event upon the current note at once
// // 					fResultingEventsCollection.registerTupletBeginEvent (
// // 						tupletBeginEvent);
// 					// keep it as pending
// 					fPendingTupletsBeginEventsMap.insert (
// 						std::make_pair (
// 							fCurrentNoteSequentialNumber,
// 							tupletBeginEvent));
// 				}
// 				break;
//
// 			case mxsrTupletEventKind::kTupletEventEnd:
// 				{
// 					// store the tuplet stop description in fPendingTupletsEndEventsMap
// 					// to be handled later,
// 					/// since it may occur before the actual last note in the tuplet
// 					// it event sequential number is temporary,
// 					// it will be set when the tuplet stop event is created
//
// 					// the temporary values will be ignored when the actual ones become known later,
// 					// when  the tuplet stop event is created by registerTupletEndEvent()
//
// 					// create a tuplet end event
// 					S_mxsrTupletEvent
// 						tupletEndEvent =
// 							fResultingEventsCollection.createATupletEndEvent (
// 								tupletEvent->getTupletNumber (),
// 								tupletEvent->getNoteSequentialNumber (),
// 								tupletEvent->getNoteStaffNumber (),
// 								tupletEvent->getNoteVoiceNumber (),
// 								tupletEvent->getEventInputLineNumber ());
//
// 					// keep it as pending
// 					fPendingTupletsEndEventsMap.insert (
// 						std::make_pair (
// 							fCurrentNoteSequentialNumber,
// 							tupletEndEvent));
// 				}
// 				break;
// 		} // switch
//
// 		// forget about the pending tuplet type kind
// 		it = fPendingTupletEventsList.erase (it);
// 	} // for
// }

