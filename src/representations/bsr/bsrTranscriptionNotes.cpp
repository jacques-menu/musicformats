/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip> // for 'setw()'

#include "visitor.h"

#include "bsrTranscriptionNotes.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrTranscriptionNotesElement bsrTranscriptionNotesElement::create (
  int           inputLineNumber,
  const std::string& transcriptionNoteText)
{
  bsrTranscriptionNotesElement* obj =
    new bsrTranscriptionNotesElement (
      inputLineNumber, transcriptionNoteText);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTranscriptionNotesElement::acceptIn ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTranscriptionNotesElement::visitStart ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotesElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTranscriptionNotesElement::acceptOut ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTranscriptionNotesElement::visitEnd ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrTranscriptionNotesElement::browseData (basevisitor* v)
{}

std::string bsrTranscriptionNotesElement::asString () const
{
  std::stringstream ss;

  ss <<
    "TranscriptionNotesElement" <<
    // JMI ", fTranscriptionNoteText: " <<
    ", \"" << fTranscriptionNoteText << "\"" <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void bsrTranscriptionNotesElement::print (std::ostream& os) const
{
  os <<
    "TranscriptionNotesElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "transcriptionNoteText " << ": \"" << fTranscriptionNoteText << "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrTranscriptionNotesElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_bsrTranscriptionNotes bsrTranscriptionNotes::create (
  int inputLineNumber)
{
  bsrTranscriptionNotes* obj =
    new bsrTranscriptionNotes (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTranscriptionNotes::acceptIn ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTranscriptionNotes::visitStart ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotes::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTranscriptionNotes::acceptOut ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTranscriptionNotes::visitEnd ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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

  if (transcriptionNotesElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "transcriptionNotesElementsList" << ": " <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
