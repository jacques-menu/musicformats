/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>
#include <algorithm>    // for_each

#include "mfStaticSettings.h"

#include "mfAssert.h"

#include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrAfterGraceNotes.h"
#include "msrArticulations.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrBreaks.h"
#include "msrDoubleTremolos.h"
#include "msrGlissandos.h"
#include "msrRehearsalMarks.h"
#include "msrSlides.h"
#include "msrTablatures.h"
#include "msrTechnicals.h"
#include "msrTempos.h"
#include "msrVoiceStaffChanges.h"

#include "msr2msrWae.h"

#include "msr2msrComponent.h"

#include "msr2msrTranslator.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "msrPathToVoice.h" // JMI TEMP TEST v0.9.66

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLineDescr msrHiddenMeasureAndBarLineDescr::create (
  int           inputLineNumber,
  S_msrDalSegno dalSegno)
{
  msrHiddenMeasureAndBarLineDescr* obj = new
    msrHiddenMeasureAndBarLineDescr (
      inputLineNumber,
      dalSegno);
  assert (obj != nullptr);
  return obj;
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

std::string msrHiddenMeasureAndBarLineDescr::hiddenMeasureAndBarLineDescrAsString () const
{
  std::stringstream ss;

  ss <<
    "hiddenMeasureAndBarLineDescr" <<
    ", inputLineNumber: " << fInputLineNumber <<
    ", dalSegno: " << fDalSegno->asString ();

  return ss.str ();
}

void msrHiddenMeasureAndBarLineDescr::print (std::ostream& os) const
{
  const int fieldWidth = 14;

  os << std::left <<
    std::setw (fieldWidth) <<
    "inputLineNumber" << ": " <<
    fInputLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "dalSegno" << ": " <<
    fDalSegno->asString () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHiddenMeasureAndBarLineDescr& elt)
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
msr2msrTranslator::msr2msrTranslator ()
{};

msr2msrTranslator::~msr2msrTranslator ()
{}

//________________________________________________________________________
S_msrScore msr2msrTranslator::translateMsrToMsr (
  const S_msrScore& theMsrScore)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create the resulting MSR score
  fResultingNewMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
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
  const S_msrScore&       theMsrScore,
  const S_msrPathToVoice& pathToVoice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  mfAssert (
    __FILE__, __LINE__,
    pathToVoice != nullptr,
    "pathToVoice is null");

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create the resulting MSR score
  fResultingNewMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
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
  gLog <<
    "Current ongoing values:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "fOnGoingIdentification" << ": " << fOnGoingIdentification <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingStaff" << ": " << fOnGoingStaff <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingHarmoniesVoice" << ": " << fOnGoingHarmoniesVoice <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingHarmony" << ": " << fOnGoingHarmony <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingFiguredBassVoice" << ": " << fOnGoingFiguredBassVoice <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingNotesStack.size ()" << ": " << fOnGoingNotesStack.size () <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingNonGraceNote" << ": " << fOnGoingNonGraceNote <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingDoubleTremolo" << ": " << fOnGoingDoubleTremolo <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingGraceNotesGroup" << ": " << fOnGoingGraceNotesGroup <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingChord" << ": " << fOnGoingChord <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingStanza" << ": " << fOnGoingStanza <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingSyllableExtend" << ": " << fOnGoingSyllableExtend <<
    std::endl;

  --gIndenter;
}

//________________________________________________________________________
void msr2msrTranslator::displayPartHiddenMeasureAndBarLineDescrList ()
{
  gLog <<
    "fPartHiddenMeasureAndBarLineDescrList:" <<
    std::endl;

  if (fPartHiddenMeasureAndBarLineDescrList.size ()) {
    ++gIndenter;

    const int fieldWidth = 19;

    std::list<S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarLineDescr->getDalSegno ();

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "inputLineNumber" << ": " <<
        hiddenMeasureAndBarLineDescr->getInputLineNumber () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "dalSegno" << ": " <<
        dalSegno <<
        std::endl;

      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
  else {
    gLog << "[EMPTY]" << std::endl;
  }
}

//________________________________________________________________________
void msr2msrTranslator::displayOnGoingNotesStack (
  const std::string& context)
{
  size_t onGoingNotesStackSize = fOnGoingNotesStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The on-going notes stack contains " <<
    onGoingNotesStackSize <<
    " elements" <<
    " (" << context << "):" <<
    std::endl;

  if (onGoingNotesStackSize) {
    std::list<S_msrNote>::const_iterator
      iBegin = fOnGoingNotesStack.begin (),
      iEnd   = fOnGoingNotesStack.end (),
      i      = iBegin;

    S_msrNote note = (*i);

    ++gIndenter;

    int n = onGoingNotesStackSize;
    for ( ; ; ) {
      gLog <<
        "v (" << n << ")" <<
        std::endl;

      ++gIndenter;
      gLog << note;
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

//________________________________________________________________________
void msr2msrTranslator::handlePartHiddenMeasureAndBarLineDescrList ()
{
  gLog <<
    "fPartHiddenMeasureAndBarLineDescrList:" <<
    std::endl;

  if (fPartHiddenMeasureAndBarLineDescrList.size ()) {
    std::list<S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarLineDescr->getDalSegno ();

      fCurrentPartClone->
        insertHiddenMeasureAndBarLineInPartClone (
          hiddenMeasureAndBarLineDescr->getInputLineNumber (),
          dalSegno->getMeasurePosition ());

      if (++i == iEnd) break;
    } // for
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2msrTranslator::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrScaling& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the aystem layout in the current MSR score clone
  fResultingNewMsrScore->
    setSystemLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the staff layout in the current MSR score clone
  fResultingNewMsrScore->
    setStaffLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the appearance in the current MSR score clone
  fResultingNewMsrScore->
    setAppearance (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the page layout in the current MSR score clone
  fResultingNewMsrScore->
    setPageLayout (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the MSR score
  fResultingNewMsrScore->
    appendCreditToScore (fCurrentCredit);
}

void msr2msrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = nullptr;
}

void msr2msrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // don't append it to the current credit, since the latter is no clone
  /* JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      */
}

void msr2msrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Pushing part group clone " <<
      partGroupClone->getPartGroupCombinedName () <<
      " onto stack" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.push (
    partGroupClone);
}

void msr2msrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the current part group at the top of the stack
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();

  if (fPartGroupsStack.size () == 1) {
    // add the current partgroup clone to the MSR score clone
    // since it's alone in the stack

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
        "Adding part group clone " <<
        currentPartGroup->getPartGroupCombinedName () <<
        " to MSR score" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fResultingNewMsrScore->
      addPartGroupToScore (
        currentPartGroup);
  }

  else {
    // append the current part group to the one one level higher,
    // i.e. the new current part group
    fPartGroupsStack.top ()->
      appendSubPartGroupToPartGroup (
        currentPartGroup);
  }

  // pop current partGroup from this visitors's stack
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Popping part group clone " <<
      fPartGroupsStack.top ()->getPartGroupCombinedName () <<
      " from stack" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.pop ();
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  std::string
    partCombinedName =
      elt->getPartCombinedName ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create a part clone
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.top ());

  // add it to the partGroup clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Adding part clone " <<
      fCurrentPartClone->getPartCombinedName () <<
      " to part group clone \"" <<
      fPartGroupsStack.top ()->getPartGroupCombinedName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.top ()->
    appendPartToPartGroup (fCurrentPartClone);
}

void msr2msrTranslator::visitEnd (S_msrPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    partInstrumentAbbreviation =
      fCurrentPartClone->
        getPartInstrumentAbbreviation ();

  // populate part instrument short name if empty and possible
  if (partInstrumentAbbreviation.size () == 0) {
    std::string
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    displayPartHiddenMeasureAndBarLineDescrList ();
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI handlePartHiddenMeasureAndBarLineDescrList ();

  // handle skip grace notes if needed
  if (fCurrentSkipGraceNotesGroup) {
    // add it ahead of the other voices in the part if needed
    fCurrentPartClone->
      addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded ( // JMI only if there's more than one voice???
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffLinesNumber" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a staff lines number clone
  fCurrentStaffLinesNumberClone =
    elt->
      createStaffLinesNumberNewbornClone ();
}
*/

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a staff tuning clone
  fCurrentStaffTuningClone =
    elt->
      createStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffTuningClone = nullptr;
}

void msr2msrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the staff details to the current voice clone
  fCurrentVoiceClone->
    appendStaffDetailsToVoice (
      elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
          setPartFiguredBassStaff (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;
  } // switch
}

void msr2msrTranslator::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

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
      "<!--=== voice \"" << elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceOriginal = elt;

  ++gIndenter;

  std::string voiceName = elt->getVoiceName ();

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
          std::string partCombinedName =
            fCurrentVoiceOriginal->
              fetchVoiceUpLinkToPart ()->
                getPartCombinedName ();

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Creating a ChordNames context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

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
          std::string partCombinedName =
            fCurrentVoiceOriginal->
              fetchVoiceUpLinkToPart ()->
                getPartCombinedName ();

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Creating a FiguredBass context for \"" << voiceName <<
              "\" in part " << partCombinedName <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  // forget about the current voice clone
  fCurrentVoiceClone = nullptr;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrHarmony " <<
      elt->asString () <<
      ", fCurrentVoiceClone->getVoiceKind (): " << fCurrentVoiceClone->getVoiceKind () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      ", fCurrentHarmonyClone: " << fCurrentHarmonyClone <<
      ", fCurrentNonGraceNoteClone: " << fCurrentNonGraceNoteClone <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a harmony newborn clone
  fCurrentHarmonyClone =
    elt->
      createHarmonyNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // register fCurrentNonGraceNoteClone as the harmony note uplink
    fCurrentHarmonyClone->
      setHarmonyUpLinkToNote (
        fCurrentNonGraceNoteClone);

    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNote (
        fCurrentHarmonyClone);

    // don't append the harmony to the part harmony,
    // this has been done in pass2b // JMI ??? v0.9.66
  }

  else if (fOnGoingChord) {
    // append the harmony clone to the current voice clone
    fCurrentChordClone->
      appendHarmonyToChord (
        fCurrentHarmonyClone); // JMI v0.9.66
  }

  else if (fOnGoingHarmoniesVoice) {
    // append the harmony clone to the current voice clone
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  else {
    std::stringstream ss;

    ss <<
      "harmony is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about current harmony clone
  fCurrentHarmonyClone = nullptr;

  fOnGoingHarmony = false;
}

void msr2msrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingHarmony) {
    // register the frame in the current non-grace note clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }

  else {
    std::stringstream ss;

    ss <<
      "frame is out of context, cannot be handled: " <<
      elt->asShortString ();

//     msr2msrInternalError ( // JMI v0.9.67
    msr2msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
//       __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFiguredBass " <<
      elt->asString () <<
      ", fCurrentVoiceClone->getVoiceKind (): " << fCurrentVoiceClone->getVoiceKind () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingFiguredBassVoice: " << fOnGoingFiguredBassVoice <<
      ", fOnGoingFiguredBass: " << fOnGoingFiguredBass <<
      ", fCurrentFiguredBassClone: " << fCurrentFiguredBassClone <<
      ", fCurrentNonGraceNoteClone: " << fCurrentNonGraceNoteClone <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a figured bass element newborn clone
  fCurrentFiguredBassClone =
    elt->
      createFiguredBassNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // register fCurrentNonGraceNoteClone as the figured bass note uplink
    fCurrentFiguredBassClone->
      setFiguredBassUpLinkToNote (
        fCurrentNonGraceNoteClone);

//     // register the figured bass in the current non-grace note clone
//     fCurrentNonGraceNoteClone->
//       appendFiguredBassToNote (
//         fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass, JMI ??? v0.9.66
    // this will be done below
  }

  else if (fOnGoingChord) {
    // register the figured bass clone in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (
        fCurrentFiguredBassClone); // JMI ??? v0.9.66
  }

  else if (fOnGoingFiguredBassVoice) {
    // append the figured bass clone to the current voice clone
    fCurrentVoiceClone->
      appendFiguredBassToVoiceClone (
        fCurrentFiguredBassClone);
  }

  else {
    std::stringstream ss;

    ss <<
      "figured bass is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingFiguredBass = true;
}

void msr2msrTranslator::visitStart (S_msrBassFigure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBassFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bass figure to the current figured bass
  fCurrentFiguredBassClone->
    appendFigureToFiguredBass (
      elt);
}

void msr2msrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about current figured bass clone
  fCurrentFiguredBassClone = nullptr;

  fOnGoingFiguredBass = true;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber: '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== " <<
      "part \"" << fCurrentPartClone->getPartName () << "\"" <<
      " (partID \"" << fCurrentPartClone->getPartID () << "\")" <<
      ", measure \"" << measureNumber << "\"" <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      " ===-->" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClone);

  if (fOnGoingMultipleFullBarRests) {
    // append current measure clone to the current multiple full-bar rests clone
    fCurrentMultipleFullBarRestsClone->
      appendMeasureToMultipleFullBarRests (
        fCurrentMeasureClone);
  }
  else {
    // append current measure clone to the current voice clone
    fCurrentVoiceClone->
      appendMeasureCloneToVoiceClone (
        inputLineNumber,
        fCurrentMeasureClone);
  }

  // is this a full-bar rest? JMI KAKA
//   if (elt->getMeasureIsAFullBarRest ()) {
//     // yes
//
    // should we compress full-bar rests?
//     if (gGlobalMsr2msrOahGroup->getCompressFullBarRests ()) {//
//       // yes
//
//       if (! fCurrentRestMeasure) {
//         // this is the first multiple full-bar rest in the sequence
//
//         // create a multiple full-bar rests  containing fCurrentMeasureClone
//         fCurrentMultipleFullBarRests =
//           msrMultipleFullBarRests::create (
//             inputLineNumber,
//             fCurrentMeasureClone,
//             fCurrentVoiceClone);
//
// /* JMI
//         // append the current multiple full-bar rests to the current voice clone
//         fCurrentVoiceClone->
//           appendMultipleFullBarRestsToVoice (
//             inputLineNumber,
//             fCurrentMultipleFullBarRests);
//             */
//       }
//
//       else {
//         // this is a subsequent multiple full-bar rest, merely append it
//         fCurrentMultipleFullBarRests->
//           appendMeasureCloneToMultipleFullBarRests (
//             fCurrentMeasureClone);
//       }
//
//       fCurrentRestMeasure = fCurrentMeasureClone;
//     }
//
//     else {
//       // no
//
//       // append current measure clone to the current voice clone
//       fCurrentVoiceClone->
//         appendMeasureCloneToVoiceClone (
//           inputLineNumber,
//           fCurrentMeasureClone);
// //     }
//   }
//
//   else {
//     // no
//
//     // append current measure clone to the current voice clone
//     fCurrentVoiceClone->
//       appendMeasureCloneToVoiceClone (
//         inputLineNumber,
//         fCurrentMeasureClone);
//   }

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

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  std::string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasure '" <<
      measureNumber <<
      "', nextMeasureNumber: '" <<
      nextMeasureNumber <<
      "', measurePuristNumber: '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasureClone->
    finalizeMeasureClone (
      inputLineNumber,
      elt, // original measure
      fCurrentVoiceClone);

  Bool doCreateABarCheck (false);

  switch (elt->getMeasureKind ()) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
        std::stringstream ss;

        ss <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureUpLinkToVoice ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2msrTranslator";

      // JMI  msr2msrInternalError (
        msr2msrInternalWarning (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
  //        __FILE__, __LINE__,
          ss.str ());
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
        case msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown:
          break;
        case msrMeasureEndRegularKind::kMeasureEndRegularKindYes:
          doCreateABarCheck = true;
          break;
        case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
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

  // is this a full-bar rest?
  if (elt->getMeasureIsAFullBarRest ()) {
    // yes JMI
  }

  else {
    // no

    // should we compress full-bar rests?
//    if (gGlobalMsr2msrOahGroup->getTraceMultipleFullBarRests ()) {
//       // yes
//
//       if (fCurrentMultipleFullBarRests) {
//         // append the current multiple full-bar rests to the current voice clone
//         fCurrentVoiceClone->
//           appendMultipleFullBarRestsToVoice (
//             inputLineNumber,
//             fCurrentMultipleFullBarRests);
//
//         // forget about the current rest measure
//         fCurrentRestMeasure = nullptr;
//
//         // forget about the current multiple full-bar rests
//         fCurrentMultipleFullBarRests = nullptr;
//       }
//
//       else {
//         std::stringstream ss;
//
//         ss <<
//           "fCurrentMultipleFullBarRests is null upon multiple full-bar rest end" <<
//           measureNumber <<
//           "', measurePuristNumber: '" <<
//           measurePuristNumber <<
//           "', line " << inputLineNumber;
//
// /* JMI ???
//         msr2msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, __LINE__,
//           ss.str ());
//           */
//       }
//     }
  }

  // is this measureNumber in the parts ignore IDs set?
  if (gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().size ()) {
    std::set<std::string>::iterator
      it =
        gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().find (
          measureNumber);

    if (it != gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().end ()) {
      gLog <<
        "==> adding a page break after measureNumber: \"" <<
        measureNumber <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;

      S_msrPageBreak
        pageBreak =
          msrPageBreak::create (
            inputLineNumber,
            fCurrentMeasureClone,
            msrUserSelectedPageBreakKind::kUserSelectedPageBreakYes);

      fCurrentMeasureClone->
        appendPageBreakToMeasure (
          pageBreak);
    }
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      appendSyllableToNoteAndSetItsUpLinkToNote (
        fCurrentNonGraceNoteClone);

//     if (gGlobalMsr2msrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) { JMI ???
//       // get the syllable texts list
//       const std::list<std::string>&
//         syllableTextsList =
//           elt->getSyllableTextsList ();
//
//       if (syllableTextsList.size ()) {
//         // build a single words value from the texts list
//         // JMI create an msrWords instance for each???
//         std::string wordsValue =
//           elt->syllableTextsListAsString();
//
//         // create the words
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//             "Changing lyrics '" <<
//             wordsValue <<
//             "' into words for note '" <<
//             fCurrentNonGraceNoteClone->asShortString () <<
//             "'" <<
//       // JMI      fCurrentSyllableClone->asString () <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         S_msrWords
//           words =
//             msrWords::create (
//               inputLineNumber,
//               msrPlacementKind::kPlacement_UNKNOWN_,                      // default value
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
//                 getSyllableUpLinkToNote ()->
//                   fetchUpLinkToNoteToStaff ()->
//                     getStaffNumber ());
//
//         // append it to the current non-grace note
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceWords ()) {
//     std::stringstream ss;
//
//     ss <<
//             "Appending words '" <<
//             words->asShortString () <<
//             "' to note '" <<
//             fCurrentNonGraceNoteClone->asShortString () <<
//             "'" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//         fCurrentNonGraceNoteClone->
//           appendWordsToNote (
//             words);
//       }
//     }
  }

  else {
    std::stringstream ss;

    ss <<
      "syllable is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendClefToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendKeyToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append time to voice clone
  fCurrentVoiceClone->
    appendTimeSignatureToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTranspositionToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (gGlobalMsr2msrOahGroup->getConvertMusicXMLTemposToMsrRehearsalMarks ()) { JMI ???
//     // create a rehearsal mark containing elt's words
//
//     S_msrRehearsalMark
//       rehearsalMark =
//         msrRehearsalMark::create (
//           elt->getInputLineNumber (),
//           msrRehearsalMarkKind::kRehearsalMarkNone,
//           elt->tempoWordsListAsString (" "), //JMI ???
//           elt->getTempoPlacementKind ());
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTempos ()) {
//       gLog <<
//         "Converting tempos '" <<
//         elt->asShortString () <<
//         "' to rehearsal mark '" <<
//         rehearsalMark->asShortString () <<
//         "'" <<
//         std::endl;
//     std::stringstream ss;
//
//     ss <<
//     }
// #endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendRehearsalMarkToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "articulation is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "fermata is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "arpeggiato is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "nonArpeggiato is out of context, cannot be handled: " <<
      elt->asShortString ();;

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "technical is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "technicalWithInteger is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "technicalWithFloat is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "technicalWithString is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnamentsDetails ()) {
      displayOnGoingNotesStack ("visitStart (S_msrOrnament&)");
    }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "ornament is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOrnament " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getSpannerTypeKind ()) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;
    case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
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
    std::stringstream ss;

    ss <<
      "spanner is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSpanner& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "glissando is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "slide is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "singleTremolo is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendDynamicToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicToChord (elt);
  }

  else {
    std::stringstream ss;

    ss <<
      "dynamic is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "otherDynamic is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWords" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
//     std::string wordsValue = elt->getWordsContents ();
//
//     if (gGlobalMsr2msrOahGroup->wordsIsToBeConvertedToTempo (wordsValue)) {
//       // create a tempo containing elt
//       S_msrTempo
//         tempo =
//           msrTempo::createTempoWordsOnly (
//             inputLineNumber,
//             elt,
//             msrTempoParenthesizedKind::kTempoParenthesizedNo,    // JMI
//             msrPlacementKind::kPlacementAbove); // JMI
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceWords ()) {
//         gLog <<
//           "Converting words '" <<
//           elt->asShortString () <<
//           "' to tempo '" <<
//           tempo->asShortString () <<
//           "'" <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
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
//     std::stringstream ss;
//
//     ss <<
//       "words is out of context, cannot be handled: " <<
//       elt->asShortString ();
//
//     msr2msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       elt->getInputLineNumber (),
//       __FILE__, __LINE__,
//       ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlur& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACING_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
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
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    std::stringstream ss;

    ss <<
      "slur is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrSlur originalSlur = elt->getOriginalSlur ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "--> visitStart (S_msrChordSlurLink&), " <<
      elt->asShortString () <<
      ", originalSlur: " <<
      originalSlur->asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    std::stringstream ss;

    ss <<
      "slur link is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingChordSlurLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordSlurLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "ligature is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "slash is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendCrescDecrescToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendCrescDecrescToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "cresc/decresc is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "wedge is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrGraceNotesGroup " <<
      elt->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool doCreateAGraceNoteClone (true); // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Creating a clone of grace notes group " <<
        elt->asString () <<
        " and attaching it to clone note " <<
        fCurrentNonGraceNoteClone->asShortString () <<
        std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentGraceNotesGroupClone =
      elt->
        createGraceNotesGroupNewbornClone ();

    // attach it to the current note clone
    // if (fOnGoingNonGraceNote) { JMI
   // { // JMI

/*
    if (fOnGoingChord) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
          fCurrentChordClone->
            setChordGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
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
        case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
      //    fCurrentNonGraceNoteClone-> JMI v0.9.67
          fOnGoingNotesStack.front ()->
            setNoteGraceNotesGroupBefore (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
      //    fCurrentNonGraceNoteClone-> JMI v0.9.67
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
      std::stringstream ss;

      ss <<
        "graceNotesGroup is out of context, fOnGoingNotesStack.size () == 0, cannot be handled: " <<
        elt->asShortString ();

      msr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "+++++++++++++++++++++++++ visitStart (S_msrGraceNotesGroup&)" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      ss << fCurrentNonGraceNoteClone;
    }
    else {
      ss <<
        "[NONE]";
    }
    ss << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupUpLinkToNote ();

  if (! noteNotesGroupIsAttachedTo) {
    std::stringstream ss;

    ss <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note upLink";

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef MF_TRACE_IS_ENABLED
  if (false && gTraceOahGroup->getTraceGraceNotes ()) { // JMI
    std::stringstream ss;

    ss <<
      "The noteNotesGroupIsAttachedTo voice clone FIRST_ONE??? '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      gLog <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      gLog <<
        "[NONE]";
    }
    gLog <<
       "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (false && gTraceOahGroup->getTraceGraceNotes ()) { // JMI v0.9.67
    std::stringstream ss;

    ss <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      gLog <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      gLog <<
        "[NONE]";
    }
    gLog <<
       "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

  if (originalVoiceFirstNonGraceNote) { // JMI
    if (noteNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote) {
    // don't createSkipGraceNotesGroupClone() is there's only a single voice JMI ??? v0.9.67

      // issue #34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part

      // create the skip grace notes group
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "Creating a skip clone of grace notes group " <<
            elt->asShortString () <<
            " to work around LilyPond issue #34" << // JMI ??? v0.9.67
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

      fCurrentSkipGraceNotesGroup =
        elt->
          createSkipGraceNotesGroupClone ();
    }
  }

  // addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded() will
  // append the same skip grace notes to the ofhter voices if needed
  // in visitEnd (S_msrPart&)
}

void msr2msrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrGraceNotesGroup" <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "+++++++++++++++++++++++++ visitEnd (S_msrGraceNotesGroup&)" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      gLog << fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]";
    }
    gLog << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

//* JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Removing the after grace notes element from the current voice clone" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChordGraceNotesGroupLink " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrGraceNotesGroup
    originalGraceNotesGroup =
      elt->getOriginalGraceNotesGroup ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "--> visitStart (S_msrChordGraceNotesGroupLink&), " <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingChord) {
    const S_msrChordGraceNotesGroupLink&
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          elt->getInputLineNumber (),
          originalGraceNotesGroup,
          fCurrentChordClone);

    switch (originalGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkBefore (
            inputLineNumber,
            chordChordGraceNotesGroupLink);
        break;
      case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkAfter (
            inputLineNumber,
            chordChordGraceNotesGroupLink);
        break;
    } // switch
  }

  else {
    displayCurrentOnGoingValues ();

    std::stringstream ss;

    ss <<
      "grace notes group link is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingChordGraceNotesGroupLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
    S_msrGraceNotesGroup
      originalGraceNotesGroup =
        elt->getOriginalGraceNotesGroup ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChordGraceNotesGroupLink" <<
      elt->asShortString () <<
      ", originalGraceNotesGroup: " <<
      originalGraceNotesGroup->asShortString () <<
      ", fOnGoingNotesStack.size (): " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingChordGraceNotesGroupLink: " << fOnGoingChordGraceNotesGroupLink <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordGraceNotesGroupLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//  displayCurrentOnGoingValues (); // JMI

  // create the note clone
  S_msrNote
    noteClone =
      elt->
        createNoteNewbornClone (
          fCurrentPartClone);

  // register clone in this tranlastors' voice notes map and ongoing notes stack
  fVoiceNotesMap [elt] = noteClone; // JMI XXL
  fOnGoingNotesStack.push_front (noteClone);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) { // JMI
    displayOnGoingNotesStack ("visitStart (S_msrNote&)");
  }
#endif // MF_TRACE_IS_ENABLED

  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/* JMI
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            gLog <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            gLog <<
              "[NONE]";
          }
          gLog <<
             "'" <<
            std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "===> fCurrentNonGraceNoteClone: " <<
          fCurrentNonGraceNoteClone->asString () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotes ()) {
          std::stringstream ss;

          ss <<
            "The first note of voice clone JMI v0.9.66 " <<
            fCurrentVoiceClone->getVoiceName () <<
            " is " <<
            fFirstNoteCloneInVoice->asShortString () <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
        std::stringstream ss;

        ss <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber<<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      "FAA fCurrentNonGraceNoteClone: " <<
      std::endl;
    if (fCurrentNonGraceNoteClone) {
      gLog <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]" <<
        std::endl;
    }

    gLog <<
      "FAA fCurrentGraceNoteClone: " <<
      std::endl;
    if (fCurrentGraceNoteClone) {
      gLog <<
        fCurrentGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]" <<
        std::endl;
    }

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (noteKind) {

    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteSkipInMeasure: // JMI
#ifdef MF_TRACE_IS_ENABLED

      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteRegularInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      if (fOnGoingDoubleTremolo) {

        if (fCurrentNonGraceNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }

        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNonGraceNoteClone);
        }

        else {
          std::stringstream ss;

          ss <<
            "note '" << fCurrentNonGraceNoteClone->asShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msr2msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str ());
        }
      }

      else {
        std::stringstream ss;

        ss <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMI
      gLog <<
        "fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        std::endl;
        */

      if (fOnGoingGraceNotesGroup) { // JMI v0.9.66
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "Appending grace note " <<
            fCurrentGraceNoteClone->asShortString () <<
            " to the grace notes group " <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            " in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
       }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }
      else {
        std::stringstream ss;

        ss <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of any grace notes group";

//         msr2msrInternalError ( // JMI v0.9.67
        msr2msrInternalWarning(
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
//           __FILE__, __LINE__,
          ss.str ());
      }

    /* JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          gLog <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          gLog <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fPendingAfterGraceNotes->
          appendNoteToAfterGraceNotesContents (
            fCurrentNonGraceNoteClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "both fCurrentGraceNoteGroupsClone and fPendingAfterGraceNoteGroup are null," <<
          std::endl <<
          "cannot handle grace note'" <<
          elt->asString () <<
          "'";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
        mfAssert (
          __FILE__, __LINE__,
          fCurrentNonGraceNoteClone != nullptr,
          "fCurrentNonGraceNoteClone is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
     }

      else {
        std::stringstream ss;

        ss <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
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
        std::stringstream ss;

        ss <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletClonesStack.top ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fCurrentVoiceClone);
      break;
  } // switch

/* JMI
  // handle melisma
  msrSyllableExtendKind
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteOctaveShift (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordOctaveShift (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "octaveShift is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "stem is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

// JMI      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
// JMI      ", fOnGoingChord: " << fOnGoingChord <<

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordBeamLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrBeam originalBeam = elt->getOriginalBeam ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "--> visitStart (S_msrChordBeamLink&), " <<
      elt->asShortString () <<
      ", originalBeam: " <<
      originalBeam->asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    std::stringstream ss;

    ss <<
      "beam link is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingChordBeamLink = true;
}

void msr2msrTranslator::visitEnd (S_msrChordBeamLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordBeamLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChord " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  // set fCurrentChordClone's measure position  // JMI ??? v0.9.66
//   fCurrentMeasureClone->
//     setChordMeasurePosition (
//       mfRational (FOO, 1),
//       "msr2msrTranslator::visitStart (S_msrChord& elt)");

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
      std::stringstream ss;

      ss <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
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
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChord" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // finalize the current chord clone
  fCurrentChordClone->
    finalizeChord (
      inputLineNumber);

  // forget about the current chord clone
  fCurrentChordClone = nullptr;

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.push (tupletClone);
}

void msr2msrTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

    fTupletClonesStack.top ()->
      appendTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteTie (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTieToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "tie is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "segno is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2msrTranslator::visitStart (S_msrDalSegno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDalSegno" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "dal segno is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
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
        measurePosition);
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
      elt->getMeasurePosition ());
     // */
}

void msr2msrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "coda is out of context, cannot be handled: " << elt->asShortString ();

    msr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);
}

void msr2msrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendLineBreakToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendPageBreakToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2msrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->getPartCombinedName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending start in the voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending end in the voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMultipleFullBarRests& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMultipleFullBarRests" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a multiple full-bar rests clone
  fCurrentMultipleFullBarRestsClone =
    elt->
      createMultipleFullBarRestsNewbornClone (
        fCurrentSegmentClone);

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendMultipleFullBarRestsToVoice (
      inputLineNumber,
      fCurrentMultipleFullBarRestsClone);

  fOnGoingMultipleFullBarRests = true;
}

void msr2msrTranslator::visitEnd (S_msrMultipleFullBarRests& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMultipleFullBarRests" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current multiple full-bar rests
  fCurrentMultipleFullBarRestsClone = nullptr;

  fOnGoingMultipleFullBarRests = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

/* JMI
  // set last segment as the measures repeat pattern segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatPattern&)");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatPatternStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatPattern&) 1");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatPatternEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatReplicas&)");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasStartInVoiceClone (
      inputLineNumber);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;


#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatReplicas&) 1");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarLine" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Handling '" <<
      msrBarLineCategoryKindAsString (
        elt->getBarLineCategory ()) <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the barLine to the current voice clone
  fCurrentVoiceClone->
    appendBarLineToVoice (elt);
}

void msr2msrTranslator::visitEnd (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2msrTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace

//________________________________________________________________________
/* JMI
void msr2msrTranslator::prependSkipGraceNotesGroupToPartOtherVoices (
  S_msrPart            partClone,
  const S_msrVoice&           voiceClone,
  const S_msrGraceNotesGroup& skipGraceNotesGroup)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "--> prepending a skip graceNotesGroup clone " <<
        skipGraceNotesGroup->asShortString () <<
        " to voices other than \"" <<
        voiceClone->getVoiceName () << "\"" <<
        " in part " <<
        partClone->getPartCombinedName () <<
        ", line " << skipGraceNotesGroup->getInputLineNumber () <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  std::map<int, S_msrStaff>
    partStavesMap =
      partClone->
        getPartStaveNumbersToStavesMap ();

  for (
    std::map<int, S_msrStaff>::const_iterator i=partStavesMap.begin ();
    i!=partStavesMap.end ();
    ++i
  ) {
    std::list<S_msrVoice>
      staffAllVoicesVector =
        (*i).second->
          getStaffAllVoicesVector ();

    for (
      std::list<S_msrVoice>::const_iterator j=staffAllVoicesVector.begin ();
      j!=staffAllVoicesVector.end ();
      ++j
    ) {
      const S_msrVoice& voice = (*j);

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
//             msrRehearsalMarkKind::kRehearsalMarkNone,
//             elt->getWordsContents (),
//             elt->getWordsPlacementKind ()); // above ??? JMI
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceWords ()) {
//     std::stringstream ss;
//
//     ss <<
//           "Converting words '" <<
//           elt->asShortString () <<
//           "' to rehearsal mark '" <<
//           rehearsalMark->asShortString () <<
//           "'" <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the current voice clone
//       fCurrentVoiceClone->
//         appendRehearsalMarkToVoice (rehearsalMark);
//
//       wordsHasBeenHandled = true;
//     }
//
//     else {
//       std::string wordsContents = elt->getWordsContents ();
//
//       // is this words contents in the string to dal segno kind map?
//       const std::map<std::string, msrDalSegno::msrDalSegnoKind>&
//         converStringToDalSegnoMap =
//           gGlobalMsr2msrOahGroup->
//             getStringToDalSegnoKindMap ();
//
//       std::map<std::string, msrDalSegno::msrDalSegnoKind>::const_iterator
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
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceWords ()) {
//     std::stringstream ss;
//
//     ss <<
//             "Converting words '" <<
//             elt->asShortString () <<
//             "' to dal segno element '" <<
//             dalSegno->asShortString () <<
//             "'" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
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
