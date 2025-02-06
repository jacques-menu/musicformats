/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "bsr2brailleWae.h"

#include "mfServices.h"
#include "mfAssert.h"

#include "bsr2brailleTranslator.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
bsr2brailleTranslator::bsr2brailleTranslator (
  const S_bsrScore&    bsrScore,
  const S_bsrOahGroup& bsrOpts,
  std::ostream&        brailleOutputStream)
  : fBrailleOutputStream (
      brailleOutputStream)
{
  fBsrOahGroup = bsrOpts;

  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsr ()) {
    std::stringstream ss;

    ss <<
      "gGlobalBrailleGenerationOahGroup->getBrailleOutputKind (): " <<
      bsrBrailleOutputKindAsString (
        gGlobalBrailleGenerationOahGroup->getBrailleOutputKind ()) <<
      ", gGlobalBrailleGenerationOahGroup->getByteOrderingKind (): " <<
      bsrByteOrderingKindAsString (
        gGlobalBrailleGenerationOahGroup->getByteOrderingKind ());

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (gGlobalBrailleGenerationOahGroup->getBrailleOutputKind ()) {
    case bsrBrailleOutputKind::kBrailleOutputAscii:
      fBrailleGenerator =
        bsrAsciiBrailleGenerator::create (
          fBrailleOutputStream);
      break;

    case bsrBrailleOutputKind::kBrailleOutputUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case bsrBrailleOutputKind::kBrailleOutputUTF8Debug:
      fBrailleGenerator =
        bsrUTF8DebugBrailleGenerator::create (
          gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case bsrBrailleOutputKind::kBrailleOutputUTF16:
      switch (gGlobalBrailleGenerationOahGroup->getByteOrderingKind ()) {
        case bsrByteOrderingKind::kByteOrderingNone:
          {
            std::stringstream ss;

            ss << "bsr2brailleTranslator: " ;

            std::string message = ss.str ();

            bsr2brailleInternalError (
              gServiceRunData->getInputSourceName (),
              1, // JMI ???
              __FILE__, __LINE__,
              message);

            throw bsr2brailleException (message);
          }
          break;

        case bsrByteOrderingKind::kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;

        case bsrByteOrderingKind::kByteOrderingSmallEndian:
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;
      } // switch
      break;
  } // switch

  mfAssert (
    __FILE__, __LINE__,
    fBrailleGenerator != nullptr,
    "fBrailleGenerator is NULL");
}

bsr2brailleTranslator::~bsr2brailleTranslator ()
{}

//________________________________________________________________________
void bsr2brailleTranslator::translateBsrToBraille ()
{
  if (fVisitedBsrScore) {
    // browse the visited bsrScore
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrSpaces& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  gLog <<
    "% --> bsrSpaces" <<
    ", numberOfSpaces: " << elt->getNumberOfSpaces () <<
    ", spacesCellsList: " << elt->fetchCellsList () <<
    ", line " << elt->getInputLineNumber () <<
    std::endl;
*/

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrSpaces& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrBarLine" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPage& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrPage& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOP);
}

void bsr2brailleTranslator::visitStart (S_bsrPageElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPageElement '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrPageElement& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPageElement '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPagination& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPagination '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrPagination& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPagination '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPageHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrPageHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->
    generateCodeForMusicHeading (elt);

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */

/*
  for (int i = 0; i < 6; ++i) {   // JMI ???
    if (true) {
      gLog <<
        "Generating a space before S_bsrMusicHeading" <<
        std::endl;
    }

    fBrailleGenerator->
      generateCodeForBrailleCell (
        kCellSpace);
  //      kCellA);
  } // for
  */
}

void bsr2brailleTranslator::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (false) { // JMI
    std::stringstream ss;

    ss <<
      "Generating an EOL after S_bsrMusicHeading";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrFootNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */
}

void bsr2brailleTranslator::visitEnd (S_bsrFootNotes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrLineContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->
    generateCodeForLineContents (elt);
}

void bsr2brailleTranslator::visitEnd (S_bsrLineContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (false) { // JMI
    std::stringstream ss;

    ss <<
      "Generating an EOL after S_bsrLineContents";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMeasure& elt)
{
  fCurrentMeasureNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      '\'' <<
      ", line " << fCurrentMeasureNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsr2brailleTranslator::visitEnd (S_bsrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrMeasure '" <<
      elt->asString () <<
      '\'' <<
      ", line " << fCurrentMeasureNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrNumber& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrNumber& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrWords '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace

/* JMI
  switch (gGlobalBrailleGenerationOahGroup->getUTFKind ()) {
    case bsrUTFKind::kUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case bsrUTFKind::kUTF16:
      switch (gGlobalBrailleGenerationOahGroup->getByteOrderingKind ()) {
        case bsrByteOrderingKind::kByteOrderingNone:
          break;

        case bsrByteOrderingKind::kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;

        case bsrByteOrderingKind::kByteOrderingSmallEndian:
          break;
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBrailleGenerationOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
      } // switch
      break;
  } // switch
  */
