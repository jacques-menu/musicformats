/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>
#include <algorithm>    // for_each

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

// #include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrAfterGraceNotes.h"
#include "msrArticulations.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrGlissandos.h"
#include "msrLineBreaks.h"
#include "msrPageBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrSlides.h"
#include "msrTablatures.h"
#include "msrTechnicals.h"
#include "msrTempos.h"
#include "msrTuplets.h"
#include "msrVoiceStaffChanges.h"

#include "msr2msrWae.h"

#include "msr2msrComponent.h"

#include "msr2msrTranslator.h"

#include "oahOah.h"

#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "msrPathToVoice.h" // JMI TEMP TEST 0.9.66

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLineDescr msrHiddenMeasureAndBarLineDescr::create (
  const mfInputLineNumber& inputLineNumber,
  S_msrDalSegno            dalSegno)
{
  msrHiddenMeasureAndBarLineDescr* obj = new
    msrHiddenMeasureAndBarLineDescr (
      inputLineNumber,
      dalSegno);
  assert (obj != nullptr);
  return obj;
}

msrHiddenMeasureAndBarLineDescr::msrHiddenMeasureAndBarLineDescr (
  const mfInputLineNumber& inputLineNumber,
  S_msrDalSegno            dalSegno)
{
  fInputLineNumber = inputLineNumber;
  fDalSegno = dalSegno;
}

msrHiddenMeasureAndBarLineDescr::~msrHiddenMeasureAndBarLineDescr ()
{}

std::string msrHiddenMeasureAndBarLineDescr::hiddenMeasureAndBarLineDescrAsString () const
{
  std::stringstream ss;

  ss <<
    "hiddenMeasureAndBarLineDescr" <<
    ", fInputLineNumber: " << fInputLineNumber <<
    ", dalSegno: " << fDalSegno->asString ();

  return ss.str ();
}

void msrHiddenMeasureAndBarLineDescr::print (std::ostream& os) const
{
  constexpr int fieldWidth = 14;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fInputLineNumber" << ": " <<
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create the resulting new MSR score
  fResultingNewMsrScore =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      "msrScore::create()");

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // set the parts browsing order
  theMsrScore->
    setStavesBrowingOrderKind (
      msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses);

  // browse the visited MSR score
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    pathToVoice != nullptr,
    "pathToVoice is NULL");

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

  // set the parts browsing order
  theMsrScore->
    setStavesBrowingOrderKind (
      msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesFiguredBassesRegulars);

  // browse the visited MSR score
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

  constexpr int fieldWidth = 25;

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

//______________________________________________________________________________
void msr2msrTranslator::displayPartGroupsStack (
  const std::string& context)
{
  size_t partGroupsStackSize = fPartGroupsStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The part groups stack contains " <<
    mfSingularOrPlural (
      partGroupsStackSize, "element", "elements") <<
    " (" << context << "):";

  if (partGroupsStackSize) {
    gLog << std::endl;

    ++gIndenter;

    int counter = partGroupsStackSize - 1;
    for (S_msrPartGroup partGroup:fPartGroupsStack) {
      gLog <<
        counter << ": " << 'v' <<
        std::endl;

      ++gIndenter;
      gLog <<
        gTab << partGroup->asString () <<
        std::endl;
      --gIndenter;

      --counter;
    } // for

    --gIndenter;
  }

  else {
    gLog <<
      " [EMPTY]" <<
      std::endl;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

//________________________________________________________________________
void msr2msrTranslator::displayPartHiddenMeasureAndBarLineDescrList ()
{
  gLog <<
    "fPartHiddenMeasureAndBarLineDescrList:" <<
    std::endl;

  if (! fPartHiddenMeasureAndBarLineDescrList.empty ()) {
    ++gIndenter;

    constexpr int fieldWidth = 19;

    std::list <S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      S_msrDalSegno
        dalSegno =
          hiddenMeasureAndBarLineDescr->getDalSegno ();

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "inputStartLineNumber" << ": " <<
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
    mfSingularOrPlural (
      onGoingNotesStackSize, "element", "elements") <<
    " (context: " << context << "):" <<
    std::endl;

  if (onGoingNotesStackSize) {
    std::list <S_msrNote>::const_iterator
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
      gLog << note << std::endl;
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

  if (! fPartHiddenMeasureAndBarLineDescrList.empty ()) {
    std::list <S_msrHiddenMeasureAndBarLineDescr>::const_iterator
      iBegin = fPartHiddenMeasureAndBarLineDescrList.begin (),
      iEnd   = fPartHiddenMeasureAndBarLineDescrList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHiddenMeasureAndBarLineDescr
        hiddenMeasureAndBarLineDescr = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        hiddenMeasureAndBarLineDescr != nullptr,
        "hiddenMeasureAndBarLineDescr is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScore" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // populate the measure numbers
  fResultingNewMsrScore->
    setScoreFirstMeasureNumber (
      elt->getScoreFirstMeasureNumber ());
  fResultingNewMsrScore->
    setScoreLastMeasureNumber (
      elt->getScoreLastMeasureNumber ());
}

void msr2msrTranslator::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScore" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set the current identification
  fCurrentIdentification = elt;

  // store it in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create a scaling clone
  S_msrScaling
    scalingClone =
      elt->createMsrScalingNewbornClone ();

  // register it in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the aystem layout in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the staff layout in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the appearance in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the page layout in the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = elt;

  // set elt as credit of the resulting new MSR score
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->fetchPartGroupInformationForTrace () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a partGroup clone
  // the current partGroup clone, i.e. the top of the stack,
  // is the upLink of the new one if it exists
  S_msrPartGroup
    partGroupClone =
      elt->createPartGroupNewbornClone (
        ! fPartGroupsStack.empty ()
          ? fPartGroupsStack.front ()
          : nullptr,
        fResultingNewMsrScore);

  // is this part group the implicit outer-most one?
  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Adding part group clone " <<
          partGroupClone->fetchPartGroupInformationForTrace () <<
          " to the resulting new MSR score" <<
          ", line " << elt->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fResultingNewMsrScore->
        addPartGroupToScore (
          partGroupClone);
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      break;
  } // switch

  // push partGroupClone onto this visitors's stack,
  // making it the current partGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Pushing part group clone " <<
      partGroupClone->fetchPartGroupInformationForTrace () <<
      " onto the stack" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.push_front (
    partGroupClone);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupsStack (
      "visitStart (S_msrPartGroup& elt)");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2msrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPartGroup " <<
      elt->fetchPartGroupInformationForTrace () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fPartGroupsStack.empty ()) {
    // pop current partGroup from this visitors's stack
    // since it is elt itself
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
      std::stringstream ss;

      ss <<
        "Popping part group clone " <<
        fPartGroupsStack.front ()->fetchPartGroupInformationForTrace () <<
        " from the stack" <<
        ", line " << elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartGroupsStack.pop_front ();

    // handle part group type
    switch (elt->getPartGroupImplicitKind ()) {
      case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
        // nothing to do here
        break;

      case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
        {
          S_msrPartGroup
            newPartGroupStackTop =
              fPartGroupsStack.front ();

          // append elt to its containing part group
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTracePartGroups ()) {
            std::stringstream ss;

            ss <<
              "Appending part group clone " <<
              elt->fetchPartGroupInformationForTrace () <<
              " to the current part group " <<
              newPartGroupStackTop->fetchPartGroupInformationForTrace () <<
              ", line " << elt->getInputLineNumber ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          newPartGroupStackTop-> // JMI 0.9.69
            appendNestedPartGroupToPartGroup (
              elt);
        }
        break;
    } // switch
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupsStack (
      "visitEnd (S_msrPartGroup& elt)");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  std::string
    partdNameForTrace =
      elt->fetchPartNameForTrace ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPart " <<
      partdNameForTrace <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (
    gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceParts ()
    ) {
      std::stringstream ss;

      ss <<
  //       std::endl <<
        "<!--=== part \"" << partdNameForTrace << "\"" <<
        ", line " << elt->getInputLineNumber () << " ===-->";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentPartIDAndName (
      elt->fetchPartIDAndName ());

  ++gIndenter;

  // create a part clone
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.front ());

  // add it to the partGroup clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Adding part clone " <<
      fCurrentPartClone->fetchPartNameForTrace () <<
      " to part group clone \"" <<
      fPartGroupsStack.front ()->fetchPartGroupInformationForTrace () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.front ()->
    appendPartToPartGroup (fCurrentPartClone);
}

void msr2msrTranslator::visitEnd (S_msrPart& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPart " <<
      elt->fetchPartNameForTrace () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    partInstrumentAbbreviation =
      fCurrentPartClone->
        getPartInstrumentAbbreviation ();

  // populate part instrument short name if empty and possible
  if (partInstrumentAbbreviation.empty ()) {
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
      elt->getInputLineNumber ());

  // handle the hidden measure and barLine elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    displayPartHiddenMeasureAndBarLineDescrList ();
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI handlePartHiddenMeasureAndBarLineDescrList ();

  // handle skip grace notes if needed ISSUE34
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->getStaffPathLikeName () << "\"" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current staff number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentStaveNumber (
      elt->getStaffNumber ());

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
      elt->getStaffPathLikeName () << "\"" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string voiceName = elt->getVoiceName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== voice \"" << voiceName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember current voice
  fCurrentVoiceOriginal = elt;

  // register the current voice number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentVoiceNumber (
      fCurrentVoiceOriginal->getVoiceNumber ());

  ++gIndenter;

  // create a voice clone
  fCurrentVoiceClone =
    fCurrentVoiceOriginal->
      createVoiceNewbornClone (
        fCurrentStaffClone);

  // add it to the staff clone
  fCurrentStaffClone->
    registerVoiceInStaffClone (
      elt->getInputLineNumber (),
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
          fCurrentVoiceOriginal->getVoiceIsMusicallyEmpty () // superfluous test ??? JMI 0.9.70
        ) {
//           // create a ChordNames context
//           std::string partdNameForTrace =
//             fCurrentVoiceOriginal->
//               fetchVoiceUpLinkToPart ()->
//                 fetchPartNameForTrace ();
//
// #ifdef MF_TRACE_IS_ENABLED
//           if (gTraceOahGroup->getTraceHarmonies ()) {
//             std::stringstream ss;
//
//             ss <<
//               "Creating a ChordNames context for \"" << voiceName <<
//               "\" in part " << partdNameForTrace <<
//               std::endl;
//
//             gWaeHandler->waeTraceWithoutInputLocation (
//               __FILE__, mfInputLineNumber (__LINE__),
//               ss.str ());
//           }
// #endif // MF_TRACE_IS_ENABLED

        }
      }

      fOnGoingHarmoniesVoice = true;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // register it as the part figured bass voice
        fCurrentPartClone->
          setPartFiguredBassVoice (
            fCurrentVoiceClone);

        if (
          fCurrentVoiceOriginal->getVoiceIsMusicallyEmpty () // superfluous test ??? JMI 0.9.70
        ) {
//           // create a FiguredBass context
//           std::string partdNameForTrace =
//             fCurrentVoiceOriginal->
//               fetchVoiceUpLinkToPart ()->
//                 fetchPartNameForTrace ();
//
// #ifdef MF_TRACE_IS_ENABLED
//           if (gTraceOahGroup->getTraceHarmonies ()) {
//             std::stringstream ss;
//
//             ss <<
//               "Creating a FiguredBass context for \"" << voiceName <<
//               "\" in part " << partdNameForTrace <<
//               std::endl;
//
//             gWaeHandler->waeTraceWithoutInputLocation (
//               __FILE__, mfInputLineNumber (__LINE__),
//               ss.str ());
//           }
// #endif // MF_TRACE_IS_ENABLED
        }
      }

      fOnGoingFiguredBassVoice = true;
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->asString () << '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->getSegmentAbsoluteNumber () << '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleSegmentCloneEndInVoiceClone (
      elt->getInputLineNumber (),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
    // this has been done in pass2b // JMI ??? 0.9.66
  }

  else if (fOnGoingChord) {
    // append the harmony clone to the current voice clone
    fCurrentChordClone->
      appendHarmonyToChord (
        fCurrentHarmonyClone); // JMI 0.9.66
  }

  else if (fOnGoingHarmoniesVoice) {
    // append the harmony clone to the current voice clone
    fCurrentVoiceClone->
      cascadeAppendHarmonyToVoiceClone (
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

    msr2msrInternalError ( // JMI 0.9.67
//     msr2msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

    // don't append the figured bass to the part figured bass, JMI ??? 0.9.66
    // this will be done below
  }

  else if (fOnGoingChord) {
    // register the figured bass clone in the current chord clone
    fCurrentChordClone->
      setChordFiguredBassesList (
        fCurrentFiguredBassClone); // JMI ??? 0.9.66
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  mfMeasureNumber
    fCurrentMeasureNumber =
      elt->getMeasureNumber ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasure '" <<
      fCurrentMeasureNumber <<
      "', measurePuristNumber: '" <<
      measurePuristNumber <<
      "', line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      " (partMusicXMLID \"" << fCurrentPartClone->getPartMusicXMLID () << "\")" <<
      ", measure \"" << fCurrentMeasureNumber << "\"" <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current measure number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (
      fCurrentMeasureNumber);

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClone);

  if (fOnGoingMultipleMeasureRests) {
    // append current measure clone to the current multiple measure rests clone
    fCurrentMultipleMeasureRestsClone->
      appendMeasureToMultipleMeasureRest (
        fCurrentMeasureClone);
  }
  else {
    // append current measure clone to the current voice clone
    fCurrentVoiceClone->
      appendMeasureCloneToVoiceClone (
        elt->getInputLineNumber (),
        fCurrentMeasureClone);
  }

  // JMI superflous ???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      fCurrentMeasureNumber);

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
  mfMeasureNumber
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
      fCurrentMeasureNumber <<
      "', nextMeasureNumber: '" <<
      nextMeasureNumber <<
      "', measurePuristNumber: '" <<
      measurePuristNumber <<
      "', line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // finalize the current measure clone
  fCurrentMeasureClone->
    finalizeMeasureClone (
      elt->getInputLineNumber (),
      elt, // original measure
      fCurrentVoiceClone,
      "msr2msrTranslator::visitEnd (S_msrMeasure&)");

  Bool doCreateABarCheck (false); // JMI ??? 0.9.70

  switch (elt->getMeasureKind ()) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
        std::stringstream ss;

        ss <<
          "measure '" << fCurrentMeasureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureUpLinkToVoice ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2msrTranslator";

        msr2msrInternalError (
//         msr2msrInternalWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
         __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      {
        // fetch the measure whole notes duration from the current measure clone
        mfWholeNotes
          fullMeasureWholeNotesDuration =
            fCurrentMeasureClone->
              getFullMeasureWholeNotesDuration ();

        // get the current voice clone time signature
        S_msrTimeSignature
          voiceCurrentTimeSignature =
            fCurrentVoiceClone->
              getVoiceCurrentTimeSignature ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          voiceCurrentTimeSignature != nullptr,
          "voiceCurrentTimeSignature is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        mfWholeNotes
          wholeNotesPerMeasure =
            voiceCurrentTimeSignature->
              timeSignatureWholeNotesPerMeasure ();

#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceBarChecks ()
            ||
          gTraceOahGroup->getTraceBarNumberChecks ()
        ) {
          std::stringstream ss;

          ss <<
            "--> kMeasureKindRegular" <<
            ", fullMeasureWholeNotesDuration: " <<
            fullMeasureWholeNotesDuration.asString () <<
            "', wholeNotesPerMeasure: " <<
            wholeNotesPerMeasure;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        doCreateABarCheck = true;
      }
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
    case msrMeasureKind::kMeasureKindIncompleteLastMeasure: // JMI ??? 0.9.67
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
//           doCreateABarCheck = true; // JMI ??? 0.9.70
          break;
        case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
          break;
      } // switch
      break;

    case msrMeasureKind::kMeasureKindOverFlowing:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      // JMI
      break;
  } // switch

  // is this a measure rest?
  if (elt->getMeasureIsAMeasureRest ()) {
    // yes JMI
  }

  else {
    // no

    // should we compress measure rests?
//    if (gGlobalMsr2msrOahGroup->getTraceMultipleMeasureRests ()) {
//       // yes
//
//       if (fCurrentMultipleMeasureRests) {
//         // append the current multiple measure rests to the current voice clone
//         fCurrentVoiceClone->
//           cascadeAppendMultipleMeasureRestToVoice (
//             elt->getInputLineNumber (),
//             fCurrentMultipleMeasureRests);
//
//         // forget about the current rest measure
//         fCurrentRestMeasure = nullptr;
//
//         // forget about the current multiple measure rests
//         fCurrentMultipleMeasureRests = nullptr;
//       }
//
//       else {
//         std::stringstream ss;
//
//         ss <<
//           "fCurrentMultipleMeasureRests is null upon multiple measure rest end" <<
//           fCurrentMeasureNumber <<
//           "', measurePuristNumber: '" <<
//           measurePuristNumber <<
//           "', line " << elt->getInputLineNumber ();
//
// /* JMI ???
//         msr2msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           elt->getInputLineNumber (),
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//           */
//       }
//     }
  }

  // is fCurrentMeasureNumber in the parts ignore IDs set?
  if (! gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().empty ()) {
    std::set <std::string>::iterator
      it =
        gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().find (
          fCurrentMeasureNumber);

    if (it != gGlobalMsr2msrOahGroup->getInserPageBreakAfterMeasureSet ().end ()) { // JMI 0.9.70
      gLog <<
        "==> adding a page break after measureNumber: \"" <<
        fCurrentMeasureNumber <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      S_msrPageBreak
        pageBreak =
          msrPageBreak::create (
            elt->getInputLineNumber (),
            fCurrentMeasureClone,
            fCurrentMeasureClone->getMeasurePuristNumber (),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

//   // set the syllable's measure uplink
//   syllable->
//     setSyllableUpLinkToMeasure (fCurrentMeasureClone);

  // append it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JMI
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanzaClone (
        fCurrentSyllableClone,
        fCurrentMeasureClone);
  }

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note

    // append syllable to currentNote
    fCurrentNonGraceNoteClone->
      appendSyllableToNote (
        fCurrentSyllableClone);

    // set syllable upLink to note
    fCurrentSyllableClone->
      setSyllableUpLinkToNote (fCurrentNonGraceNoteClone);

//     if (gGlobalMsr2msrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) { JMI ???
//       // get the syllable texts list
//       const std::list <std::string>&
//         syllableElementsList =
//           elt->getSyllableElementsList ();
//
//       if (! syllableElementsList.empty ()) {
//         // build a single words value from the texts list
//         // JMI create an msrWords instance for each???
//         std::string wordsValue =
//           elt->syllableElementsListAsString();
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
//             '\'' <<
//       // JMI      fCurrentSyllableClone->asString () <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         S_msrWords
//           words =
//             msrWords::create (
//               elt->getInputLineNumber (),
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
//             '\'' <<
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    /* JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          elt->getInputLineNumber (),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrClefKeyTimeSignatureGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrClefKeyTimeSignatureGroup " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an msrClefKeyTimeSignatureGroup clone
  fCurrentClefKeyTimeSignatureGroup =
    elt->
      createClefKeyTimeSignatureGroupNewbornClone ();

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendClefKeyTimeSignatureGroupToVoice (
      fCurrentClefKeyTimeSignatureGroup);

//   fCurrentStaffClone->
//     appendClefKeyTimeSignatureGroupToStaffClone (
//       fCurrentClefKeyTimeSignatureGroup);
}

void msr2msrTranslator::visitEnd (S_msrClefKeyTimeSignatureGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrClefKeyTimeSignatureGroup" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "--> Start visiting msrClef " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // store clef in fCurrentClefKeyTimeSignatureGroup
  fCurrentClefKeyTimeSignatureGroup->
    setClef (elt);

//   fCurrentStaffClone->
//     appendClefToStaff (
//       fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber (),
//       elt);
}

void msr2msrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "--> Start visiting msrKey " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // store key in fCurrentClefKeyTimeSignatureGroup
  fCurrentClefKeyTimeSignatureGroup->
    setKey (elt);

//   fCurrentStaffClone->
//     appendKeyToStaff (
//       fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber (),
//       elt);
}

void msr2msrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // store time signature in fCurrentClefKeyTimeSignatureGroup
  fCurrentClefKeyTimeSignatureGroup->
    setTimeSignature (elt);

//   fCurrentStaffClone->
//     appendTimeSignatureToStaff (
//       fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber (),
//       elt);
}

void msr2msrTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
//         '\'' <<
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

// JMI 0.9.61
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
//             elt->getInputLineNumber (),
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
//           '\'' <<
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
//       __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fOnGoingChordSlurLink;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /* JMI ???
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequence:
    these should be ignored
  */

  if (! fOnGoingNotesStack.empty ()) {
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
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      originalSlur->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   Bool doCreateAGraceNoteClone (true); // JMI
//
//   if (doCreateAGraceNoteClone) {
//     // create a clone of this graceNotesGroup
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceGraceNotes ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Creating a clone of grace notes group " <<
//         elt->asString () <<
//         " and attaching it to clone note " <<
//         fCurrentNonGraceNoteClone->asShortString () <<
//         std::endl;
//
//         gWaeHandler->waeTrace (
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED

    fCurrentGraceNotesGroupClone =
      elt->
        createGraceNotesGroupNewbornClone ();

    // attach fCurrentGraceNotesGroupClone to the current chord clone ??? JMI 0.9.72
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

    // attach fCurrentGraceNotesGroupClone to the current note clone
    if (! fOnGoingNotesStack.empty ()) {
      switch (elt->getGraceNotesGroupKind ()) {
        case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
      //    fCurrentNonGraceNoteClone-> JMI 0.9.67
          fOnGoingNotesStack.front ()->
            setGraceNotesGroupBeforeNote (
              fCurrentGraceNotesGroupClone);
          break;
        case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
      //    fCurrentNonGraceNoteClone-> JMI 0.9.67
          fOnGoingNotesStack.front ()->
            setGraceNotesGroupAfterNote (
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
        "graceNotesGroup is out of context, fOnGoingNotesStack.empty (), cannot be handled: " <<
        elt->asShortString ();

      msr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
//   }

  fOnGoingGraceNotesGroup = true;
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      gLog << "[NULL]";
    }
    gLog << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//* JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Removing the after grace notes element from the current voice clone";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     fCurrentVoiceClone->
//       removeElementFromVoice (
//         elt->getInputLineNumber (),
//         fCurrentAfterGraceNotesGroupElement);

    // forget about the current after grace notes element
    fCurrentAfterGraceNotesGroupElement = nullptr;

    // forget about these after the pending grace notes
    fPendingAfterGraceNotesGroup = nullptr;
  }

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChordGraceNotesGroupLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
        fOnGoingChordGraceNotesGroupLink;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
            elt->getInputLineNumber (),
            chordChordGraceNotesGroupLink);
        break;
      case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
        fCurrentChordClone->
          setChordGraceNotesGroupLinkAfter (
            elt->getInputLineNumber (),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "--> Start visiting msrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//  displayCurrentOnGoingValues (); // JMI

  // create the note clone
  fCurrentNoteClone =
    elt->
      createNoteNewbornClone (
        fCurrentPartClone);

  // register clone in this tranlastors' voice notes map and ongoing notes stack
  fVoiceNotesMap [elt] = fCurrentNoteClone; // JMI XXL
  fOnGoingNotesStack.push_front (fCurrentNoteClone);

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
              "[NULL]";
          }
          gLog <<
             '\'' <<
            std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
*/

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      fCurrentGraceNoteClone = fCurrentNoteClone;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "===> fCurrentGraceNoteClone: " <<
          fCurrentGraceNoteClone->asString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    default:
      fCurrentNonGraceNoteClone = fCurrentNoteClone;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "===> fCurrentNonGraceNoteClone: " <<
          fCurrentNonGraceNoteClone->asString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
            "The first note of voice clone JMI 0.9.66 " <<
            fCurrentVoiceClone->getVoiceName () <<
            " is " <<
            fFirstNoteCloneInVoice->asShortString ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }

      fOnGoingNonGraceNote = true;
  } // switch

//* JMI 0.9.70
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()
  ) {
    // yes, create the after grace notes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
        elt->getInputLineNumber (),
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
  msrNoteKind
    noteKind = elt->getNoteKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    gLog <<
      "FAA fCurrentNonGraceNoteClone: " << // JMI 0.9.71
      std::endl;
    if (fCurrentNonGraceNoteClone) {
      gLog <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NULL]" <<
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
        "[NULL]" <<
        std::endl;
    }

    gLog << std::endl;
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
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
              __FILE__, mfInputLineNumber (__LINE__),
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
              __FILE__, mfInputLineNumber (__LINE__),
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
            elt->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
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
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMI
      gLog <<
        "fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        std::endl;
        */

//       if (fOnGoingGraceNotesGroup) { // JMI 0.9.66
      if (fCurrentGraceNotesGroupClone) { // JMI 0.9.66
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "visitEnd (S_msrNote& elt): Appending grace note " <<
            fCurrentGraceNoteClone->asShortString () <<
            " to the grace notes group " <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            " in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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

//         msr2msrInternalError ( // JMI 0.9.67
        msr2msrInternalWarning(
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
//           __FILE__, mfInputLineNumber (__LINE__),
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
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      */
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fOnGoingChord) {
        // append fCurrentNonGraceNoteClone to fCurrentChordClone
        fCurrentChordClone->
          appendNoteToChord (
            fCurrentNonGraceNoteClone,
            fCurrentVoiceClone);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          fCurrentNonGraceNoteClone != nullptr,
          "fCurrentNonGraceNoteClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        if (! fCurrentChordHasBeenPopulatedFromItsFirstNote) {
          // copy the current note's values to the current chord
          fCurrentChordClone->
            copyNoteValuesToChord (
              fCurrentNoteClone);

          // append current chord clone to the current voice,
          // only now that its duration is known
                                              //           fCurrentVoiceClone->
                                              //             appendChordToVoice (
                                              //               fCurrentChordClone);

//           // increment the current measure's accumulated duration
//           // by the current chord's duration
//           fCurrentMeasureClone->
//             incrementMeasureCurrentPositionInMeasure (
//               elt->getInputLineNumber (),
//               fCurrentNoteClone->getMeasureElementSoundingWholeNotes (),
//               "msr2msrTranslator::visitEnd (S_msrNote& elt) 2: "
//                 +
//               fCurrentChordClone->asShortString ());

          fCurrentChordHasBeenPopulatedFromItsFirstNote = true;
        }
     }

      else {
// #ifdef MF_TRACE_IS_ENABLED
//     if (true) { // JMI 0.9.72 ???
//       gLog <<
//         std::endl <<
//         std::endl <<
//         "&&&&&&&&&&&&&&&&&& msr2msrTranslator::visitEnd (S_msrNote& elt):" <<
//         std::endl;
//
//       fCurrentChordClone->print (gLog);
//
//       gLog <<
//         std::endl <<
//         "&&&&&&&&&&&&&&&&&&" <<
//         std::endl <<
//         std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED

        std::stringstream ss;

        ss <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): kNoteRegularInChord note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fOnGoingChord) {
        // append fCurrentGraceNoteClone to fCurrentVoiceClone
        fCurrentChordClone->
          appendNoteToChord (
            fCurrentGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "msr2msrTranslator::visitEnd (S_msrNote& elt): kNoteInChordInGraceNotesGroup note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotesBasics ()) {
        std::stringstream ss;

        ss <<
          "Appending note clone " <<
          fCurrentNonGraceNoteClone->asShortString () <<
          " to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletClonesStack.front ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone);

      fCurrentVoiceClone->
        registerTupletNoteInVoice (fCurrentNonGraceNoteClone);

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
              elt->getInputLineNumber (),
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
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    default:
      fOnGoingNonGraceNote = false;
  } // switch

  // copy note's elements if any to the current chord if any
  if (fCurrentChordClone) {
    fCurrentChordClone->
      copyNoteElementsIfAnyToChord (
        fCurrentNoteClone);
  }

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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceStems ()) {
        std::stringstream ss;

        ss <<
          "--> visitStart (S_msrStem& elt)" <<
          ", fOnGoingNonGraceNote: " <<
          fOnGoingNonGraceNote <<
          ", fOnGoingChord: " <<
          fOnGoingChord <<
          ", line " << elt->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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

//   if (fOnGoingChord) { // else ??? JMI
//     // don't append a beam if we're inside a beam link JMI ???
//     if (fOnGoingNonGraceNote) {
//       S_msrChordBeamLink
//         chordBeamLink =
//           msrChordBeamLink::create (
//             fCurrentChordClone->getInputLineNumber (),
//             elt,
//             fCurrentChordClone);
//
//       fCurrentChordClone->
//         appendChordBeamLinkToChord (chordBeamLink);
//     }
//   }
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      originalBeam->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordBeamLink = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChord " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  // register it as not yet populated fron its first note
  fCurrentChordHasBeenPopulatedFromItsFirstNote = false;

  // set fCurrentChordClone's measure position  // JMI ??? 0.9.66
//   fCurrentMeasureClone->
//     setChordPositionInMeasure (
//       mfRational (FOO, 1),
//       "msr2msrTranslator::visitStart (S_msrChord& elt)");

  if (! fTupletClonesStack.empty ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.front ()->
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
        elt->getInputLineNumber (),
        __FILE__, mfInputLineNumber (__LINE__),
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
    // DON'T append current chord chone to the current voice clone yet,
    // wait until its first note is appended to it,
    // i.e. its duration is known
    // append current chord clone to the current voice
    fCurrentVoiceClone->
      appendChordToVoice (
        fCurrentChordClone);
  }

  fOnGoingChord = true;
}

void msr2msrTranslator::visitEnd (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // finalize the current chord clone
//   fCurrentChordClone->
//     finalizeChord (
//       elt->getInputLineNumber ());

  if (! fTupletClonesStack.empty ()) {
//     // append current chord clone to the current, innermost tuplet
//     fTupletClonesStack.front ()->
//       appendChordToTuplet (
//         fCurrentChordClone);
  }

  else if (fCurrentGraceNotesGroupClone) {
//     // append the chord to the grace notes
//     fCurrentGraceNotesGroupClone->
//       appendChordToGraceNotesGroup (
//         fCurrentChordClone);
  }

  else {
//     // append current chord clone to the current voice
//     fCurrentVoiceClone->
//       appendChordToVoice (
//         fCurrentChordClone);
  }

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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the tuplet clone
  const S_msrTuplet&
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Pushing tuplet " <<
      tupletClone->asString () <<
      " to tuplets stack";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.push_front (tupletClone);
}

void msr2msrTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping tuplet " <<
      elt->asString () <<
      " from tuplets stack";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.pop_front ();

  if (! fTupletClonesStack.empty ()) {
    // tuplet is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
        "Adding nested tuplet " <<
      elt->asString () <<
        " to stack top tuplet '" <<
      fTupletClonesStack.front ()->asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

    fTupletClonesStack.front ()->
      appendTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
        "Adding top level tuplet " <<
      elt->asString () <<
      " to voice" <<
      fCurrentVoiceClone->getVoiceName ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTieToNote (elt);
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

void msr2msrTranslator::visitStart (S_msrDalSegno& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDalSegno" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

/* JMI
  // create a hidden measure and barLine
  S_msrHiddenMeasureAndBarLine
    hiddenMeasureAndBarLine =
      msrHiddenMeasureAndBarLine::create (
        elt->getInputLineNumber ());

  // create a hidden measure and barLine descr
  S_msrHiddenMeasureAndBarLineDescr
    hiddenMeasureAndBarLineDescr =
      msrHiddenMeasureAndBarLineDescr::create (
        elt->getInputLineNumber (),
        positionInMeasure);
*/

  // register it in the hidden measure and barLine descr list
  fPartHiddenMeasureAndBarLineDescrList.push_back (
    msrHiddenMeasureAndBarLineDescr::create (
      elt->getInputLineNumber (),
       elt));

//* JMI BLARK
  fCurrentPartClone->
    insertHiddenMeasureAndBarLineInPartClone (
      elt->getInputLineNumber (),
      elt->getMeasureElementPositionInMeasure ());
     // */
}

void msr2msrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatStartInVoiceClone (
      elt->getInputLineNumber (),
      elt);
}

void msr2msrTranslator::visitEnd (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->fetchPartNameForTrace () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndInVoiceClone (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      elt->getInputLineNumber ());
}

void msr2msrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending start in the voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndingStartInVoiceClone (
      elt->getInputLineNumber (),
      elt->getRepeatEndingKind (),
      elt->getRepeatEndingNumber ());
}

void msr2msrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending end in the voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoiceClone (
      elt->getInputLineNumber (),
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMultipleMeasureRest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMultipleMeasureRest" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a multiple measure rests clone
  fCurrentMultipleMeasureRestsClone =
    elt->
      createMultipleMeasureRestNewbornClone (
        fCurrentSegmentClone);

  // append it to the current voice clone
  fCurrentVoiceClone->
    cascadeAppendMultipleMeasureRestToVoice (
      elt->getInputLineNumber (),
      fCurrentMultipleMeasureRestsClone);

  fOnGoingMultipleMeasureRests = true;
}

void msr2msrTranslator::visitEnd (S_msrMultipleMeasureRest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMultipleMeasureRest" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current multiple measure rests
  fCurrentMultipleMeasureRestsClone = nullptr;

  fOnGoingMultipleMeasureRests = false;
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatStartInVoiceClone (
      elt->getInputLineNumber (),
      elt);
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatEndInVoiceClone (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    fCurrentVoiceClone->
      displayVoice (
        elt->getInputLineNumber (),
        "Upon visitStart (S_msrMeasureRepeatPattern&)");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatPatternStartInVoiceClone (
      elt->getInputLineNumber ());
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    fCurrentVoiceClone->
      displayVoice (
        elt->getInputLineNumber (),
        "Upon visitEnd (S_msrMeasureRepeatPattern&) 1");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatPatternEndInVoiceClone (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    fCurrentVoiceClone->
      displayVoice (
        elt->getInputLineNumber (),
        "Upon visitStart (S_msrMeasureRepeatReplicas&)");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasStartInVoiceClone (
      elt->getInputLineNumber ());
}

void msr2msrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_msrMeasureRepeatReplicas" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;


#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    fCurrentVoiceClone->
      displayVoice (
        elt->getInputLineNumber (),
        "Upon visitEnd (S_msrMeasureRepeatReplicas&) 1");
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasEndInVoiceClone (
      elt->getInputLineNumber ());
}

//________________________________________________________________________
void msr2msrTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarLine" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fCurrentVoiceClone->getVoiceName () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
        partClone->fetchPartNameForTrace () <<
        ", line " << skipGraceNotesGroup->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  std::map <int, S_msrStaff>
    partStavesMap =
      partClone->
        getPartStavesMap ();

  for (
    std::map <int, S_msrStaff>::const_iterator i = partStavesMap.begin ();
    i != partStavesMap.end ();
    ++i
  ) {
    std::list <S_msrVoice>
      staffVoicesVector =
        (*i).second->
          getStaffAllVoicesList ();

    for (
      std::list <S_msrVoice>::const_iterator j=staffVoicesVector.begin ();
      j!=staffVoicesVector.end ();
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
//             elt->getInputLineNumber (),
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
//           '\'' <<
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
//       const std::map <std::string, msrDalSegno::msrDalSegnoKind>&
//         converStringToDalSegnoMap =
//           gGlobalMsr2msrOahGroup->
//             getStringToDalSegnoKindMap ();
//
//       std::map <std::string, msrDalSegno::msrDalSegnoKind>::const_iterator
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
//               elt->getInputLineNumber (),
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
//             '\'' <<
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


  // is this a measure rest? JMI 0.9.67
//   if (elt->getMeasureIsAMeasureRest ()) {
//     // yes
//
    // should we compress measure rests?
//     if (gGlobalMsr2msrOahGroup->getCompressMeasureRests ()) {//
//       // yes
//
//       if (! fCurrentRestMeasure) {
//         // this is the first multiple measure rest in the sequence
//
//         // create a multiple measure rests  containing fCurrentMeasureClone
//         fCurrentMultipleMeasureRests =
//           msrMultipleMeasureRest::create (
//             elt->getInputLineNumber (),
//             fCurrentMeasureClone,
//             fCurrentVoiceClone);
//
// /* JMI
//         // append the current multiple measure rests to the current voice clone
//         fCurrentVoiceClone->
//           cascadeAppendMultipleMeasureRestToVoice (
//             elt->getInputLineNumber (),
//             fCurrentMultipleMeasureRests);
//             */
//       }
//
//       else {
//         // this is a subsequent multiple measure rest, merely append it
//         fCurrentMultipleMeasureRests->
//           appendMeasureCloneToMultipleMeasureRests (
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
//           elt->getInputLineNumber (),
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
//         elt->getInputLineNumber (),
//         fCurrentMeasureClone);
//   }

/* JMI
  // append current measure clone to the current voice clone
  fCurrentVoiceClone->
    appendMeasureCloneToVoiceClone (
      elt->getInputLineNumber (),
      fCurrentMeasureClone);
*/
