/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // for 'setw()'

#include "visitor.h"

#include "bsrScores.h"

#include "bsrScores.h"

#include "oahOah.h"

#include "brailleGenerationOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrScore bsrScore::create (
  int            inputLineNumber,
  const S_msrScore&     theMsrScore)
{
  bsrScore* obj = new bsrScore (
    inputLineNumber, theMsrScore);
  assert (obj != nullptr);
  return obj;
}

bsrScore::bsrScore (
  int            inputLineNumber,
  const S_msrScore&     theMsrScore)
    : bsrElement (inputLineNumber)
{
  fMsrScore = theMsrScore;

  // create a transcription note
  fTranscriptionNotes =
    bsrTranscriptionNotes::create (
      inputLineNumber);

  fBrailleLineLength = gGlobalBrailleGenerationOahGroup->getCellsPerLine ();
  fBraillePageLength = gGlobalBrailleGenerationOahGroup->getLinesPerPage ();
}

bsrScore::~bsrScore ()
{}

void bsrScore::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrScore::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrScore::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrScore::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrScore::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrScore::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrScore::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrScore::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fTranscriptionNotes) {
    // browse the transcription notes
    msrBrowser<bsrTranscriptionNotes> browser (v);
    browser.browse (*fTranscriptionNotes);
  }

  for (
    std::list<S_bsrPage>::const_iterator i =
      fScorePagesList.begin ();
    i != fScorePagesList.end ();
    ++i
  ) {
    // browse the page
    msrBrowser<bsrPage> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% <== bsrScore::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsrScore::printFull (std::ostream& os) const
{
  os <<
    "BSR Score" <<
    std::endl << std::endl;

  ++gIndenter;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os << std::endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gBsrOahGroup->getDisplayBsrFull ()) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: [NONE]" <<
        std::endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    std::setw (fieldWidth) <<
    "BrailleLineLength" << ": " << fBrailleLineLength <<
    std::endl <<
    std::setw (fieldWidth) <<
    "BraillePageLength" << ": " << fBraillePageLength <<
    std::endl;
  os << std::endl;
        */

  // print the score pages if any
  size_t scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no std::endl here
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " <<
         "[NONE]" <<
      std::endl;
    }
  }

  --gIndenter;
}

void bsrScore::print (std::ostream& os) const
{
  os <<
    "BSR Score" <<
    std::endl << std::endl;

  ++gIndenter;

  // print the MSR structure (without the voices)
  os << fMsrScore;
  os << std::endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gBsrOahGroup->getDisplayBsrFull ()) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: [NONE]" <<
        std::endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    std::setw (fieldWidth) <<
    "BrailleLineLength" << ": " << fBrailleLineLength <<
    std::endl <<
    std::setw (fieldWidth) <<
    "BraillePageLength" << ": " << fBraillePageLength <<
    std::endl;
  os << std::endl;
        */

  // print the score pages if any
  size_t scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no std::endl here
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " <<
         "[EMPTY]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrScore& score)
{
  score->print (os);
  return os;
}


}
