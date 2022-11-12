/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip> // for 'std::setw()'

#include "visitor.h"

#include "bsrTranscriptionNotes.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrTranscriptionNotesElement bsrTranscriptionNotesElement::create (
  int           inputLineNumber,
  const std::string& transcriptionNoteText)
{
  bsrTranscriptionNotesElement* o =
    new bsrTranscriptionNotesElement (
      inputLineNumber, transcriptionNoteText);
  assert (o != nullptr);
  return o;
}

bsrTranscriptionNotesElement::bsrTranscriptionNotesElement (
  int           inputLineNumber,
  const std::string& transcriptionNoteText)
    : bsrElement (inputLineNumber)
{
  fTranscriptionNoteText = transcriptionNoteText;
}

bsrTranscriptionNotesElement::~bsrTranscriptionNotesElement ()
{}

S_bsrCellsList bsrTranscriptionNotesElement::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  return result;
}

void bsrTranscriptionNotesElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTranscriptionNotesElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTranscriptionNotesElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotesElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTranscriptionNotesElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTranscriptionNotesElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTranscriptionNotesElement::browseData (basevisitor* v)
{}

std::string bsrTranscriptionNotesElement::asString () const
{
  std::stringstream s;

  s <<
    "TranscriptionNotesElement" <<
    // JMI ", fTranscriptionNoteText: " <<
    ", \"" << fTranscriptionNoteText << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrTranscriptionNotesElement::print (std::ostream& os) const
{
  os <<
    "TranscriptionNotesElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "transcriptionNoteText " << " : \"" << fTranscriptionNoteText << "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrTranscriptionNotesElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_bsrTranscriptionNotes bsrTranscriptionNotes::create (
  int inputLineNumber)
{
  bsrTranscriptionNotes* o =
    new bsrTranscriptionNotes (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

bsrTranscriptionNotes::bsrTranscriptionNotes (
  int inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gGlobalBrailleGenerationOahGroup->getCellsPerLine ())
{}

bsrTranscriptionNotes::~bsrTranscriptionNotes ()
{}

void bsrTranscriptionNotes::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTranscriptionNotes::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTranscriptionNotes::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotes::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTranscriptionNotes::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTranscriptionNotes::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTranscriptionNotes::browseData (basevisitor* v)
{
  for (
    std::list<S_bsrTranscriptionNotesElement>::const_iterator i =
      fTranscriptionNotesElementsList.begin ();
    i != fTranscriptionNotesElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrTranscriptionNotesElement> browser (v);
    browser.browse (*(*i));
  } // for
}

void bsrTranscriptionNotes::print (std::ostream& os) const
{
  os <<
    "TranscriptionNotes" <<
    std::endl;

  ++gIndenter;

  // print the notes if any
  const int fieldWidth = 19;

  size_t transcriptionNotesElementsListSize =
    fTranscriptionNotesElementsList.size ();

  if (transcriptionNotesElementsListSize || gGlobalBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "transcriptionNotesElementsList" << " : " <<
      std::endl;

    if (transcriptionNotesElementsListSize) {
      ++gIndenter;

      std::list<S_bsrTranscriptionNotesElement>::const_iterator
        iBegin = fTranscriptionNotesElementsList.begin (),
        iEnd   = fTranscriptionNotesElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      os << std::endl;

      --gIndenter;
    }
    else {
      os <<
        "[EMPTY]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrTranscriptionNotes& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}


}
