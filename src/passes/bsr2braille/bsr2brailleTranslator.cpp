/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "bsr2brailleWae.h"

#include "mfServiceRunData.h"
#include "mfAssert.h"

#include "bsr2brailleTranslator.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"


namespace MusicFormats
{

//________________________________________________________________________
bsr2brailleTranslator::bsr2brailleTranslator (
  S_bsrScore     bsrScore,
  const S_bsrOahGroup& bsrOpts,
  std::ostream&       brailleOutputStream)
  : fBrailleOutputStream (
      brailleOutputStream)
{
  fBsrOahGroup = bsrOpts;

  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsr ()) {
    gLogStream <<
      "gGlobalBrailleGenerationOahGroup->getBrailleOutputKind (): " <<
      bsrBrailleOutputKindAsString (
        gGlobalBrailleGenerationOahGroup->getBrailleOutputKind ()) <<
      ", gGlobalBrailleGenerationOahGroup->getByteOrderingKind (): " <<
      bsrByteOrderingKindAsString (
        gGlobalBrailleGenerationOahGroup->getByteOrderingKind ()) <<
      std::endl;
  }
#endif

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
            std::stringstream s;

            s << "bsr2brailleTranslator: " ;

            std::string message = s.str ();

            bsr2brailleInternalError (
              gGlobalServiceRunData->getInputSourceName (),
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
    "fBrailleGenerator is null");
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrSpaces& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/* JMI
  gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPage& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrPage& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOP);
}

void bsr2brailleTranslator::visitStart (S_bsrPageElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrPageElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPagination& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPagination '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrPagination& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPagination '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPageHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrPageHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMusicHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForMusicHeading (elt);

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */

/*
  for (int i = 0; i < 6; ++i) {   // JMI ???
    if (true) {
      gLogStream <<
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "Generating an EOL after S_bsrMusicHeading" <<
      std::endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrFootNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */
}

void bsr2brailleTranslator::visitEnd (S_bsrFootNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrLine& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrLineContents& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForLineContents (elt);
}

void bsr2brailleTranslator::visitEnd (S_bsrLineContents& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "Generating an EOL after S_bsrLineContents" <<
      std::endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      bsrCellKind::kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrNumber& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrWords& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrClef& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrKey& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTimeSignature " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
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
void bsr2brailleTranslator::visitStart (S_bsrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrNote& elt)
{
#ifdef TRACING_IS_ENABLED
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
