/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <algorithm>    // for_each

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msr2msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msr2msrComponent.h"

#include "msr2msrTranslator.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

// JMI TEMP TEST
#include "msrPathToVoice.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLineDescr msrHiddenMeasureAndBarLineDescr::create (
  int           inputLineNumber,
  S_msrDalSegno dalSegno)
{
  msrHiddenMeasureAndBarLineDescr* o = new
    msrHiddenMeasureAndBarLineDescr (
      inputLineNumber,
      dalSegno);
  assert (o != nullptr);
  return o;
}

msrHiddenMeasureAndBarLineDescr::msrHiddenMeasureAndBarLineDescr (
  int           inputLineNumber,
  S_msrDalSegno dalSegno)
{
  fInputLineNumber = inputLineNumber;
  fDalSegno        = dalSegno;
}

msrHiddenMeasureAndBarLineDescr::~msrHiddenMeasureAndBarLineDescr ()
{}

string msrHiddenMeasureAndBarLineDescr::hiddenMeasureAndBarLineDescrAsString () const
{
  stringstream s;

  s <<
    "hiddenMeasureAndBarLineDescr" <<
    ", inputLineNumber: " << fInputLineNumber <<
    ", dalSegno: " << fDalSegno->asString ();

  return s.str ();
}

void msrHiddenMeasureAndBarLineDescr::print (ostream& os) const
{
  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "inputLineNumber" << " : " <<
    fInputLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "dalSegno" << " : " <<
    fDalSegno->asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrHiddenMeasureAndBarLineDescr& elt)
{
  elt->print (os);
  return os;
}

//________________________________________________________________________
msr2msrTranslator::msr2msrTranslator ()
{};

msr2msrTranslator::~msr2msrTranslator ()
{}

//________________________________________________________________________
S_msrScore msr2msrTranslator::translateMsrToMsr (
  S_msrScore theMsrScore)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create the resulting MSR score
  fResultingNewMsrScore =
    msrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      "msrScore::create()");

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // browse the visited score with the browser
  browser.browse (*fVisitedMsrScore);

  // forget about the visited MSR score
  fVisitedMsrScore = nullptr;

  return fResultingNewMsrScore;
}

//________________________________________________________________________
S_msrScore msr2msrTranslator::translateMsrToMsrAlongPathToVoice (
  S_msrScore       theMsrScore,
  S_msrPathToVoice pathToVoice)
{
  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  mfAssert (
    __FILE__, __LINE__,
    pathToVoice != nullptr,
    "pathToVoice is null");

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create the resulting MSR score
  fResultingNewMsrScore =
    msrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      "msrScore::create()");

  // create a msrScore browser
  msrBrowserAlongPathToVoice<msrScore> browser (
    this,
    pathToVoice);

  // browse the visited score with the browser
  browser.browse (*fVisitedMsrScore);

  // forget about the visited MSR score
  fVisitedMsrScore = nullptr;

  return fResultingNewMsrScore;
}

//________________________________________________________________________
void msr2msrTranslator::displayCurrentOnGoingValues ()
{
  gLogStream <<
    "Current ongoing values:" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLogStream << left <<
    setw (fieldWidth) <<
    "fOnGoingIdentification" << ": " << fOnGoingIdentification <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingStaff" << ": " << fOnGoingStaff <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingHarmoniesVoice" << ": " << fOnGoingHarmoniesVoice <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingHarmony" << ": " << fOnGoingHarmony <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingFiguredBassVoice" << ": " << fOnGoingFiguredBassVoice <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingNotesStack.size ()" << ": " << fOnGoingNotesStack.size () <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingNonGraceNote" << ": " << fOnGoingNonGraceNote <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingDoubleTremolo" << ": " << fOnGoingDoubleTremolo <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingGraceNotesGroup" << ": " << fOnGoingGraceNotesGroup <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingChord" << ": " << fOnGoingChord <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingStanza" << ": " << fOnGoingStanza <<
    endl<<
    setw (fieldWidth) <<
    "fOnGoingSyllableExtend" << ": " << fOnGoingSyllableExtend <<
    endl;

  --gIndenter;
}

//________________________________________________________________________
void msr2msrTranslator::displayPartHiddenMeasureAndBarLineDescrList ()
{
  gLogStream <<
    "fPartHiddenMeasureAndBarLineDescrList:" <<
    endl;

  if (fPartHiddenMeasureAndBarLineDescrList.size ()) {
    ++gIndenter;

    const int fieldWidth = 19;

    list<S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is null");

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarLineDescr->getDalSegno ();

      gLogStream << left <<
        setw (fieldWidth) <<
        "inputLineNumber" << " : " <<
        hiddenMeasureAndBarLineDescr->getInputLineNumber () <<
        endl <<
        setw (fieldWidth) <<
        "dalSegno" << " : " <<
        dalSegno <<
        endl;

      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << "empty" << endl;
  }
}

//________________________________________________________________________
void msr2msrTranslator::displayOnGoingNotesStack (
  const string& context)
{
  size_t onGoingNotesStackSize = fOnGoingNotesStack.size ();

  gLogStream <<
    endl <<
    ">>++++++++++++++++ " <<
    "The on-going notes stack contains " <<
    onGoingNotesStackSize <<
    " elements" <<
    " (" << context << "):" <<
    endl;

  if (onGoingNotesStackSize) {
    list<S_msrNote>::const_iterator
      iBegin = fOnGoingNotesStack.begin (),
      iEnd   = fOnGoingNotesStack.end (),
      i      = iBegin;

    S_msrNote note = (*i);

    ++gIndenter;

    int n = onGoingNotesStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        endl;

      ++gIndenter;
      note->printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

//________________________________________________________________________
void msr2msrTranslator::handlePartHiddenMeasureAndBarLineDescrList ()
{
  gLogStream <<
    "fPartHiddenMeasureAndBarLineDescrList:" <<
    endl;

  if (fPartHiddenMeasureAndBarLineDescrList.size ()) {
    list<S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is null");

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarLineDescr->getDalSegno ();

      fCurrentPartClone->
        insertHiddenMeasureAndBarLineInPartClone (
          hiddenMeasureAndBarLineDescr->getInputLineNumber (),
          dalSegno->getMeasureElementPositionInMeasure ());

      if (++i == iEnd) break;
    } // for
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

void msr2msrTranslator::visitEnd (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

  // set the current identification
  fCurrentIdentification = elt;

  // store it in the resulting MSR score
  fResultingNewMsrScore->
    setIdentification (
      fCurrentIdentification);

  fOnGoingIdentification = true;
}

void msr2msrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

  // create a scaling clone
  S_msrScaling
    scalingClone =
      elt->createMsrScalingNewbornClone ();

  // register it in the current MSR score clone
  fResultingNewMsrScore-> // JMI BLARK ???
    setScaling (
      scalingClone);
}

void msr2msrTranslator::visitEnd (S_msrScaling& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the aystem layout in the current MSR score clone
  fResultingNewMsrScore->
    setSystemLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the staff layout in the current MSR score clone
  fResultingNewMsrScore->
    setStaffLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the appearance in the current MSR score clone
  fResultingNewMsrScore->
    setAppearance (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // register the page layout in the current MSR score clone
  fResultingNewMsrScore->
    setPageLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the MSR score
  fResultingNewMsrScore->
    appendCreditToScore (fCurrentCredit);
}

void msr2msrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentCredit = nullptr;
}

void msr2msrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't append it to the current credit, since the latter is no clone
  /* JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      */
}

void msr2msrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a partGroup clone
  // current partGroup clone, i.e. the top of the stack,
  // is the upLink of the new one if it exists
  S_msrPartGroup
    partGroupClone =
      elt->createPartGroupNewbornClone (
        fPartGroupsStack.size ()
          ? fPartGroupsStack.top ()
          : nullptr,
        fResultingNewMsrScore);

  // push it onto this visitors's stack,
  // making it the current partGroup
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Pushing part group clone " <<
      partGroupClone->getPartGroupCombinedName () <<
      " onto stack" <<
      endl;
  }
#endif

  fPartGroupsStack.push (
    partGroupClone);

/* JMI
  // add it to the MSR score clone
  fResultingNewMsrScore->
    addPartGroupToScore (fCurrentPartGroupClone);
*/
}

void msr2msrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();

  if (fPartGroupsStack.size () == 1) {
    // add the current partgroup clone to the MSR score clone
    // if it is the top-level one, i.e it's alone in the stack

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Adding part group clone " <<
        currentPartGroup->getPartGroupCombinedName () <<
        " to MSR score" <<
        endl;
    }
#endif

    fResultingNewMsrScore->
      addPartGroupToScore (currentPartGroup);

    fPartGroupsStack.pop ();
  }

  else {

    // pop current partGroup from this visitors's stack
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePartGroups ()) {
      gLogStream <<
        "Popping part group clone " <<
        fPartGroupsStack.top ()->getPartGroupCombinedName () <<
        " from stack" <<
        endl;
    }
#endif

    fPartGroupsStack.pop ();

    // append the current part group to the one one level higher,
    // i.e. the new current part group
    fPartGroupsStack.top ()->
      appendSubPartGroupToPartGroup (
        currentPartGroup);
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    partCombinedName =
      elt->getPartCombinedName ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  ++gIndenter;

  // create a part clone
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.top ());

  // add it to the partGroup clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Adding part clone " <<
      fCurrentPartClone->getPartCombinedName () <<
      " to part group clone \"" <<
      fPartGroupsStack.top ()->getPartGroupCombinedName () <<
      "\"" <<
      endl;
  }
#endif

  fPartGroupsStack.top ()->
    appendPartToPartGroup (fCurrentPartClone);
}

void msr2msrTranslator::visitEnd (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  string
    partInstrumentAbbreviation =
      fCurrentPartClone->
        getPartInstrumentAbbreviation ();

  // populate part instrument short name if empty and possible
  if (partInstrumentAbbreviation.size () == 0) {
    string
      partAbbreviation =
        elt->getPartAbbreviation ();

    fCurrentPartClone->
      setPartInstrumentAbbreviation (
        partAbbreviation);
  }

  // finalize the current part clone
  fCurrentPartClone->
    finalizePartClone (
      inputLineNumber);

  // handle the hidden measure and barLine elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    displayPartHiddenMeasureAndBarLineDescrList ();
  }
#endif

  // JMI handlePartHiddenMeasureAndBarLineDescrList ();

  // handle skip grace notes if needed
  if (fCurrentSkipGraceNotesGroup) {
    // add it ahead of the other voices in the part if needed
    fCurrentPartClone->
      addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded ( // JMI only if there's more than one voice???
        fCurrentVoiceClone,
        fCurrentSkipGraceNotesGroup);

    // forget about this skip grace notes group
    fCurrentSkipGraceNotesGroup = nullptr;
  }
}

//________________________________________________________________________
/* JMI
void msr2msrTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLinesNumber" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create a staff lines number clone
  fCurrentStaffLinesNumberClone =
    elt->
      createStaffLinesNumberNewbornClone ();
}
*/

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a staff tuning clone
  fCurrentStaffTuningClone =
    elt->
      createStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentStaffTuningClone = nullptr;
}

void msr2msrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the staff details to the current voice clone
  fCurrentVoiceClone->
    appendStaffDetailsToVoice (
      elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

  switch (elt->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindHarmonies:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // register it as the part figured bass staff
        fCurrentPartClone->
          setPartFiguredBassStaff (fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;
  } // switch
}

void msr2msrTranslator::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      {
        fOnGoingStaff = false;
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      // JMI
      break;

    case msrStaffKind::kStaffKindHarmonies:
      // JMI
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      // JMI
      break;
  } // switch
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrVoice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceOriginal = elt;

  ++gIndenter;

  string voiceName = elt->getVoiceName ();

  // create a voice clone
  fCurrentVoiceClone =
    fCurrentVoiceOriginal->
      createVoiceNewbornClone (
        fCurrentStaffClone);

  // add it to the staff clone
  fCurrentStaffClone->
    registerVoiceInStaffClone (
      inputLineNumber,
      fCurrentVoiceClone);

  switch (fCurrentVoiceOriginal->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      {
        // register it as the part harmonies voice
        fCurrentPartClone->
          setPartHarmoniesVoice (
            fCurrentVoiceClone);

        if (
          fCurrentVoiceOriginal->getMusicHasBeenInsertedInVoice () // superfluous test ??? JMI
          ) {
          // create a ChordNames context
          string partCombinedName =
            fCurrentVoiceOriginal->
              fetchVoicePartUpLink ()->
                getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Creating a ChordNames context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          fOnGoingHarmoniesVoice = true;
        }
      }
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // register it as the part figured bass voice
        fCurrentPartClone->
          setPartFiguredBassVoice (
            fCurrentVoiceClone);

        if (
          fCurrentVoiceOriginal->getMusicHasBeenInsertedInVoice () // superfluous test ??? JMI
          ) {
          // create a FiguredBass context
          string partCombinedName =
            fCurrentVoiceOriginal->
              fetchVoicePartUpLink ()->
                getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
            gLogStream <<
              "Creating a FiguredBass context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              endl;
          }
#endif

          fOnGoingFiguredBassVoice = true;
        }
      }
      break;
  } // switch

  // clear the voice notes map
  fVoiceNotesMap.clear ();

  fFirstNoteCloneInVoice = nullptr;
}

void msr2msrTranslator::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // JMI
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fOnGoingHarmoniesVoice = false;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a voice staff change clone
  S_msrVoiceStaffChange
    voiceStaffChangeClone =
      elt->
        createStaffChangeNewbornClone ();

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendVoiceStaffChangeToVoice (
      voiceStaffChangeClone);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSegment& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a clone of the segment
  fCurrentSegmentClone =
    elt->createSegmentNewbornClone (
      fCurrentVoiceClone);

  // set it as the new voice last segment
  fCurrentVoiceClone->
    setVoiceLastSegmentInVoiceClone (
      fCurrentSegmentClone);
}

void msr2msrTranslator::visitEnd (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleSegmentCloneEndInVoiceClone (
      inputLineNumber,
      fCurrentSegmentClone);

  // forget current segment clone
  fCurrentSegmentClone = nullptr;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->
      createHarmonyNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNoteHarmoniesList (
        fCurrentHarmonyClone);

    // don't append the harmony to the part harmony,
    // this has been done in pass2b // JMI ???
  }

  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      appendHarmonyToChord (
        fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmoniesVoice) {
  /* JMI
    // get the harmony whole notes offset
    rational
      harmonyWholeNotesOffset =
        elt->getHarmonyWholeNotesOffset ();

    // is harmonyWholeNotesOffset not equal to 0?
    if (harmonyWholeNotesOffset.getNumerator () != 0) {
      // create skip with duration harmonyWholeNotesOffset
      S_msrNote
        skip =
          msrNote::createSkipNote (
            elt->                getInputLineNumber (),
            "666", // JMI elt->                getHarmoniesMeasureNumber (),
            elt->                getHarmonyDisplayWholeNotes (), // would be 0/1 otherwise JMI
            elt->                getHarmonyDisplayWholeNotes (),
            0, // JMI elt->                getHarmonyDotsNumber (),
            fCurrentVoiceClone-> getRegularVoiceStaffSequentialNumber (), // JMI
            fCurrentVoiceClone-> getVoiceNumber ());

      // append it to the current voice clone
      // to 'push' the harmony aside
      fCurrentVoiceClone->
        appendNoteToVoice (skip);
    }
*/

    // append the harmony to the current voice clone
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  else {
    stringstream s;

    s <<
      "harmony is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingHarmony = true;
}

void msr2msrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      "', line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentHarmonyClone = nullptr;
  fOnGoingHarmony = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingHarmony) {
    // register the frame in the current non-grace note clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }

  else {
    stringstream s;

    s <<
      "frame is out of context, cannot be handled:'" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFiguredBassElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFiguredBassElement '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingFiguredBassVoice = " << fOnGoingFiguredBassVoice <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a figured bass element new born clone
  fCurrentFiguredBassElementClone =
    elt->
      createFiguredBassElementNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassElementToNoteFiguredBassElementsList (
        fCurrentFiguredBassElementClone);

    // don't append the figured bass to the part figured bass,  JMI ???
    // this will be done below
  }

  /* JMI
  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBassElementClone); // JMI
  }
  */

  else if (fOnGoingFiguredBassVoice) { // JMI
    /*
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassElementToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBassElementClone);
        */
    // append the figured bass to the current voice clone
    fCurrentVoiceClone->
      appendFiguredBassElementToVoiceClone (
        fCurrentFiguredBassElementClone);
  }

  else {
    stringstream s;

    s <<
      "figured bass is out of context, cannot be handled:'" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitStart (S_msrBassFigure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBassFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the bass figure to the current figured bass
  fCurrentFiguredBassElementClone->
    appendFigureToFiguredBass (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrFiguredBassElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFiguredBassElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentFiguredBassElementClone = nullptr;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      endl <<
      "<!--=== measure '" << measureNumber <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClone);

  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes

    // should we compress full measures rests?
//     if (gGlobalMsr2msrOahGroup->getCompressEmptyMeasures ()) {//
//       // yes
//
//       if (! fCurrentRestMeasure) {
//         // this is the first full measure rest in the sequence
//
//         // create a full measure rests  containing fCurrentMeasureClone
//         fCurrentFullMeasureRests =
//           msrFullMeasureRests::create (
//             inputLineNumber,
//             fCurrentMeasureClone,
//             fCurrentVoiceClone);
//
// /* JMI
//         // append the current full measure rests to the current voice clone
//         fCurrentVoiceClone->
//           appendFullMeasureRestsToVoice (
//             inputLineNumber,
//             fCurrentFullMeasureRests);
//             */
//       }
//
//       else {
//         // this is a subsequent full measure rest, merely append it
//         fCurrentFullMeasureRests->
//           appendMeasureCloneToFullMeasureRests (
//             fCurrentMeasureClone);
//       }
//
//       fCurrentRestMeasure = fCurrentMeasureClone;
//     }
//
//     else {
//       // no

      // append current measure clone to the current voice clone
      fCurrentVoiceClone->
        appendMeasureCloneToVoiceClone (
          inputLineNumber,
          fCurrentMeasureClone);
//     }
  }

  else {
    // no

    // append current measure clone to the current voice clone
    fCurrentVoiceClone->
      appendMeasureCloneToVoiceClone (
        inputLineNumber,
        fCurrentMeasureClone);
  }

/* JMI
  // append current measure clone to the current voice clone
  fCurrentVoiceClone->
    appendMeasureCloneToVoiceClone (
      inputLineNumber,
      fCurrentMeasureClone);
*/
  // JMI superflous ???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      measureNumber);

  // should the last bar check's measure purist number be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarPuristNumber (
        measurePuristNumber);

    fLastBarCheck = nullptr;
  }
}

void msr2msrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasure '" <<
      measureNumber <<
      "', nextMeasureNumber = '" <<
      nextMeasureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentMeasureClone->
    finalizeMeasureClone (
      inputLineNumber,
      elt, // original measure
      fCurrentVoiceClone);

  Bool doCreateABarCheck (false);

  switch (elt->getMeasureKind ()) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureVoiceUpLink ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2msrTranslator";

      // JMI  msr2msrInternalError (
        msr2msrInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
  //        __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      // generate a bar check if relevant
      switch (elt-> getMeasureEndRegularKind ()) {
        case msrMeasure::kMeasureEndRegularKindUnknown:
          break;
        case msrMeasure::kMeasureEndRegularKindYes:
          doCreateABarCheck = true;
          break;
        case msrMeasure::kMeasureEndRegularKindNo:
          break;
      } // switch
      break;

    case msrMeasureKind::kMeasureKindOvercomplete:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      // JMI
      break;
  } // switch

  // is this a full measures rest?
  if (elt->getMeasureIsAFullMeasureRest ()) {
    // yes JMI
  }

  else {
    // no

    // should we compress full measures rests?
//    if (gGlobalMsr2msrOahGroup->getCompressEmptyMeasures ()) {//
//       // yes
//
//       if (fCurrentFullMeasureRests) {
//         // append the current full measure rests to the current voice clone
//         fCurrentVoiceClone->
//           appendFullMeasureRestsToVoice (
//             inputLineNumber,
//             fCurrentFullMeasureRests);
//
//         // forget about the current rest measure
//         fCurrentRestMeasure = nullptr;
//
//         // forget about the current full measure rests
//         fCurrentFullMeasureRests = nullptr;
//       }
//
//       else {
//         stringstream s;
//
//         s <<
//           "fCurrentFullMeasureRests is null upon full measure rest end" <<
//           measureNumber <<
//           "', measurePuristNumber = '" <<
//           measurePuristNumber <<
//           "', line " << inputLineNumber;
//
// /* JMI ???
//         msr2msrInternalError (
//           gGlobalServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, __LINE__,
//           s.str ());
//           */
//       }
//     }
  }

  if (doCreateABarCheck) {
    // create a bar check
    int voiceCurrentMeasurePuristNumber =
      fCurrentVoiceClone->
        getVoiceCurrentMeasurePuristNumber ();

    fLastBarCheck =
      msrBarCheck::createWithNextBarPuristNumber (
        inputLineNumber,
        nextMeasureNumber,
        voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (fLastBarCheck);

    // create a bar number check
    // should NOT be done in cadenza, SEE TO IT JMI
    S_msrBarNumberCheck
      barNumberCheck_ =
        msrBarNumberCheck::create (
          inputLineNumber,
          nextMeasureNumber,
          voiceCurrentMeasurePuristNumber);

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarNumberCheckToVoice (barNumberCheck_);
  }

  // should a page break be inserted?
//   gLogStream << // JMI
//     "==> measureNumber = \"" <<
//     measureNumber <<
//     "\"" <<
//     ", line " << inputLineNumber <<
//     endl;

  // is this measureNumber in the parts ignore IDs set?
  if (gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().size ()) {
    set<string>::iterator
      it =
        gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().find (
          measureNumber);

    if (it != gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().end ()) {
      gLogStream <<
        "==> adding a page break after measureNumber = \"" <<
        measureNumber <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;

      S_msrPageBreak
        pageBreak =
          msrPageBreak::create (
            inputLineNumber,
            msrUserChosenPageBreakKind::kUserChosenPageBreakYes);

      fCurrentMeasureClone->
        appendPageBreakToMeasure (
          pageBreak);
    }
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);

    // append the stanza clone to the current voice clone
    fCurrentVoiceClone ->
      addStanzaToVoice (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = msr; // JMI

  fOnGoingStanza = true;
}

void msr2msrTranslator::visitEnd (S_msrStanza& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

  // add it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JMI
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanza (
        fCurrentSyllableClone);
  }

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsNoteUpLink (
        fCurrentNonGraceNoteClone);

//     if (gGlobalMsr2msrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) { JMI ???
//       // get the syllable texts list
//       const list<string>&
//         syllableTextsList =
//           elt->getSyllableTextsList ();
//
//       if (syllableTextsList.size ()) {
//         // build a single words value from the texts list
//         // JMI create an msrWords instance for each???
//         string wordsValue =
//           elt->syllableTextsListAsString();
//
//         // create the words
// #ifdef TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceLyrics ()) {
//           gLogStream <<
//             "Changing lyrics '" <<
//             wordsValue <<
//             "' into words for note '" <<
//             fCurrentNonGraceNoteClone->asShortString () <<
//             "'" <<
//       // JMI      fCurrentSyllableClone->asString () <<
//             endl;
//         }
// #endif
//
//         S_msrWords
//           words =
//             msrWords::create (
//               inputLineNumber,
//               msrPlacementKind::k_NoPlacement,                      // default value
//               wordsValue,
//               msrJustifyKind::kJustifyNone,                         // default value
//               msrHorizontalAlignmentKind::kHorizontalAlignmentNone, // default value
//               msrVerticalAlignmentKind::kVerticalAlignmentNone,     // default value
//               msrFontStyleKind::kFontStyleNone,                     // default value
//               msrFontSize::create (
//                 msrFontSizeKind::kFontSizeNone),                    // default value
//               msrFontWeightKind::kFontWeightNone,                   // default value
//               msrXMLLangKind::kXMLLangIt,                           // default value
//               elt->
//                 getSyllableNoteUpLink ()->
//                   fetchNoteStaffUpLink ()->
//                     getStaffNumber ());
//
//         // append it to the current non-grace note
// #ifdef TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceWords ()) {
//           gLogStream <<
//             "Appending words '" <<
//             words->asShortString () <<
//             "' to note '" <<
//             fCurrentNonGraceNoteClone->asShortString () <<
//             "'" <<
//             endl;
//         }
// #endif
//         fCurrentNonGraceNoteClone->
//           appendWordsToNote (
//             words);
//       }
//     }
  }

  else {
    stringstream s;

    s <<
      "syllable is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "' ";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    /* JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          inputLineNumber,
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
*/

    fOnGoingSyllableExtend = false;
  }
}

void msr2msrTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append time to voice clone
  fCurrentVoiceClone->
    appendTimeSignatureToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTranspose& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTransposeToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrTranspose& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

//   if (gGlobalMsr2msrOahGroup->getConvertMusicXMLTemposToMsrRehearsalMarks ()) { JMI ???
//     // create a rehearsal mark containing elt's words
//
//     S_msrRehearsalMark
//       rehearsalMark =
//         msrRehearsalMark::create (
//           elt->getInputLineNumber (),
//           msrRehearsalMark::kNone,
//           elt->tempoWordsListAsString (" "), //JMI ???
//           elt->getTempoPlacementKind ());
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceTempos ()) {
//       gLogStream <<
//         "Converting tempos '" <<
//         elt->asShortString () <<
//         "' to rehearsal mark '" <<
//         rehearsalMark->asShortString () <<
//         "'" <<
//         endl;
//     }
// #endif
//
//     // append the rehearsalMark to the current voice clone
//     fCurrentVoiceClone->
//       appendRehearsalMarkToVoice (rehearsalMark);
//   }
//
//   else {
    fCurrentVoiceClone->
      appendTempoToVoice (elt);
//   }
}

void msr2msrTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendRehearsalMarkToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "articulation is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // a fermata is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "fermata is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // an arpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "arpeggiato is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // an nonArpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "nonArpeggiato is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "' ";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technical is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithInteger is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithFloat is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "technicalWithString is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
      displayOnGoingNotesStack ("visitStart (S_msrOrnament&)");
    }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "ornament is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getSpannerTypeKind ()) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;
    case msrSpannerTypeKind::k_NoSpannerType:
      break;
  } // switch

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSpannerToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSpannerToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "spanner is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSpanner& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "glissando is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "slide is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
  else {
    stringstream s;

    s <<
      "singleTremolo is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create a double tremolo clone from the two elements
  fCurrentDoubleTremoloClone = elt; // JMI FIX THAT
/* JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createNewBornClone ());
        */

  fOnGoingDoubleTremolo = true;
}

void msr2msrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = nullptr;

  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendDynamicToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicToChord (elt);
  }

  else {
    stringstream s;

    s <<
      "dynamic is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "otherDynamic is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWords" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote || fOnGoingChord) {
    if (fOnGoingNonGraceNote) {
      // append the words to the current non-grace note clone
      fCurrentNonGraceNoteClone->
        appendWordsToNote (elt);
    }
    else if (fOnGoingChord) {
      // append the words to the current chord clone
      fCurrentChordClone->
        appendWordsToChord (elt);
    }

// JMI v0.9.61
//     Bool wordsHasBeenHandled;
//
//     // is wordsValue to be converted to an MSR tempo?
//     string wordsValue = elt->getWordsContents ();
//
//     if (gGlobalMsr2msrOahGroup->wordsIsToBeConvertedToTempo (wordsValue)) {
//       // create a tempo containing elt
//       S_msrTempo
//         tempo =
//           msrTempo::createTempoWordsOnly (
//             inputLineNumber,
//             elt,
//             msrTempo::kTempoParenthesizedNo,    // JMI
//             msrPlacementKind::kPlacementAbove); // JMI
//
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalTracingOahGroup->getTraceWords ()) {
//         gLogStream <<
//           "Converting words '" <<
//           elt->asShortString () <<
//           "' to tempo '" <<
//           tempo->asShortString () <<
//           "'" <<
//           endl;
//       }
// #endif
//
//       // append the tempo to the current voice clone
//       fCurrentVoiceClone->
//         appendTempoToVoice (tempo);
//
//       wordsHasBeenHandled = true;
//     }
//
//     else {
//       if (! wordsHasBeenHandled) {
//         if (fOnGoingNonGraceNote) {
//           // append the words to the current non-grace note clone
//           fCurrentNonGraceNoteClone->
//             appendWordsToNote (elt);
//         }
//         else if (fOnGoingChord) {
//           // append the words to the current chord clone
//           fCurrentChordClone->
//             appendWordsToChord (elt);
//         }
//       }
// //     }
//   }
//
//   else {
//     stringstream s;
//
//     s <<
//       "words is out of context, cannot be handled: '" <<
//       elt->asShortString () <<
//       "'";
//
//     msr2msrInternalError (
//       gGlobalServiceRunData->getInputSourceName (),
//       elt->getInputLineNumber (),
//       __FILE__, __LINE__,
//       s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlur& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> visitStart (S_msrSlur&), " <<
      elt->asShortString () <<
      ", fOnGoingNotesStack.size (): " <<
      fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " <<
      fOnGoingChord <<
      ", fOnGoingNonGraceNote: " <<
      fOnGoingNonGraceNote <<
      ", fOnGoingGraceNotesGroup: " <<
      fOnGoingGraceNotesGroup <<
      ", fOnGoingChordSlurLink: " <<
      fOnGoingChordSlurLink <<
      endl;
  }
#endif

  /* JMI ???
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequence:
    these should be ignored
  */

  if (fOnGoingNotesStack.size () > 0) {
//    if (fOnGoingNonGraceNote) {
      fOnGoingNotesStack.front ()->
        appendSlurToNote (elt);
//    }
  }

  else if (fOnGoingChord) {
    // don't append a slur if we're inside a slur link JMI ???
    if (fOnGoingNonGraceNote) {
      S_msrChordSlurLink
        chordSlurLink =
          msrChordSlurLink::create (
            fCurrentChordClone->getInputLineNumber (),
            elt,
            fCurrentChordClone);

      fCurrentChordClone->
        appendChordSlurLinkToChord (chordSlurLink);
    }
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "slur is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrSlur originalSlur = elt->getOriginalSlur ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> visitStart (S_msrChordSlurLink&), " <<
      elt->asShortString () <<
      ", originalSlur: " <<
      originalSlur->asShortString () <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordSlurLink
      chordSlurLink =
        msrChordSlurLink::create (
          elt->getInputLineNumber (),
          originalSlur,
          fCurrentChordClone);

    fCurrentChordClone->
      appendChordSlurLinkToChord (chordSlurLink);
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "slur link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordSlurLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordSlurLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "ligature is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "slash is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendCrescDecrescToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendCrescDecrescToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "cresc/decresc is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrCrescDecresc& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "wedge is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup " <<
      elt->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  Bool doCreateAGraceNoteClone (true); // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Creating a clone of grace notes group '" <<
        elt->asShortString () <<
        "' and attaching it to clone note '" <<
        fCurrentNonGraceNoteClone->asShortString () <<
        "'" <<
        endl;
      }
#endif

    fCurrentGraceNotesGroupClone =
      elt->
        createGraceNotesGroupNewbornClone ();

    // attach it to the current note clone
    // if (fOnGoingNonGraceNote) { JMI
   // { // JMI

/*
    if (fOnGoingChord) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroup::kGraceNotesGroupBefore:
          fCurrentChordClone->
            setChordGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroup::kGraceNotesGroupAfter:
          fCurrentChordClone->
            setChordGraceNotesGroupAfter (
              fCurrentGraceNotesGroupClone);
          break;
      } // switch
    }
    else {
    */

    if (fOnGoingNotesStack.size ()) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroup::kGraceNotesGroupBefore:
      //    fCurrentNonGraceNoteClone-> JMI
          fOnGoingNotesStack.front ()->
            setNoteGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroup::kGraceNotesGroupAfter:
      //    fCurrentNonGraceNoteClone-> JMI
          fOnGoingNotesStack.front ()->
            setNoteGraceNotesGroupAfter (
              fCurrentGraceNotesGroupClone);
          break;
      } // switch
    }

    else if (fOnGoingChordGraceNotesGroupLink) {
      // JMI ???
    }

    else {
      stringstream s;

      s <<
        "graceNotesGroup is out of context, fOnGoingNotesStack.size () == 0, cannot be handled: '" <<
        elt->asShortString () <<
        "'";

      msr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ visitStart (S_msrGraceNotesGroup&)" <<
      endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      fCurrentNonGraceNoteClone->printShort (
        gLogStream);
    }
    else {
      gLogStream <<
        "nullptr";
    }
    gLogStream << endl;
  }
#endif

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupNoteUpLink ();

  if (! noteNotesGroupIsAttachedTo) {
    stringstream s;

    s <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note upLink";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef TRACING_IS_ENABLED
  if (false && gGlobalTracingOahGroup->getTraceGraceNotes ()) { // JMI
    gLogStream <<
      "The noteNotesGroupIsAttachedTo voice clone FIRST_ONE??? '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      gLogStream <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      gLogStream <<
        "none";
    }
    gLogStream <<
       "'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false && gGlobalTracingOahGroup->getTraceGraceNotes ()) { // JMI
    gLogStream <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      gLogStream <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      gLogStream <<
        "none";
    }
    gLogStream <<
       "'" <<
      endl;
  }
#endif

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

  if (originalVoiceFirstNonGraceNote) { // JMI
    if (noteNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote) {
    // don't createSkipGraceNotesGroupClone() is there's only a single voice JMI

      // issue #34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part

      // create the skip grace notes group
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "Creating a skip clone of grace notes group '" <<
            elt->asShortString () <<
            "' to work around LilyPond issue #34" << // JMI ???
            endl;
        }
#endif

      fCurrentSkipGraceNotesGroup =
        elt->
          createSkipGraceNotesGroupClone ();
    }
  }

  // addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded() will
  // append the same skip grace notes to the ofhter voices if needed
  // in visitEnd (S_msrPart&)
}

void msr2msrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ visitEnd (S_msrGraceNotesGroup&)" <<
      endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      fCurrentNonGraceNoteClone->printShort (
        gLogStream);
    }
    else {
      gLogStream <<
        "nullptr";
    }
    gLogStream << endl;
  }
#endif

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

//* JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Removing the after grace notes element from the current voice clone" <<
        endl;
    }
#endif

    fCurrentVoiceClone->
      removeElementFromVoice (
        elt->getInputLineNumber (),
        fCurrentAfterGraceNotesGroupElement);

    // forget about the current after grace notes element
    fCurrentAfterGraceNotesGroupElement = nullptr;

    // forget about these after the pending grace notes
    fPendingAfterGraceNotesGroup = nullptr;
  }
//  */
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordGraceNotesGroupLink& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordGraceNotesGroupLink " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrGraceNotesGroup
    originalGraceNotesGroup =
      elt->getOriginalGraceNotesGroup ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "--> visitStart (S_msrChordGraceNotesGroupLink&), " <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          elt->getInputLineNumber (),
          originalGraceNotesGroup,
          fCurrentChordClone);

    switch (originalGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroup::kGraceNotesGroupBefore:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkBefore (
            inputLineNumber,
            chordChordGraceNotesGroupLink);
        break;
      case msrGraceNotesGroup::kGraceNotesGroupAfter:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkAfter (
            inputLineNumber,
            chordChordGraceNotesGroupLink);
        break;
    } // switch
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "grace notes group link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordGraceNotesGroupLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef TRACING_IS_ENABLED
    S_msrGraceNotesGroup
      originalGraceNotesGroup =
        elt->getOriginalGraceNotesGroup ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordGraceNotesGroupLink" <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " << fOnGoingChordGraceNotesGroupLink <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordGraceNotesGroupLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the note clone
  S_msrNote
    noteClone =
      elt->
        createNoteNewbornClone (
          fCurrentPartClone);

  // register clone in this tranlastors' voice notes map and ongoing notes stack
  fVoiceNotesMap [elt] = noteClone; // JMI XXL
  fOnGoingNotesStack.push_front (noteClone);

  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/* JMI
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            gLogStream <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            gLogStream <<
              "none";
          }
          gLogStream <<
             "'" <<
            endl;
        }
#endif
*/

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      fCurrentGraceNoteClone = noteClone;
      break;

    default:
      fCurrentNonGraceNoteClone = noteClone;

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'" <<
            endl;
        }
#endif
      }

      fOnGoingNonGraceNote = true;
  } // switch

//* JMI
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()
  ) {
    int inputLineNumber =
      elt->getInputLineNumber ();

    // yes, create the after grace notes
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber<<
        endl;
    }
#endif

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
        inputLineNumber,
        fCurrentNonGraceNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fCurrentVoiceClone);

    // register current afterGraceNotesGroup element
    fCurrentAfterGraceNotesGroupElement =
      fCurrentNonGraceNoteClone;
  }
//*/
}

void msr2msrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  msrNoteKind
    noteKind = elt->getNoteKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "FAA fCurrentNonGraceNoteClone = " <<
      endl;
    if (fCurrentNonGraceNoteClone) {
      gLogStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLogStream <<
        "nullptr" <<
        endl;
    }

    gLogStream <<
      "FAA fCurrentGraceNoteClone = " <<
      endl;
    if (fCurrentGraceNoteClone) {
      gLogStream <<
        fCurrentGraceNoteClone;
    }
    else {
      gLogStream <<
        "nullptr" <<
        endl;
    }
  }
#endif

  switch (noteKind) {

    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRestInMeasure:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteSkipInMeasure: // JMI
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteRegularInMeasure:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      if (fOnGoingDoubleTremolo) {

        if (fCurrentNonGraceNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotes ()) {
            gLogStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
#endif

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }

        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotes ()) {
            gLogStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              endl;
          }
#endif

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNonGraceNoteClone);
        }

        else {
          stringstream s;

          s <<
            "note '" << fCurrentNonGraceNoteClone->asShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msr2msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      else {
        stringstream s;

        s <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msr2msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMI
      gLogStream <<
        "fOnGoingGraceNotesGroup = " <<
        fOnGoingGraceNotesGroup <<
        endl;
        */

      if (! fOnGoingGraceNotesGroup) {
        stringstream s;

        s <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of grace notes";

        msr2msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "Appending grace note '" <<
            fCurrentGraceNoteClone->asShortString () <<
            "' to the grace notes group'" <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            "' in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif

        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }

    /* JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif

        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
          gLogStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            endl;
        }
#endif

        fPendingAfterGraceNotes->
          appendNoteToAfterGraceNotesContents (
            fCurrentNonGraceNoteClone);
      }

      else {
        stringstream s;

        s <<
          "both fCurrentGraceNoteGroupsClone and fPendingAfterGraceNoteGroup are null," <<
          endl <<
          "cannot handle grace note'" <<
          elt->asString () <<
          "'";

        msr2msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fOnGoingChord) {
        // append fCurrentNonGraceNoteClone to fCurrentChordClone
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentNonGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        stringstream s;

        s <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        stringstream s;

        s <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          endl;
      }
#endif

      fTupletClonesStack.top ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fCurrentVoiceClone);
      break;
  } // switch

/* JMI
  // handle melisma
  msrSyllable::msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        / * JMI ???
        // create melisma start command
        S_lpsrMelismaCommand
          melismaCommand =
            lpsrMelismaCommand::create (
              inputLineNumber,
              lpsrMelismaCommand::kMelismaStart);

        // append it to current voice clone
        fCurrentVoiceClone->
          appendOtherElementToVoice (melismaCommand);

        // append
        * /

        fOnGoingSyllableExtend = true;
      }
      break;
    case msrSyllable::kStartSyllableExtend:
      break;
    case msrSyllable::kContinueSyllableExtend:
      break;
    case msrSyllable::kStopSyllableExtend:
      break;
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch
*/

  switch (noteKind) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    default:
      fOnGoingNonGraceNote = false;
  } // switch

  // forget about current note
  fOnGoingNotesStack.pop_front ();
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteOctaveShift (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordOctaveShift (elt);
  }
  else {
    stringstream s;

    s <<
      "octaveShift is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
  if (false) // JMI
    fCurrentChordClone->
      appendStemToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "stem is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
// JMI      ", fOnGoingChord: " << fOnGoingChord <<
      endl;
  }
#endif

  // a beam may be present at the same time
  // in a note or grace note and the chord the latter belongs to

  if (fOnGoingGraceNotesGroup) {
    fCurrentGraceNoteClone->
      appendBeamToNote (elt);
  }
  else if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) { // else ??? JMI
    // don't append a beam if we're inside a beam link JMI ???
    if (fOnGoingNonGraceNote) {
      S_msrChordBeamLink
        chordBeamLink =
          msrChordBeamLink::create (
            fCurrentChordClone->getInputLineNumber (),
            elt,
            fCurrentChordClone);

      fCurrentChordClone->
        appendChordBeamLinkToChord (chordBeamLink);
    }
  }
/* JMI
    fCurrentChordClone->
      appendBeamToChord (elt);
      */
}

void msr2msrTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordBeamLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrBeam originalBeam = elt->getOriginalBeam ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "--> visitStart (S_msrChordBeamLink&), " <<
      elt->asShortString () <<
      ", originalBeam: " <<
      originalBeam->asShortString () <<
      endl;
  }
#endif

  if (fOnGoingChord) {
    S_msrChordBeamLink
      chordBeamLink =
        msrChordBeamLink::create (
          elt->getInputLineNumber (),
          originalBeam,
          fCurrentChordClone);

    fCurrentChordClone->
      appendChordBeamLinkToChord (chordBeamLink);
  }

  else {
    displayCurrentOnGoingValues ();

    stringstream s;

    s <<
      "beam link is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingChordBeamLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordBeamLink& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingChordBeamLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.top ()->
      appendChordToTuplet (
        fCurrentChordClone);
  }

  else if (fOnGoingDoubleTremolo) {
    if (elt->getChordIsFirstChordInADoubleTremolo ()) {
      // replace double tremolo's first element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordFirstElement (
          elt);
    }

    else if (elt->getChordIsSecondChordInADoubleTremolo ()) {
      // replace double tremolo's second element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordSecondElement (
          elt);
    }

    else {
      stringstream s;

      s <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else if (fCurrentGraceNotesGroupClone) {
    // append the chord to the grace notes
    fCurrentGraceNotesGroupClone->
      appendChordToGraceNotesGroup (
        fCurrentChordClone);
  }

  else {
    // appending the chord to the voice clone at once
    fCurrentVoiceClone->
      appendChordToVoice (
        fCurrentChordClone);
  }

  fOnGoingChord = true;
}

void msr2msrTranslator::visitEnd (S_msrChord& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget about the current chord clone
  fCurrentChordClone = nullptr;

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.push (tupletClone);
}

void msr2msrTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      endl;
  }
#endif

  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->asString () <<
      "'" <<
      endl;
    }
#endif

    fTupletClonesStack.top ()->
      appendTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      endl;
    }
#endif

    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteTie (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTieToChord (elt);
  }
  else {
    stringstream s;

    s <<
      "tie is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendSegnoToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "segno is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2msrTranslator::visitStart (S_msrDalSegno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDalSegno" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendDalSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendDalSegnoToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "dal segno is out of context, cannot be handled: '" <<
      elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

/* JMI
  // create a hidden measure and barLine
  S_msrHiddenMeasureAndBarLine
    hiddenMeasureAndBarLine =
      msrHiddenMeasureAndBarLine::create (
        inputLineNumber);

  // create a hidden measure and barLine descr
  S_msrHiddenMeasureAndBarLineDescr
    hiddenMeasureAndBarLineDescr =
      msrHiddenMeasureAndBarLineDescr::create (
        inputLineNumber,
        positionInMeasure);
*/

  // register it in the hidden measure and barLine descr list
  fPartHiddenMeasureAndBarLineDescrList.push_back (
    msrHiddenMeasureAndBarLineDescr::create (
      inputLineNumber,
       elt));

//* JMI BLARK
  fCurrentPartClone->
    insertHiddenMeasureAndBarLineInPartClone (
      inputLineNumber,
      elt->getMeasureElementPositionInMeasure ());
     // */
}

void msr2msrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendCodaToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendCodaToNote (elt);
    }
  }
  else {
    stringstream s;

    s <<
      "coda is out of context, cannot be handled: '" << elt->asShortString () <<
      "'";

    msr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendLineBreakToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    appendPageBreakToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2msrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->getPartCombinedName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending start in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingStartInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingKind (),
      elt->getRepeatEndingNumber ());
}

void msr2msrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // handle the repeat ending end in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFullMeasureRests& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFullMeasureRests" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleFullMeasureRestsStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2msrTranslator::visitEnd (S_msrFullMeasureRests& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFullMeasureRests" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleFullMeasureRestsEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFullMeasureRestsContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFullMeasureRestsContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrFullMeasureRestsContents&)");
  }
#endif

  fCurrentVoiceClone->
    handleFullMeasureRestsContentsStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrFullMeasureRestsContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFullMeasureRestsContents" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrFullMeasureRestsContents&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleFullMeasureRestsContentsEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

/* JMI
  // set last segment as the measures repeat pattern segment
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatPattern&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatPatternStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatPattern&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatPatternEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatReplicas&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  --gIndenter;


#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatReplicas&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarLine" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Handling '" <<
      msrBarLine::barLineCategoryKindAsString (
        elt->getBarLineCategory ()) <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      endl;
  }
#endif

  // append the barLine to the current voice clone
  fCurrentVoiceClone->
    appendBarLineToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void msr2msrTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace

//________________________________________________________________________
/* JMI
void msr2msrTranslator::prependSkipGraceNotesGroupToPartOtherVoices (
  S_msrPart            partClone,
  S_msrVoice           voiceClone,
  S_msrGraceNotesGroup skipGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "--> prepending a skip graceNotesGroup clone " <<
        skipGraceNotesGroup->asShortString () <<
        " to voices other than \"" <<
        voiceClone->getVoiceName () << "\"" <<
        " in part " <<
        partClone->getPartCombinedName () <<
        ", line " << skipGraceNotesGroup->getInputLineNumber () <<
        endl;
    }
#endif

  map<int, S_msrStaff>
    partStavesMap =
      partClone->
        getPartStaveNumbersToStavesMap ();

  for (
    map<int, S_msrStaff>::const_iterator i=partStavesMap.begin ();
    i!=partStavesMap.end ();
    ++i
  ) {
    list<S_msrVoice>
      staffAllVoicesVector =
        (*i).second->
          getStaffAllVoicesVector ();

    for (
      list<S_msrVoice>::const_iterator j=staffAllVoicesVector.begin ();
      j!=staffAllVoicesVector.end ();
      ++j
    ) {
      S_msrVoice voice = (*j);

      if (voice != voiceClone) {
        // prepend skip grace notes to voice JMI
        / *
        voice->
          prependGraceNotesGroupToVoice (
            skipGraceNotesGroup);
            * /
      }
    } // for

  } // for
}
*/

/*
//     else if (gGlobalMsr2msrOahGroup->getConvertWordsToRehearsalMarks ()) {
//       // create a rehearsal mark containing elt's words contents
//       S_msrRehearsalMark
//         rehearsalMark =
//           msrRehearsalMark::create (
//             inputLineNumber,
//             msrRehearsalMark::kNone,
//             elt->getWordsContents (),
//             elt->getWordsPlacementKind ()); // above ??? JMI
//
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalTracingOahGroup->getTraceWords ()) {
//         gLogStream <<
//           "Converting words '" <<
//           elt->asShortString () <<
//           "' to rehearsal mark '" <<
//           rehearsalMark->asShortString () <<
//           "'" <<
//           endl;
//       }
// #endif
//
//       // append the rehearsalMark to the current voice clone
//       fCurrentVoiceClone->
//         appendRehearsalMarkToVoice (rehearsalMark);
//
//       wordsHasBeenHandled = true;
//     }
//
//     else {
//       string wordsContents = elt->getWordsContents ();
//
//       // is this words contents in the string to dal segno kind map?
//       const map<string, msrDalSegno::msrDalSegnoKind>&
//         converStringToDalSegnoMap =
//           gGlobalMsr2msrOahGroup->
//             getStringToDalSegnoKindMap ();
//
//       map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
//         it = converStringToDalSegnoMap.find (wordsContents);
//
//       if (it != converStringToDalSegnoMap.end ()) {
//         // yes
//         msrDalSegno::msrDalSegnoKind
//           dalSegnoKind =
//             (*it).second;
//
//         // create a dal segno element containing elt's words contents
//         S_msrDalSegno
//           dalSegno =
//             msrDalSegno::create (
//               inputLineNumber,
//               dalSegnoKind,
//               wordsContents,
//               elt->getWordsStaffNumber ());
//
// #ifdef TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceWords ()) {
//           gLogStream <<
//             "Converting words '" <<
//             elt->asShortString () <<
//             "' to dal segno element '" <<
//             dalSegno->asShortString () <<
//             "'" <<
//             endl;
//         }
// #endif
//
//         if (fOnGoingNonGraceNote) {
//           fCurrentNonGraceNoteClone->
//             appendDalSegnoToNote (dalSegno);
//         }
//         else if (fOnGoingChord) {
//           fCurrentChordClone->
//             appendDalSegnoToChord (dalSegno);
//         }
//
//       wordsHasBeenHandled = true;
//       }
*/
