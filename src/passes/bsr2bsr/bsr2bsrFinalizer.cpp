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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      K_MF_INPUT_LINE_UNKNOWN,
      fBaseMsrScore);
}

void bsr2bsrFinalizer::visitEnd (S_bsrScore& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrSpaces& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
/* JMI
  fCurrentLine->
    appendSpacesToLine (elt);
    */
}

void bsr2bsrFinalizer::visitEnd (S_bsrSpaces& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrBarLine& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure->
    appendBarLineToMeasure (elt);
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2bsrFinalizer::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // JMI create the BSR score
  fBsrScore->
    appendTranscriptionNotesElementToScore (
      elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrPage& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentPage = elt->createPageNewbornClone ();

  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void bsr2bsrFinalizer::visitEnd (S_bsrPage& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrPageHeading& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentPageHeading = elt;

  fCurrentPage->
    appendPageHeadingToPage (fCurrentPageHeading);
}

void bsr2bsrFinalizer::visitEnd (S_bsrPageHeading& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentPageHeading = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentMusicHeading = elt;

  fCurrentPage->
    appendMusicHeadingToPage (fCurrentMusicHeading);
}

void bsr2bsrFinalizer::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMusicHeading = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrFootNotes& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentFootNotes = elt;

  fCurrentPage->
    appendFootNotesToPage (fCurrentFootNotes);
}

void bsr2bsrFinalizer::visitEnd (S_bsrFootNotes& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFootNotes = nullptr;
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrLine& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
}

void bsr2bsrFinalizer::visitEnd (S_bsrLine& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrLineContents& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/* JMI ???
  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
    */
}

void bsr2bsrFinalizer::visitEnd (S_bsrLineContents& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrMeasure& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure =
    elt->createMeasureNewbornClone ();

  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void bsr2bsrFinalizer::visitEnd (S_bsrMeasure& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrNumber& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure->
    appendNumberToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrNumber& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrWords& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure->
    appendWordsToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrWords& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrClef& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure->
    appendClefToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrClef& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrKey& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTimeSignature& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrTempo& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2bsrFinalizer::visitStart (S_bsrNote& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMeasure->
    appendNoteToMeasure (elt);
}

void bsr2bsrFinalizer::visitEnd (S_bsrNote& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}


} // namespace
