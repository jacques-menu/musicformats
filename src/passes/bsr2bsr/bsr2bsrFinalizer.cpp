/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfConstants.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsr2bsrFinalizer.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
bsr2bsrFinalizer::bsr2bsrFinalizer (
  const S_bsrOahGroup& bsrOpts,
  const S_bsrScore&     originalBsrScore)
{
  fBsrOahGroup = bsrOpts;

  // the MSR score we're relying on
  fBaseMsrScore = originalBsrScore->getMsrScore ();

  // the BSR score we're visiting
  fVisitedBsrScore = originalBsrScore;
};

bsr2bsrFinalizer::~bsr2bsrFinalizer ()
{}

//________________________________________________________________________
void bsr2bsrFinalizer::translateBsrToFinalizedBsr ()
{
  if (fVisitedBsrScore) {
    // browse a bsrScore browser
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      fBaseMsrScore);
}

void bsr2bsrFinalizer::visitEnd (S_bsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrSpaces& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
/* JMI
  fCurrentLine->
    appendSpacesToLine (elt);
    */
}

void bsr2bsrFinalizer::visitEnd (S_bsrSpaces& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasure->
    appendBarLineToMeasure (elt);
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2bsrFinalizer::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI create the BSR score
  fBsrScore->
    appendTranscriptionNotesElementToScore (
      elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrPage& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPage = elt->createPageNewbornClone ();

  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void bsr2bsrFinalizer::visitEnd (S_bsrPage& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrPageHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentPageHeading = elt;

  fCurrentPage->
    appendPageHeadingToPage (fCurrentPageHeading);
}

void bsr2bsrFinalizer::visitEnd (S_bsrPageHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPageHeading = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentMusicHeading = elt;

  fCurrentPage->
    appendMusicHeadingToPage (fCurrentMusicHeading);
}

void bsr2bsrFinalizer::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMusicHeading = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrFootNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentFootNotes = elt;

  fCurrentPage->
    appendFootNotesToPage (fCurrentFootNotes);
}

void bsr2bsrFinalizer::visitEnd (S_bsrFootNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFootNotes = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
}

void bsr2bsrFinalizer::visitEnd (S_bsrLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrLineContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI ???
  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
    */
}

void bsr2bsrFinalizer::visitEnd (S_bsrLineContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasureNumber =
    elt->getInputLineNumber ();

  fCurrentMeasure =
    elt->createMeasureNewbornClone ();

  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void bsr2bsrFinalizer::visitEnd (S_bsrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrNumber& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasure->
    appendNumberToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrNumber& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasure->
    appendWordsToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasure->
    appendClefToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingKey (elt);
  }
  else {
    fCurrentLine->
      appendKeyToLine (elt);
  }
}

void bsr2bsrFinalizer::visitEnd (S_bsrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingTimeSignature (elt);
  }
  else {
    fCurrentLine->
      appendTimeSignatureToLine (elt);
  }
}

void bsr2bsrFinalizer::visitEnd (S_bsrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI ???
  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingTempo (elt);
  }
  else {
  */
    fCurrentLine->
      appendTempoToLine (elt);
 // }
}

void bsr2bsrFinalizer::visitEnd (S_bsrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasure->
    appendNoteToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       fCurrentMeasureNumber,
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
