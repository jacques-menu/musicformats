/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip> // for 'setw()'

#include "visitor.h"

#include "bsrFootNotes.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrFootNotes bsrFootNotes::create (
  const mfInputLineNumber& inputLineNumber)
{
  bsrFootNotes* obj =
    new bsrFootNotes (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

bsrFootNotes::bsrFootNotes (
  const mfInputLineNumber& inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gGlobalBrailleGenerationOahGroup->getCellsPerLine ())
{}

bsrFootNotes::~bsrFootNotes ()
{}

void bsrFootNotes::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrFootNotes::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrFootNotes>*
    p =
      dynamic_cast<visitor<S_bsrFootNotes>*> (v)) {
        S_bsrFootNotes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrFootNotes::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrFootNotes::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrFootNotes::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrFootNotes>*
    p =
      dynamic_cast<visitor<S_bsrFootNotes>*> (v)) {
        S_bsrFootNotes elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrFootNotes::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrFootNotes::browseData (basevisitor* v)
{
  for (
    std::list <S_bsrFootNotesElement>::const_iterator i =
      fFootNotesElementsList.begin ();
    i != fFootNotesElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrFootNotesElement> browser (v);
    browser.browse (*(*i));
  } // for
}

void bsrFootNotes::print (std::ostream& os) const
{
  os <<
    "FootNotes" <<
    std::endl;

  ++gIndenter;

  // print the notes if any
  constexpr int fieldWidth = 19;

  size_t footNotesElementsListSize = fFootNotesElementsList.size ();

  if (footNotesElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "footNotesElementsList" << ": " <<
      std::endl;

    if (footNotesElementsListSize) {
      ++gIndenter;

      std::list <S_bsrFootNotesElement>::const_iterator
        iBegin = fFootNotesElementsList.begin (),
        iEnd   = fFootNotesElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

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

std::ostream& operator << (std::ostream& os, const S_bsrFootNotes& elt)
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
