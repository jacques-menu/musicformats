/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "tree_browser.h"


#include "musicxmlWae.h"
#include "mxsr2msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msr2summaryVisitor.h"

#include "oahOah.h"

#include "mxsrOah.h"
#include "mxsr2msrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "oahEarlyOptions.h"

#include "mxsr2msrSkeletonBuilder.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mxmlPartGroupDescr mxmlPartGroupDescr::create (
  int            startInputLineNumber,
  int            partGroupNumber,
  S_msrPartGroup partGroup,
  int            startPosition)
{
  mxmlPartGroupDescr* o = new
    mxmlPartGroupDescr (
      startInputLineNumber,
      partGroupNumber,
      partGroup,
      startPosition);
  assert (o != nullptr);
  return o;
}

mxmlPartGroupDescr::mxmlPartGroupDescr (
  int            startInputLineNumber,
  int            partGroupNumber,
  S_msrPartGroup partGroup,
  int            startPosition)
{
  fStartInputLineNumber   = startInputLineNumber;
  fStopInputLineNumber   = -1;

  fPartGroupNumber = partGroupNumber;
  fPartGroup       = partGroup;

  fStartPosition   = startPosition;
  fStopPosition    = -1;
}

mxmlPartGroupDescr::~mxmlPartGroupDescr ()
{}

string mxmlPartGroupDescr::partGroupDescrAsString () const
{
  stringstream s;

  s <<
    "'" <<
    fPartGroupNumber <<
    "' -=> " <<
    fPartGroup->getPartGroupCombinedName ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    s <<
      ", positions " <<
      fStartPosition << ".." << fStopPosition;
  }
#endif

  s <<
    ", lines " <<
    fStartInputLineNumber << ".." << fStopInputLineNumber;

  return s.str ();
}

void mxmlPartGroupDescr::print (ostream& os) const
{
  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "partGroupNumber" << " : " <<
    fPartGroupNumber <<
    endl <<
    setw (fieldWidth) <<
    "partGroup" << " : " <<
    fPartGroup <<
    endl <<
    setw (fieldWidth) <<
    "startPosition" << " : " <<
    fStartPosition <<
    endl <<
    setw (fieldWidth) <<
    "startInputLineNumber" << " : " <<
    fStartInputLineNumber <<
    endl <<
    "stopPosition" << " : " <<
    fStopPosition <<
    endl <<
    setw (fieldWidth) <<
    "stopInputLineNumber" << " : " <<
    fStopInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_mxmlPartGroupDescr& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder ()
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      "msrScore::create()");

  // score handling
  fScoreNumberOfMeasures = 0;

  // part groups handling
  fPartGroupsCounter = 0;
//   fOnGoingPartGroupNameDisplay = false;

  // parts handling
  fCurrentNoIDPartNumber = 0;
  fCurrentPartsPosition = 0;
//   fOnGoingPartNameDisplay = false;

  // staff handling
  fCurrentStaffMusicXMLNumber = -1;

  // voice handling
  fCurrentVoiceMusicXMLNumber = -1;

  // measures
  fPartNumberOfMeasures = 0;

  // lyrics handling
  fCurrentStanzaNumber = msrStanza::K_NO_STANZA_NUMBER; // JMI
  fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME; // JMI

  // harmonies handling
//   fThereAreHarmoniesToBeAttachedToCurrentNote = false;
  fHarmoniesVoicesCounter = 0;

  // figured bass handling
//   fThereAreFiguredBassToBeAttachedToCurrentNote = false;
  fFiguredBassVoicesCounter = 0;

  // ongoing note
//   fOnGoingNote = false;

  // note context
  fCurrentStaffMusicXMLNumber = 0;
  fCurrentVoiceMusicXMLNumber = 0;

  // create an empty list for part groups starting at 0,
  // i.e. for the implicit part group
  fPositionStartingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create an empty list for part groups stopping at 0,
  // that will remain empty
  fPositionStoppingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create the implicit part group
  createImplicitPartGroup ();
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
S_mxmlPartGroupDescr mxsr2msrSkeletonBuilder::fetchStartedPartGroupDescr (
  int partGroupNumber)
{
  S_mxmlPartGroupDescr result;

  if (fStartedPartGroupDescrsMap.count (partGroupNumber)) {
    result =
      fStartedPartGroupDescrsMap [partGroupNumber];
  }

  return result;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showAllPartGroupDescrsMap (
  int inputLineNumber)
{
  gLogStream <<
    "AllPartGroupDescrsMap:" <<
    endl;

  if (fAllPartGroupDescrsMap.size ()) {
    ++gIndenter;

    map<int, S_mxmlPartGroupDescr>::const_iterator
      iBegin = fAllPartGroupDescrsMap.begin (),
      iEnd   = fAllPartGroupDescrsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_mxmlPartGroupDescr
        partGroupDescr = (*i).second;

      S_msrPartGroup
        partGroup =
          partGroupDescr->getPartGroup ();

      gLogStream <<
        partGroupDescr->partGroupDescrAsString () <<
        endl;

        ++gIndenter;

        partGroup->
          printPartGroupElementsList (
            inputLineNumber,
            gLogStream);

        --gIndenter;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  else {
    gLogStream <<
      gTab << "empty map" <<
      endl;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showStartedPartGroupDescrsMap (
  int inputLineNumber)
{
  gLogStream <<
    "StartedPartGroupDescrsMap:" <<
    endl;

  if (fStartedPartGroupDescrsMap.size ()) {
    ++gIndenter;

    map<int, S_mxmlPartGroupDescr>::const_iterator
      iBegin = fStartedPartGroupDescrsMap.begin (),
      iEnd   = fStartedPartGroupDescrsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_mxmlPartGroupDescr
        partGroupDescr = (*i).second;

      S_msrPartGroup
        partGroup =
          partGroupDescr->getPartGroup ();

      gLogStream <<
        partGroupDescr->partGroupDescrAsString () <<
        endl;

        ++gIndenter;

        partGroup->
          printPartGroupElementsList (
            inputLineNumber,
            gLogStream);

        --gIndenter;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  else {
    gLogStream <<
      gTab << "empty map" <<
      endl;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPartGroupsStack (
  int inputLineNumber)
{
  gLogStream <<
    "PartGroupsDescrStack:" <<
    endl;

  if (fPartGroupsDescrStack.size ()) {
    ++gIndenter;

    list<S_mxmlPartGroupDescr>::const_iterator
      iBegin = fPartGroupsDescrStack.begin (),
      iEnd   = fPartGroupsDescrStack.end (),
      i      = iBegin;

    for ( ; ; ) {
      gLogStream <<
        "v " <<
        (*i)->partGroupDescrAsString () <<
        endl;
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  else {
    gLogStream <<
      gTab << "empty stack" <<
      endl;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPartGroupDescrsVector (
  int inputLineNumber)
{
  gLogStream <<
    "PartGroupDescrsVector:" <<
    endl;

  ++gIndenter;

  for (size_t i = 0; i < fPartGroupDescsVector.size (); ++i) {
    S_mxmlPartGroupDescr
      partGroupDescr = fPartGroupDescsVector [i];

    S_msrPartGroup
      partGroup =
        partGroupDescr->getPartGroup ();

    gLogStream <<
      i + 1 << ": " <<
        partGroupDescr->partGroupDescrAsString () <<
      ", line " << inputLineNumber <<
      endl;
  } // for

  gLogStream <<
    "------------------" <<
    endl;

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPositionStartingPartGroupDescrsVector (
  int inputLineNumber)
{
  gLogStream <<
    "PositionStartingPartGroupDescrsVector:" <<
    endl;

  if (fPositionStartingPartGroupDescrsVector.size ()) {
    ++gIndenter;

    for (size_t k = 0; k < fPositionStartingPartGroupDescrsVector.size (); ++k) {
      gLogStream <<
        k << ": " <<
        endl;

      list<S_mxmlPartGroupDescr>&
        startingPartGroupDescrsList =
          fPositionStartingPartGroupDescrsVector [k];

      if (startingPartGroupDescrsList.size ()) {
        ++gIndenter;

        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = startingPartGroupDescrsList.begin (),
          iEnd   = startingPartGroupDescrsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);

          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();

          gLogStream <<
              partGroupDescr->partGroupDescrAsString () <<
            ", line " << inputLineNumber <<
            endl;

          if (++i == iEnd) break;
          // no endl here
        } // for

        --gIndenter;
      }

      else {
        gLogStream <<
          gTab << "empty list" <<
          endl;
      }
    }

    --gIndenter;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPositionStoppingPartGroupDescrsVector (
  int inputLineNumber)
{
  gLogStream <<
    "PositionStoppingPartGroupDescrsVector:" <<
    endl;

  if (fPositionStoppingPartGroupDescrsVector.size ()) {
    ++gIndenter;

    for (size_t k = 0; k < fPositionStoppingPartGroupDescrsVector.size (); ++k) {
      gLogStream <<
        k << ": " <<
        endl;

      list<S_mxmlPartGroupDescr>&
        theList =
          fPositionStoppingPartGroupDescrsVector [k];

      if (theList.size ()) {
        ++gIndenter;

        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = theList.begin (),
          iEnd   = theList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);

          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();

          gLogStream <<
              partGroupDescr->partGroupDescrAsString () <<
            ", line " << inputLineNumber <<
            endl;

          if (++i == iEnd) break;
          // no endl here
        } // for

        --gIndenter;
      }

      else {
        gLogStream <<
          gTab << "empty list" <<
          endl;
      }
    }

    --gIndenter;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPartsVector (
  int inputLineNumber)
{
  gLogStream <<
    "PartsVector:" <<
    endl;

  if (fPartsVector.size ()) {
    ++gIndenter;

    for (size_t i = 0; i < fPartsVector.size (); ++i) {
      S_msrPart
        part = fPartsVector [i];

      gLogStream <<
        i + 1 << ": " <<
        part->getPartCombinedName () <<
        ", upLink to: ";

      S_msrPartGroup
        partPartGroupUpLink =
          part->
            getPartPartGroupUpLink ();

      if (partPartGroupUpLink) {
        gLogStream <<
          partPartGroupUpLink->
            getPartGroupCombinedName ();
      }
      else {
        gLogStream <<
          "not yet established";
      }

      gLogStream <<
        ", line " << inputLineNumber <<
        endl;
    } // for

    --gIndenter;
  }

  else {
    gLogStream <<
      gTab << "empty vector" <<
      endl;
  }

  gLogStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::showPartGroupsData (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    context <<
    ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
    ", line " << inputLineNumber <<
    ":" <<
    endl <<
    ">>> ================================================" <<
    endl;

  showAllPartGroupDescrsMap (
    inputLineNumber);
  gLogStream << endl;

  showStartedPartGroupDescrsMap (
    inputLineNumber);
  gLogStream << endl;

  showPartGroupDescrsVector (
    inputLineNumber);
  gLogStream << endl;

  showPositionStartingPartGroupDescrsVector (
    inputLineNumber);
  gLogStream << endl;

  showPositionStoppingPartGroupDescrsVector (
    inputLineNumber);
  gLogStream << endl;

  showPartsVector (
    inputLineNumber);
  gLogStream << endl;

  showPartGroupsStack (
    inputLineNumber);
  gLogStream << endl;

  gLogStream <<
    " <<< ================================================" <<
    endl << endl;
}

//________________________________________________________________________
S_mxmlPartGroupDescr mxsr2msrSkeletonBuilder::fetchPartGroupDescrStackTop ()
{
  S_mxmlPartGroupDescr result;

  // the current part group is the top of the stack,
  // i.e. the front for the list used to implement it
  if (fPartGroupsDescrStack.size () != 0) {
   result =
    fPartGroupsDescrStack.front ();
  }

  return result;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPartGroupDescrAsStarted (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Registering part group descr '" <<
      partGroupDescr->getPartGroupDescrCombinedName () <<
      " as started" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE registering part group " +
      partGroupDescr->
        partGroupDescrAsString () +
      " in the data");
  }
#endif

  // register part group in part groups vector, i.e. by appearance order
  fPartGroupDescsVector.push_back (
    partGroupDescr);

  // register it in fAllPartGroupDescrsMap under its position as part group,
  // since its part group number may reused later by other part groups
  fAllPartGroupDescrsMap [fPartGroupsCounter] =
    partGroupDescr;

  // register it temporarily in fStartedPartGroupDescrsMap
  // under its part group number,
  int partGroupNumber =
    partGroupDescr->
      getPartGroup ()->
        getPartGroupNumber ();

  fStartedPartGroupDescrsMap [partGroupNumber] =
    partGroupDescr;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER registering part group " +
      partGroupDescr->
        partGroupDescrAsString () +
      " in the data");
  }
#endif
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::insertPartGroupDescInStartingList (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr,
  list<S_mxmlPartGroupDescr>&
                       startingPartGroupDescrsList)
{
  // this list is created in stop position decreasing order,
  // so as to have the outer-most part group descr appearing first
  list<S_mxmlPartGroupDescr>::iterator
    iBegin = startingPartGroupDescrsList.begin (),
    iEnd   = startingPartGroupDescrsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append partGroupDescr to the list
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePartGroups ()) {
        gLogStream <<
          "Appending part group descr " <<
          partGroupDescr->
            partGroupDescrAsString () <<
          "' at the end of the list of part group descrs starting at position " <<
          fCurrentPartsPosition <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      startingPartGroupDescrsList.push_back (
        partGroupDescr);
      /*
      stringstream s;
      s <<
        "part group descr " <<
        partGroupDescr->partGroupDescrAsString () <<
        " could not be inserted in part groups to be stopped list";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxmlPartGroupDescr
      currentPartGroupDescr = (*i);

    if (
      partGroupDescr->getStopPosition ()
        >
      currentPartGroupDescr->getStopPosition ()
    ) {
      // insert partGroupDescr before currentPartGroupDescr
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePartGroups ()) {
        gLogStream <<
          "Inserting part group descr " <<
          currentPartGroupDescr->
            partGroupDescrAsString () <<
          "' in pending list before" <<
          partGroupDescr->
            partGroupDescrAsString () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      startingPartGroupDescrsList.insert (
        i, partGroupDescr);
      break;
    }

    ++i;
  } // while
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::insertPartGroupDescInStoppingList (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr,
  list<S_mxmlPartGroupDescr>&
                       stoppingPartGroupDescrsList)
{
  // this list is created in start position decreasing order,
  // so as to have the inner-most part group descr appearing first
  list<S_mxmlPartGroupDescr>::iterator
    iBegin = stoppingPartGroupDescrsList.begin (),
    iEnd   = stoppingPartGroupDescrsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append partGroupDescr to the list
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePartGroups ()) {
        gLogStream <<
          "Appending part group descr " <<
          partGroupDescr->
            partGroupDescrAsString () <<
          "' at the end of the list of part group descrs stopping at position " <<
          fCurrentPartsPosition <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      stoppingPartGroupDescrsList.push_back (
        partGroupDescr);
      /*
      stringstream s;
      s <<
        "part group descr " <<
        partGroupDescr->partGroupDescrAsString () <<
        " could not be inserted in part groups to be stopped list";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxmlPartGroupDescr
      currentPartGroupDescr = (*i);

    if (
      partGroupDescr->getStartPosition ()
        >
      currentPartGroupDescr->getStartPosition ()
    ) {
      // insert partGroupDescr before currentPartGroupDescr
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePartGroups ()) {
        gLogStream <<
          "Inserting part group descr " <<
          currentPartGroupDescr->
            partGroupDescrAsString () <<
          "' in pending list before" <<
          partGroupDescr->
            partGroupDescrAsString () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      stoppingPartGroupDescrsList.insert (
        i, partGroupDescr);
      break;
    }

    ++i;
  } // while
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPartGroupDescrAsStoppingAtCurrentPosition (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
  // fetch the list of part group descrs stopping at
  // the same position as partGroupDescr
  list<S_mxmlPartGroupDescr>&
    stoppingPartGroupDescrsList =
      fPositionStoppingPartGroupDescrsVector [
        fCurrentPartsPosition];

  if (stoppingPartGroupDescrsList.size () == 0) {
    // first element, insert it directly
    stoppingPartGroupDescrsList.push_back (
      partGroupDescr);
  }

  else {
    // insert the element at the right place in the stopping list
    insertPartGroupDescInStoppingList (
      inputLineNumber,
      partGroupDescr,
      stoppingPartGroupDescrsList);
  }

  // fetch the list of part group descrs starting at
  // the same position as partGroupDescr
  list<S_mxmlPartGroupDescr>&
    startingPartGroupDescrsList =
      fPositionStartingPartGroupDescrsVector [
        partGroupDescr->
          getStartPosition ()];

  if (startingPartGroupDescrsList.size () == 0) {
    // first element, insert it directly
    startingPartGroupDescrsList.push_back (
      partGroupDescr);
  }

  else {
    // insert partGroupDescr at the right place in the starting list
    insertPartGroupDescInStartingList (
      inputLineNumber,
      partGroupDescr,
      startingPartGroupDescrsList);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPartGroupDescrAsStopped (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Registering part group descr '" <<
      partGroupDescr->getPartGroupDescrCombinedName () <<
      " as stopped" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // register part group descr as stopping at current position
  registerPartGroupDescrAsStoppingAtCurrentPosition (
    inputLineNumber,
    partGroupDescr);

  // forget it in fStartedPartGroupDescrsMap,
  // it remains in fAllPartGroupDescrsMap
  fStartedPartGroupDescrsMap.erase (
    partGroupDescr->
      getPartGroup ()->
        getPartGroupNumber ());

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER forgetting part group descr " +
        partGroupDescr->
          partGroupDescrAsString () +
        " in the data");
  }
#endif
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupStart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Creating part group with number '" <<
      fCurrentPartGroupNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++fPartGroupsCounter;

  // create the part group,
  // with the current part group as part group upLink
  S_msrPartGroup
    partGroupToBeStarted =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        fPartGroupsCounter,
        fCurrentPartGroupName,
        fCurrentPartGroupNameDisplayText,
        fCurrentPartGroupAccidentalText,
        fCurrentPartGroupAbbreviation,
        fCurrentPartGroupSymbolKind,
        fCurrentPartGroupSymbolDefaultX,
        msrPartGroup::kPartGroupImplicitNo,
        fCurrentPartGroupBarLineKind,
        nullptr, // partGroupPartGroupUpLink will be set upon 'stop'
        fMsrScore);

  // partGroupToBeStarted will be appended to the MSR score
  // upon 'stop', once it is complete

  // create the part group descr
  S_mxmlPartGroupDescr
    partGroupDescr =
      mxmlPartGroupDescr::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        partGroupToBeStarted,
        fCurrentPartsPosition) ;

  // register it in the part groups data
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Adding part group descr for '" << fCurrentPartGroupNumber <<
      "' to visitor's part groups data" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  registerPartGroupDescrAsStarted (
    inputLineNumber,
    partGroupDescr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStart()");
  }
#endif
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::handlePartGroupStop (
  int inputLineNumber)
{
  // is the part group to be stopped started?
  S_mxmlPartGroupDescr
    partGroupDescrToBeStopped =
      fetchStartedPartGroupDescr (
        fCurrentPartGroupNumber);

  if (! partGroupDescrToBeStopped) {
    // no, but we should have found it
    stringstream s;

    s <<
      "no part group '" << fCurrentPartGroupNumber <<
      "' has not been started, it cannot be stopped";

 // JMI   musicxmlError (
    musicxmlWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());
  }

  else {
    // set the stopping position
    partGroupDescrToBeStopped->
      setStopPosition (
        inputLineNumber,
        fCurrentPartsPosition);

    // register partGroupDescrToBeStopped as stopped
    registerPartGroupDescrAsStopped (
      inputLineNumber,
      partGroupDescrToBeStopped);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStop()");
  }
#endif
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::doNestPartGroupDescrInItsContainer (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescrToBeStopped,
  S_mxmlPartGroupDescr containingPartGroupDescr)
{
  // get part groups from descrs
  S_msrPartGroup
    partGroupToBeStopped =
      partGroupDescrToBeStopped->
        getPartGroup (),
    containingPartGroup =
      containingPartGroupDescr->
        getPartGroup ();

  // set currentPartGroup's upLink to containingPartGroupDescr
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Setting the upLink of part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  partGroupToBeStopped->
    setPartGroupPartGroupUpLink (
      containingPartGroup);

  // appending currentPartGroup to containingPartGroupDescr
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending sub part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  containingPartGroup->
    appendSubPartGroupToPartGroup (
      partGroupToBeStopped);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::createImplicitPartGroup ()
{
  // an implicit part group has to be created to contain everything,
  // since there can be parts out of any part group

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fImplicitPartGroup == nullptr,
    "fImplicitPartGroup already exists");

  int inputLineNumber = 0;
    // this occurs independantly from the MusicXML data

  // create an implicit part group
  fCurrentPartGroupNumber = 0;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Creating an implicit part group with number '" <<
      fCurrentPartGroupNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++fPartGroupsCounter;

  fImplicitPartGroup =
    msrPartGroup::createImplicitPartGroup (
      fCurrentPartGroupNumber,
      fPartGroupsCounter,
      "Implicit_PartGroup", // partGroupName
      "",                   // PartGroupNameDisplayText
      "",                   // partGroupAccidentalText
      "Impl. PG",           // partGroupAbbreviation
      msrPartGroup::kPartGroupBarLineYes,
      fMsrScore);

  // append it to the MSR score
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending implicit part group '" <<
      fImplicitPartGroup->getPartGroupNumber () <<
      "' to MSR score" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMsrScore->
    addPartGroupToScore (
      fImplicitPartGroup);

  // create the part group descr for the implicit part group
  fImplicitPartGroupDescr =
    mxmlPartGroupDescr::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fImplicitPartGroup,
      fCurrentPartsPosition);

  // register it in the part groups data
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Adding implicit part group descr for '" <<
      fCurrentPartGroupNumber <<
      "' to the part groups data" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  registerPartGroupDescrAsStarted (
    inputLineNumber,
    fImplicitPartGroupDescr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER creating fImplicitPartGroup");
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::doPartGroupsNestingAndPartsAllocation (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "doPartGroupsNestingAndPartsAllocation:" <<
      endl;
  }
#endif

  // this is actually a partial subpass of pass 2a,
  // since we run through the contents of <part-list />,
  // stored in the data we've built, a second time

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE doPartGroupsNestingAndPartsAllocation");
  }
#endif

  // handle each position in turn
  for (int k = 0; k <= fCurrentPartsPosition; ++k) {

    if (k > 0) {
      // parts actual positions start at 1
      // append part to current part group, i.e. to the top of the stack
      S_msrPart
        part =
          fPartsVector [k - 1];

      // fetch the part group descr stack top
      S_mxmlPartGroupDescr
        partGroupsDescrStackTop =
          fetchPartGroupDescrStackTop ();

      if (! partGroupsDescrStackTop) {
        stringstream s;

        s <<
          "Cannot append part " <<
          part->getPartCombinedName () <<
          " to any part group " <<
          " at position " << k <<
          ", since the stack is empty";

        musicxmlError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // append part to current part group
      S_msrPartGroup
        currentPartGroup =
          partGroupsDescrStackTop->
            getPartGroup ();

      currentPartGroup->
        appendPartToPartGroup (
          part);
    }

    // handle the part groups descrs stopping at position k
    if (fPositionStoppingPartGroupDescrsVector.size ()) {
      list<S_mxmlPartGroupDescr>&
        stoppingPartGroupDescrsList =
          fPositionStoppingPartGroupDescrsVector [k];

      if (stoppingPartGroupDescrsList.size ()) {
        ++gIndenter;

        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = stoppingPartGroupDescrsList.begin (),
          iEnd   = stoppingPartGroupDescrsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);

          int stopInputLineNumber =
            partGroupDescr->getStopInputLineNumber ();

          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();

          // fetch the part group descr stack top
          S_mxmlPartGroupDescr
            partGroupsDescrStackTop =
              fetchPartGroupDescrStackTop ();

          if (! partGroupsDescrStackTop) {
            stringstream s;

            s <<
              "Cannot 'stop' part group descr " <<
              partGroupDescr->getPartGroupDescrCombinedName () <<
              " at position " << k <<
              ", since the stack is empty";

            musicxmlError (
              gGlobalServiceRunData->getInputSourceName (),
              stopInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }

          if (partGroupsDescrStackTop == partGroupDescr) {
            // pop partGroupDescrToBeStopped from the stack
            S_mxmlPartGroupDescr
              partGroupsDescrStackTop =
                fPartGroupsDescrStack.front ();

#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTracePartGroups ()) {
              gLogStream <<
                "Popping part group " <<
                partGroupDescr->
                  getPartGroupDescrCombinedName () <<
                "' from the stack" <<
                ", line " << stopInputLineNumber <<
                endl;
            }
#endif

            fPartGroupsDescrStack.pop_front ();

            // the implicit part group isn't contained in any other
            if (partGroupDescr != fImplicitPartGroupDescr) {
              // fetch new current part group
              S_mxmlPartGroupDescr
                newPartGroupDescrStackTop =
                  fetchPartGroupDescrStackTop ();

              if (! newPartGroupDescrStackTop) {
                stringstream s;

                s <<
                  "there is no part group in the stack to nest part group descr " <<
                  partGroupDescr->partGroupDescrAsString () <<
                  fCurrentPartID << "\"" <<
                  " into";

                mxsr2msrInternalError (
                  gGlobalServiceRunData->getInputSourceName (),
                  stopInputLineNumber,
                  __FILE__, __LINE__,
                  s.str ());
              }

              // partGroupDescr is nested in newPartGroupDescrStackTop,
              // do the nesting
              doNestPartGroupDescrInItsContainer (
                stopInputLineNumber,
                partGroupDescr,
                newPartGroupDescrStackTop);
            }
          }

          else {
#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTracePartGroups ()) {
              showPartGroupsData (
                stopInputLineNumber,
                "UPON overlapping part groups");
            }
#endif

            stringstream s;

            s <<
              endl <<
              "There are overlapping part groups, namely: " <<
              endl <<
              gTab << partGroupDescr->partGroupDescrAsString () <<
              endl <<
              "and" <<
              endl <<
              gTab << partGroupsDescrStackTop->partGroupDescrAsString () <<
              endl;

/* JMI
            // fetch the positions in the intersection
            int
              startOne =
                partGroupDescr->getStartPosition (),
              startTwo =
                partGroupsDescrStackTop->getStartPosition (),
              stopOne =
                partGroupDescr->getStopPosition (),
              stopTwo =
                partGroupsDescrStackTop->getStopPosition ();

            int firstCommonPosision = startOne;
            if (startTwo > startOne) {
              firstCommonPosision = startTwo;
            }

            int lastCommonPosision = stopOne;
            if (stopTwo < stopOne) {
              lastCommonPosision = stopTwo;
            }

            s <<
              endl <<
              "The parts they share are:" <<
              endl;

            s <<
              "(positions range is " <<
              firstCommonPosision << ".." << lastCommonPosision <<
              ")" <<
              endl;

            for (int m = firstCommonPosision; m <= lastCommonPosision; ++m) {
              S_msrPart
                part =
                  fPartsVector [m];

              s <<
                gTab <<
                part->getPartCombinedName () <<
                ", line " << part->getInputLineNumber () <<
                endl;
            } // for
*/

            s <<
              endl <<
              regex_replace (
R"(Please contact the maintainers of MusicFormats (see option '-c, -contact'):
  either you found a bug in the EXECUTABLE translator,
  or this MusicXML data is the first-ever real-world case
  of a score exhibiting overlapping part groups.)",
              regex ("EXECUTABLE"),
              gGlobalOahOahGroup->getOahOahGroupServiceName ());

            musicxmlError (
              gGlobalServiceRunData->getInputSourceName (),
              stopInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }

          if (++i == iEnd) break;
          // no endl here
        } // for

        --gIndenter;
      }
    }

    // handle the part groups descrs starting at position k
    if (fPositionStartingPartGroupDescrsVector.size ()) {
      list<S_mxmlPartGroupDescr>&
        startingPartGroupDescrsList =
          fPositionStartingPartGroupDescrsVector [k];

      if (startingPartGroupDescrsList.size ()) {
        ++gIndenter;

        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = startingPartGroupDescrsList.begin (),
          iEnd   = startingPartGroupDescrsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);

          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();

          // make it the new current part group
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTracePartGroups ()) {
            gLogStream <<
              "Pushing part group descr '" <<
              partGroupDescr->partGroupDescrAsString () <<
              "' onto the part groups stack" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          fPartGroupsDescrStack.push_front (
            partGroupDescr);

          if (++i == iEnd) break;
          // no endl here
        } // for

        --gIndenter;
      }
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
      stringstream s;

      s <<
        "AT position " << k;

      showPartGroupsData (
        inputLineNumber,
        s.str ());
    }
#endif

  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER doPartGroupsNestingAndPartsAllocation");
  }
#endif
}

//______________________________________________________________________________
S_msrStaff mxsr2msrSkeletonBuilder::createStaffInCurrentPartIfNotYetDone (
  int inputLineNumber,
  int staffNumber)
{
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
  }

  return voice;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrSkeletonBuilder::fetchFirstRegularVoiceFromStaff (
  int inputLineNumber,
  int staffNumber)
{
  // the voice number is relative to a part,

  // create the staff if not yet done
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNotYetDone (
        inputLineNumber,
        staffNumber);

  // fetch the first regular
  S_msrVoice
    voice =
      staff->
        fetchFirstRegularVoiceFromStaff (
          inputLineNumber);

  return voice;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrSkeletonBuilder::createRegularVoiceHarmoniesVoiceIfNotYetDone (
  int        inputLineNumber,
  S_msrVoice voice)
{
  // is the harmonies voice already present in voice?
  S_msrVoice
    harmoniesVoice =
      voice->
        getRegularVoiceHarmoniesVoiceForwardLink ();

  if (! harmoniesVoice) {
    // create the voice and append it to the staff
    harmoniesVoice =
      voice->
        createRegularVoiceHarmoniesVoice (
          inputLineNumber,
          fCurrentMeasureNumber);
  }

  return harmoniesVoice;
}

S_msrVoice mxsr2msrSkeletonBuilder::createPartHarmoniesVoiceIfNotYetDone (
  int        inputLineNumber,
  S_msrPart  part)
{
  // is the harmonies voice already present in part?
  S_msrVoice
    partHarmoniesVoice =
      part->
        getPartHarmoniesVoice ();

  if (! partHarmoniesVoice) {
    // create the harmonies voice and append it to the part
    partHarmoniesVoice =
      part->
        createPartHarmoniesVoice (
          inputLineNumber,
          fCurrentMeasureNumber);
  }

  return partHarmoniesVoice;
}

S_msrVoice mxsr2msrSkeletonBuilder::createPartFiguredBassVoiceIfNotYetDone (
  int        inputLineNumber,
  S_msrPart  part)
{
  // is the figured bass voice already present in part?
  S_msrVoice
    partFiguredBassVoice =
      part->
        getPartFiguredBassVoice ();

  if (! partFiguredBassVoice) {
    // create the figured bass voice and append it to the part
    partFiguredBassVoice =
      part->
        createPartFiguredBassVoice (
          inputLineNumber,
          fCurrentMeasureNumber);
  }

  return partFiguredBassVoice;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_score_partwise& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_score_partwise" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Analysing the score partwise" <<
      endl;
  }
#endif

  fScoreNumberOfMeasures = 0;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_score_partwise& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_score_partwise" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // register the number of measures
  fMsrScore->
    setScoreNumberOfMeasures (
      fScoreNumberOfMeasures);

/* JMI ??? THROW AWAY ???
  // fetch the identification from the credits if any
  fMsrScore ->
    fetchIdentificationFromCreditsIfAny (
      inputLineNumber);
      */
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart ( S_work_number& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_work_number" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMsrScore->getIdentification () ->
    setIdentificationWorkNumber (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart ( S_work_title& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_work_title" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string workTitle = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationWorkTitle (
      inputLineNumber,
      workTitle);
}

void mxsr2msrSkeletonBuilder::visitStart ( S_opus& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_opus" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string opus = elt->getValue ();

  fMsrScore->getIdentification () ->
    setIdentificationOpus (
      inputLineNumber,
      opus);
}

void mxsr2msrSkeletonBuilder::visitStart ( S_movement_number& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_movement_number" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMsrScore->getIdentification () ->
    setIdentificationMovementNumber (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart ( S_movement_title& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_movement_title" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string movementTitle = elt->getValue ();

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
void mxsr2msrSkeletonBuilder::visitStart ( S_identification& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_identification" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void mxsr2msrSkeletonBuilder::visitEnd ( S_identification& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_identification" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void mxsr2msrSkeletonBuilder::visitStart ( S_creator& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_creator" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string creatorType = elt->getAttributeValue ("type");
  string creatorValue = elt->getValue ();

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
    stringstream s;

    s <<
      "creator type \"" << creatorType <<
      "\" is unknown";

// JMI    musicxmlError (
    musicxmlWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
//      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrSkeletonBuilder::visitStart ( S_rights& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_rights" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      inputLineNumber,
      rightsValue);
}

void mxsr2msrSkeletonBuilder::visitStart ( S_source& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_source" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  string rightsValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (rightsValue); // JMI &#x00a9;

  fMsrScore->getIdentification () ->
    appendRight (
      inputLineNumber,
      rightsValue);
      */
}

void mxsr2msrSkeletonBuilder::visitStart ( S_relation& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_relation" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  string rightsValue = elt->getValue ();

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_encoding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "*** Analysing S_encoding ***" <<
      ", " << elt->getValue () <<
      endl;
  }
#endif

  fScoreNumberOfMeasures = 0;
}

void mxsr2msrSkeletonBuilder::visitEnd ( S_encoding& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_encoding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void mxsr2msrSkeletonBuilder::visitStart ( S_encoding_date& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_encoding_date" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
}

void mxsr2msrSkeletonBuilder::visitStart ( S_encoder& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_encoder" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart ( S_software& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_software" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string softwareValue = elt->getValue ();

  // convert clef to lower case for analysis
  string softwareValueToLower = mfStringToLowerCase (softwareValue);

  if (softwareValueToLower.find ("cubase") != string::npos) {
    musicxmlWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      "<software /> contains 'Cubase'");

    // is the '-cubase' option set???
    if (gGlobalMxsr2msrOahGroup->getCubase ()) {
      S_oahElement
        cubaseOption =
          gGlobalMxsrOahGroup->
            getHandlerUpLink ()->
              fetchNameInNamesToElementsMap ("cubase");

      if (
        // combined booleans atom?
        S_oahCombinedBooleansAtom
          combinedBooleanAtom =
            dynamic_cast<oahCombinedBooleansAtom*>(&(*cubaseOption))
      ) {
        // handle it at once
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
          gLogStream <<
            "Setting '-cubase' option" <<
            endl;
        }
#endif

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

void mxsr2msrSkeletonBuilder::visitStart ( S_encoding_description& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_encoding_description" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

void mxsr2msrSkeletonBuilder::visitStart ( S_supports& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_supports" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  fMsrScore->getIdentification () ->
    setIdentificationEncodingDate (
      inputLineNumber,
      elt->getValue ());
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart ( S_miscellaneous& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_miscellaneous" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  string miscellaneousFielValue = elt->getValue ();

  mfConvertHTMLEntitiesToPlainCharacters (
    miscellaneousFielValue);

  fMsrScore->getIdentification () ->
    setIdentificationMiscellaneousField (
      inputLineNumber,
      miscellaneousFielValue);
      */
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd ( S_miscellaneous& elt )
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_miscellaneous" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart ( S_miscellaneous_field& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_miscellaneous_field" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string miscellaneousFielValue = elt->getValue ();

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

void mxsr2msrSkeletonBuilder::visitStart ( S_credit& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_credit" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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

void mxsr2msrSkeletonBuilder::visitStart ( S_credit_type& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_credit_type" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

	string creditTypeValue = elt->getValue ();

	msrCreditTypeKind
		creditTypeKind =
			msrCreditTypeKind::k_NoCreditType; // default value

  if      (creditTypeValue == "page number")
    creditTypeKind = msrCreditTypeKind::kPageNumber;
  else if (creditTypeValue == "title")
    creditTypeKind =  msrCreditTypeKind::kTitle;
  else if (creditTypeValue == "subtitle")
    creditTypeKind =  msrCreditTypeKind::kSubtitle;
  else if (creditTypeValue == "composer")
    creditTypeKind =  msrCreditTypeKind::kComposer;
  else if (creditTypeValue == "arranger")
    creditTypeKind =  msrCreditTypeKind::kArranger;
  else if (creditTypeValue == "lyricist")
    creditTypeKind =  msrCreditTypeKind::kLyricist;
  else if (creditTypeValue == "rights")
    creditTypeKind =  msrCreditTypeKind::kRights;
  else if (creditTypeValue == "part name")
    creditTypeKind =  msrCreditTypeKind::kPartName;
  else {
  	creditTypeKind =  msrCreditTypeKind::kOtherCreditType;
  }

	// set the current credit type kind
	fCurrentCredit->
		setCreditTypeKind (creditTypeKind);
}

void mxsr2msrSkeletonBuilder::visitStart ( S_credit_symbol& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_credit_symbol" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void mxsr2msrSkeletonBuilder::visitStart ( S_credit_image& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_credit_image" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void mxsr2msrSkeletonBuilder::visitStart ( S_credit_words& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_credit_words" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
    <credit-words default-x="51" default-y="1253" font-family="FreeSerif" font-size="12" valign="bottom" xml:lang="fr">arrangement and lyrics by Andr??s Rojas</credit-words>
  </credit>
  */

  string creditWordsContents =
    elt->getValue ();

  // position
  float creditWordsDefaultX=
    elt->getAttributeFloatValue ("default-x", 0.0);
  float creditWordsDefaultY=
    elt->getAttributeFloatValue ("default-y", 0.0);

  // font family
  string creditWordsFontFamily =
    elt->getAttributeValue ("font-family");

  // font size
  float creditWordsFontSize =
    elt->getAttributeFloatValue ("font-size", 0.0);

  // font weight
  string creditWordsFontWeightString =
    elt->getAttributeValue ("font-weight"); // JMI etc

  msrFontWeightKind
    creditWordsFontWeightKind =
      msrFontWeightKindFromString (
        inputLineNumber,
        creditWordsFontWeightString);

  // font style
  string creditWordsFontStyleString =
    elt->getAttributeValue ("font-style");

  msrFontStyleKind
    creditWordsFontStyleKind =
      msrFontStyleKindFromString (
        inputLineNumber,
        creditWordsFontStyleString);

  // justify
  string creditWordsJustifyString =
    elt->getAttributeValue ("justify");

  msrJustifyKind
    creditWordsJustifyKind =
      msrJustifyKindFromString (
        inputLineNumber,
        creditWordsJustifyString);

  // halign
  string creditWordsHAlignString =
    elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    creditWordsHorizontalAlignment =
      msrHorizontalAlignmentKindFromString (
        inputLineNumber,
        creditWordsHAlignString);

  // valign
  string creditWordsVAlignString =
    elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    creditWordsVerticalAlignment =
      msrVerticalAlignmentKindFromString (
        inputLineNumber,
        creditWordsVAlignString);

  // XMLLang
  string creditWordsXMLLangString =
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

void mxsr2msrSkeletonBuilder::visitEnd ( S_credit& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_credit" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

	// forget about the current credit
  fCurrentCredit = nullptr;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_list& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_list" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Analysing part list" <<
      endl;
  }
#endif

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_list& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part_list" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

  // set implicit part group descr end position
  fImplicitPartGroupDescr->
    setStopPosition (
      inputLineNumber, // JMI ???
      INT_MAX);

  // register implicit part group descr as stopped
  registerPartGroupDescrAsStopped (
    inputLineNumber,
    fImplicitPartGroupDescr);

  // do the job...
  doPartGroupsNestingAndPartsAllocation (
    inputLineNumber);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    showPartGroupsData (
      inputLineNumber,
      "Part groups data gathered for score skeleton");
  }
#endif

  if (gGlobalMsrOahGroup->getDisplayPartGroups ()) {
    gLogStream <<
      endl <<
      "The implicit part group contains:" <<
      endl;

    ++gIndenter;

    fImplicitPartGroup->
      printPartGroupElementsList (
        inputLineNumber,
        gLogStream);

#ifdef TRACING_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
      gLogStream << endl;
    }
#endif

    --gIndenter;
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part_group& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_group" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // part group number

  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);

  // part group type

  string partGroupType = elt->getAttributeValue ("type");

  fCurrentPartGroupTypeKind = msrPartGroup::kPartGroupTypeNone;

  if      (partGroupType == "start")
    fCurrentPartGroupTypeKind = msrPartGroup::kPartGroupTypeStart;
  else if (partGroupType == "stop")
    fCurrentPartGroupTypeKind = msrPartGroup::kPartGroupTypeStop;
  else {
    stringstream s;

    s <<
      "unknown part group type \"" <<
      partGroupType <<
      "\"";

    musicxmlError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // part group print style

  string printStyle = elt->getAttributeValue ("print-style"); // JMI

  // part group print object

  string printObject = elt->getAttributeValue ("print-object"); // JMI

  // handle part group type

  switch (fCurrentPartGroupTypeKind) {
    case msrPartGroup::kPartGroupTypeNone:
      // should not occur
      break;

    case msrPartGroup::kPartGroupTypeStart:
      fCurrentPartGroupName = "";
      fCurrentPartGroupNameDisplayText = "";
      fCurrentPartGroupAccidentalText = "";
      fCurrentPartGroupAbbreviation = "";
      fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolNone;
      fCurrentPartGroupSymbolDefaultX = INT_MIN;
      fCurrentPartGroupBarLineKind = msrPartGroup::kPartGroupBarLineYes;
      break;

    case msrPartGroup::kPartGroupTypeStop:
      break;
  } // switch
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_name& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_group_name" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPartGroupName = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_name_display& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_group_name_display" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // print-object

  string
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_group_name_display" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPartGroupNameDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_display_text& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_display_text" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<display-text /> is out of context");
  }
}

void mxsr2msrSkeletonBuilder::visitStart (S_accidental_text& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accidental_text" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_abbreviation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_group_abbreviation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void mxsr2msrSkeletonBuilder::visitStart (S_group_symbol& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_group_symbol" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string groupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolKind =
    msrPartGroup::kPartGroupSymbolNone; // default value

  if      (groupSymbol == "brace")
    fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolBrace;

  else if (groupSymbol == "bracket")
    fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolBracket;

  else if (groupSymbol == "line")
    fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolLine;

  else if (groupSymbol == "square")
    fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolSquare;

  else if (groupSymbol == "none")
    fCurrentPartGroupSymbolKind = msrPartGroup::kPartGroupSymbolNone;

  else {
    if (groupSymbol.size ()) {
      // part group type may be absent
      stringstream s;

      s <<
        "unknown part group symbol \"" + groupSymbol + "\"";

      musicxmlError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxsr2msrSkeletonBuilder::visitStart ( S_group_barline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_group_barline" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string groupBarLine = elt->getValue ();

  // check part group barLine
  if      (groupBarLine == "yes")
    fCurrentPartGroupBarLineKind = msrPartGroup::kPartGroupBarLineYes;
  else if (groupBarLine == "no")
    fCurrentPartGroupBarLineKind = msrPartGroup::kPartGroupBarLineNo;
  else {
    stringstream s;

    s <<
      "unknown part group barLine \"" + groupBarLine + "\"";

    musicxmlError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitEnd (S_part_group& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part_group" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Handling part group '" <<
      fCurrentPartGroupNumber <<
      "', type: \"" <<
      msrPartGroup::partGroupTypeKindAsString (
        fCurrentPartGroupTypeKind) << "\""  <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // handle part group type
  switch (fCurrentPartGroupTypeKind) {

    case msrPartGroup::kPartGroupTypeStart:
      // handle the part group start
      handlePartGroupStart (
        inputLineNumber);
      break;

    case msrPartGroup::kPartGroupTypeStop:
      // handle the part group stop
      handlePartGroupStop (
        inputLineNumber);
      break;

    case msrPartGroup::kPartGroupTypeNone:
      // should not occur
      break;
  } // switch

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_score_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_score_part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentPartID = elt->getAttributeValue ("id");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Found part name \"" << fCurrentPartID << "\"" <<
      " in part list" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // is the part id a pure number?
  string regularExpression (
    "[[:space:]]*"
    "[[:digit:]]+"
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (fCurrentPartID, sm, e);

  if (sm.size () == 1) {
    stringstream s;

    s <<
      "Part name \"" << fCurrentPartID << "\"" <<
      " is a pure number" <<
      ", line " << inputLineNumber;

    musicxmlWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
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
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_name" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentPartName = elt->getValue ();

  string printStyle = elt->getAttributeValue ("print-style"); // JMI
  if (printStyle == "JMI") {
  }

  // print-object

  string
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_name_display" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPartNameDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_name_display& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part_name_display" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPartNameDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_abbreviation& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_abbreviation" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentPartAbbreviation = elt->getValue ();

  // print-object

  string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

void mxsr2msrSkeletonBuilder::visitStart (S_part_abbreviation_display& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPartAbbreviationDisplay = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part_abbreviation_display& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part_abbreviation_display" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPartAbbreviationDisplay = false;
}

void mxsr2msrSkeletonBuilder::visitStart (S_instrument_name& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPartInstrumentName = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitStart (S_instrument_abbreviation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instrument_abbreviation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void mxsr2msrSkeletonBuilder::visitEnd (S_score_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_score_part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++fCurrentPartsPosition;

  string partID = elt->getAttributeValue ("id");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
   gLogStream <<
    "--------------------------------------------" <<
    endl <<
    "Handling score part name \"" << fCurrentPartID << "\"" <<
    ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
    ", line " << inputLineNumber <<
    endl;
  }
#endif

  ++gIndenter;

  // create the part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
   gLogStream <<
    "--------------------------------------------" <<
    endl <<
    "Creating part \"" << fCurrentPartID << "\"" <<
    ", line " << inputLineNumber <<
    endl;
  }
#endif

  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        fCurrentPartID,
        0); // partPartGroupUpLink will be set later

  // populate it
  part->
    setPartName (
      fCurrentPartName);
  part->
    setPartNameDisplayText (
      fCurrentPartNameDisplayText);

  part->
    setPartAbbreviation (
      fCurrentPartAbbreviation);
  part->
    setPartAbbreviationDisplayText (
      fCurrentPartAbbreviationDisplayText);

  part->
    setPartInstrumentName (
      fCurrentPartInstrumentName);
  part->
    setPartInstrumentAbbreviation (
      fCurrentPartInstrumentAbbreviation);

  // register part in the parts data
  registerPart (
    inputLineNumber,
    fCurrentPartsPosition,
    part);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handling score part \"" + partID + "\"");
  }
#endif

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::registerPart (
  int       inputLineNumber,
  int       partPosition,
  S_msrPart part)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Registering part " <<
      part->getPartCombinedName () <<
      " in the parts data" <<
      ", partPosition = " << partPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // register part in the parts vector
  fPartsVector.push_back (part);

  // register it in the parts map
  fPartsMap [part->getPartID ()] = part;

  // create an empty list for part groups starting at partPosition
  fPositionStartingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create an empty list for part groups stopping at partPosition
  fPositionStoppingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceParts ()
      ||
    gGlobalOahEarlyOptions.getEarlyTracePasses ()
  ) {
    gLogStream <<
      endl <<
      "<!--=== part \"" << fCurrentPartID << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  if (! fCurrentPartID.size ()) {
    if (fPartsMap.size () == 1) {
      // there's only one part in the part list,
      // assume this is the one
      fCurrentPartID =
        (*fPartsMap.begin ()).second->
          getPartID ();

      stringstream s;

      s <<
        "part id is empty, using '" <<
        fCurrentPartID <<
        "' since it is the only part in the <part-list />";

      musicxmlWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }

    else {
      ++fCurrentNoIDPartNumber;

      stringstream s;

      s <<
        "NO_ID_PART_" << fCurrentNoIDPartNumber;

      s >> fCurrentPartID;

      musicxmlWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        "part 'id' is empty, creating one as '" + fCurrentPartID + "'");
    }
  }

  // is this part already known?
  if (fPartsMap.count (fCurrentPartID)) {
    fCurrentPart = // used thoughoutfRepeatHasBeenCreatedForCurrentPart
      fPartsMap [fCurrentPartID];
  }

  else {
    stringstream s;

    s <<
      "part \"" <<
      fCurrentPartID <<
      "\" is not known in the <part-list />";

    musicxmlError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;
  }
#endif

  // measures
  fPartNumberOfMeasures = 0;

  // staves and voices
  fCurrentStaffMusicXMLNumber = 1; // default if there are no <staff> element
  fCurrentVoiceMusicXMLNumber = 1; // default if there are no <voice> element
}

void mxsr2msrSkeletonBuilder::visitEnd (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
      endl <<
      "--------------------------------------------" <<
      endl;
  }
#endif

  --gIndenter;

  // handle current part number of measures
  if (fScoreNumberOfMeasures == 0) {
    fScoreNumberOfMeasures =
      fPartNumberOfMeasures;
  }
  else {
    // is the part number of measures consistent?
    if (fPartNumberOfMeasures != fScoreNumberOfMeasures) {
      stringstream s;

      s <<
        "part " << fCurrentPart->getPartCombinedName () <<
        " has " << fPartNumberOfMeasures <<
        " measures while the other ones have " << fScoreNumberOfMeasures;

      musicxmlError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
      __FILE__, __LINE__,
        s.str ());
    }
  }

  // set current part's number of measures
  fCurrentPart->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

/* JMI
  // are there more staves in <staves/> that specified with <stave/>?
  fCurrentPart->
    addAVoiceToStavesThatHaveNone (
      inputLineNumber);
*/

  // assign sequential numbers to the voices in the part
  fCurrentPart->
    assignSequentialNumbersToRegularVoicesInPart (
      inputLineNumber);
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_staves& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  int stavesNumber = int(*elt);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    switch (stavesNumber) {
      case 0:
        gLogStream <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;

      case 1:
        gLogStream <<
          "There is 1 staff";
        break;

      default:
        gLogStream <<
          "There are " << stavesNumber << " staves";
    } // switch

    gLogStream <<
      " in part " << fCurrentPart->getPartCombinedName() <<
      endl;
  }
#endif

  if (stavesNumber > 1) {
    // add stavesNumber staves to current part
    int n = 1;

    while (n <= stavesNumber) {
    /* JMI
      fCurrentPart->
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
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentStaffMusicXMLNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentStaffMusicXMLNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentStaffMusicXMLNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssert (
        __FILE__, __LINE__,
      false, s.str ());
  }

  if (fOnGoingNote) { // JMI
    // regular staff indication in note/rest, fine
  }
  else {
    // JMI ???
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_voice& elt )
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_voice" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceMusicXMLNumber = int(*elt);

  if (fOnGoingNote) { // JMI
    // regular voice indication in note/rest, fine
  }
  else {
    // JMI ???
  }

  // don't attempt to create the voice now,
  // it's staff number if any comes later!
  // do it upton visitEnd ( S_note& )
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_measure& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  fCurrentMeasureNumber = // JMI local variable???
    elt->getAttributeValue ("number");

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalOahEarlyOptions.getEarlyTracePasses ()
  ) {
    gLogStream <<
      endl <<
      "<!--=== measure \"" << fCurrentMeasureNumber << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // take this measure into account
  ++fPartNumberOfMeasures;

  ++gIndenter;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_measure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_measure" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  --gIndenter;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_print& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_print" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fOnGoingPrint = true;
}

void mxsr2msrSkeletonBuilder::visitEnd (S_print& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_print" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingPrint = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart ( S_note& elt )
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffMusicXMLNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceMusicXMLNumber = 1;

  // lyrics
  fCurrentStanzaNumber = msrStanza::K_NO_STANZA_NUMBER;
  fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME;

  fOnGoingNote = true;
}

void mxsr2msrSkeletonBuilder::visitEnd ( S_note& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_note" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "--> S_note, fCurrentStaffMusicXMLNumber = " <<
      fCurrentStaffMusicXMLNumber <<
      endl <<
      "--> S_note, current staff name  = " <<
      staff->getStaffName() <<
      endl <<
      "--> S_note, fCurrentVoiceMusicXMLNumber        = " <<
      fCurrentVoiceMusicXMLNumber <<
      endl <<
      "--> S_note, current noteVoice name  = " <<
      noteVoice->getVoiceName() <<
      endl;
  }
#endif

  // are there harmonies attached to the current note?
  if (fThereAreHarmoniesToBeAttachedToCurrentNote) {
    if (gGlobalMxsr2msrOahGroup->getIgnoreHarmonies ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Ignoring the harmonies" <<
          ", line " <<
          inputLineNumber <<
          endl;
      }
#endif
    }
    else {
      // create the part harmonies voice if not yet done
      S_msrVoice
        partHarmoniesVoice =
          createPartHarmoniesVoiceIfNotYetDone (
            inputLineNumber,
            fCurrentPart);
    }

    fThereAreHarmoniesToBeAttachedToCurrentNote = false;
  }

  // are there figured bass attached to the current note?
  if (fThereAreFiguredBassToBeAttachedToCurrentNote) {
    if (gGlobalMxsr2msrOahGroup->getIgnoreFiguredBassElements ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
        gLogStream <<
          "Ignoring the figured bass elements" <<
          ", line " <<
          inputLineNumber <<
          endl;
      }
#endif
    }
    else {
      // create the part figured bass voice if not yet done
      S_msrVoice
        partFiguredBassVoice =
          createPartFiguredBassVoiceIfNotYetDone (
            inputLineNumber,
            fCurrentPart);
    }

    fThereAreFiguredBassToBeAttachedToCurrentNote = false;
  }

  fOnGoingNote = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart (S_lyric& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_lyric" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.size () == 0) {
      musicxmlWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        "lyric number is empty, using \"1\" by default");

      fCurrentStanzaNumber = "1";
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLyrics ()) {
        gLogStream <<
          "--> setting fCurrentStanzaNumber to " <<
          fCurrentStanzaNumber <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

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
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLyrics ()) {
        // lyrics names are not so frequent after all...
        stringstream s;

        s <<
          "lyric name is empty, using \"" <<
          msrStanza::K_NO_STANZA_NAME <<
          "\" by default";

        musicxmlWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          s.str ());
      }
#endif

      fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME;
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLyrics ()) {
        gLogStream <<
          "--> setting fCurrentStanzaName to " <<
          fCurrentStanzaName <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      // register it as current stanza name, JMI
      // that remains set another positive value is found,
      // thus allowing a skip syllable to be generated
      // for notes without lyrics
    }
  }
}

void mxsr2msrSkeletonBuilder::visitEnd ( S_lyric& elt )
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_lyric" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      endl <<
      "visitEnd ( S_lyric& )" <<
      ", line = " << inputLineNumber <<
      ", with:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "Lyric data:" <<
      endl;

    {
      ++gIndenter;

      const int fieldWidth = 28;

      gLogStream << left <<
        setw (fieldWidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl;

      --gIndenter;
    }

    --gIndenter;
  }
#endif

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
void mxsr2msrSkeletonBuilder::visitStart ( S_harmony& elt )
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_harmony" <<
      ", harmoniesVoicesCounter = " << fHarmoniesVoicesCounter <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  /* JMI ???
    several harmonies can be attached to a given note,
    leading to as many harmonies voices in the current part
  */

  // take harmonies voice into account
  ++fHarmoniesVoicesCounter; // UNUSED JMI

  fThereAreHarmoniesToBeAttachedToCurrentNote = true;
}

//______________________________________________________________________________
void mxsr2msrSkeletonBuilder::visitStart ( S_figured_bass& elt )
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_figured_bass" <<
      ", figuredBassVoicesCounter = " << fFiguredBassVoicesCounter <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  /* JMI
    several figured bass elements can be attached to a given note,
    leading to as many figured bass voices in the current part JMI TRUE???
  */

  // take figured bass voice into account
  ++fFiguredBassVoicesCounter;

  fThereAreFiguredBassToBeAttachedToCurrentNote = true;
}


} // namespace
