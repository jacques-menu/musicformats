/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrScore bsrScore::create (
  int            inputLineNumber,
  S_msrScore     theMsrScore)
{
  bsrScore* o = new bsrScore (
    inputLineNumber, theMsrScore);
  assert (o != nullptr);
  return o;
}

bsrScore::bsrScore (
  int            inputLineNumber,
  S_msrScore     theMsrScore)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrScore::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrScore::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrScore::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrScore::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrScore::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrScore::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fTranscriptionNotes) {
    // browse the transcription notes
    msrBrowser<bsrTranscriptionNotes> browser (v);
    browser.browse (*fTranscriptionNotes);
  }

  for (
    list<S_bsrPage>::const_iterator i =
      fScorePagesList.begin ();
    i != fScorePagesList.end ();
    ++i
  ) {
    // browse the page
    msrBrowser<bsrPage> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

void bsrScore::print (ostream& os) const
{
  os <<
    "BSR Score" <<
    endl << endl;

  ++gIndenter;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os << endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: none" <<
        endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    setw (fieldWidth) <<
    "BrailleLineLength" << " : " << fBrailleLineLength <<
    endl <<
    setw (fieldWidth) <<
    "BraillePageLength" << " : " << fBraillePageLength <<
    endl;
  os << endl;
        */

  // print the score pages if any
  size_t scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << endl;
      ++gIndenter;

      list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " <<
         "none" <<
      endl;
    }
  }

  --gIndenter;
}

void bsrScore::printShort (ostream& os) const
{
  os <<
    "BSR Score" <<
    endl << endl;

  ++gIndenter;

  // print the MSR structure (without the voices)
  fMsrScore->
    printShort (os);
  os << endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: none" <<
        endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    setw (fieldWidth) <<
    "BrailleLineLength" << " : " << fBrailleLineLength <<
    endl <<
    setw (fieldWidth) <<
    "BraillePageLength" << " : " << fBraillePageLength <<
    endl;
  os << endl;
        */

  // print the score pages if any
  size_t scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << endl;
      ++gIndenter;

      list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        (*i)->printShort (os);
        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " <<
         "empty" <<
      endl;
    }
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrScore& score)
{
  score->print (os);
  return os;
}


}
