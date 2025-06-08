/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfStringsHandling.h"

#include "bsrPages.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrPage bsrPage::create (
  const mfInputLineNumber& inputLineNumber,
  int printPageNumber,
  int linesPerPage)
{
  bsrPage* obj =
    new bsrPage (
      inputLineNumber, printPageNumber, linesPerPage);
  assert (obj != nullptr);
  return obj;
}

bsrPage::bsrPage (
  const mfInputLineNumber& inputLineNumber,
  int printPageNumber,
  int linesPerPage)
    : bsrElement (inputLineNumber)
{
  fPrintPageNumber = printPageNumber;
  fBraillePageNumber = fPrintPageNumber; // this will be set by BSR finalizer

  fLinesPerPage = linesPerPage;

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTracePages ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrPage '" <<
      asString () <<
      "', line " <<
      fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

bsrPage::~bsrPage ()
{}

S_bsrPage bsrPage::createPageNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTracePages ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of page " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_bsrPage
    newbornClone =
      bsrPage::create (
        fInputLineNumber,
        fPrintPageNumber,
        fLinesPerPage);

  // braille line number
  newbornClone->fBraillePageNumber =
    fBraillePageNumber;

  return newbornClone;
}

int bsrPage::fetchLineContentsNumber () const
{
  int result = 0;

  for (
    std::list <S_bsrPageElement>::const_iterator i = fPageElementsList.begin ();
    i != fPageElementsList.end ();
    ++i
  ) {
    result += (*i)->fetchLineContentsNumber ();
  } // for

  return result;
}

void bsrPage::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPage::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPage::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrPage::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPage::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPage>*
    p =
      dynamic_cast<visitor<S_bsrPage>*> (v)) {
        S_bsrPage elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPage::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrPage::browseData (basevisitor* v)
{
  for (
    std::list <S_bsrPageElement>::const_iterator i = fPageElementsList.begin ();
    i != fPageElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string bsrPage::asString () const
{
  std::stringstream ss;

  ss <<
    "Page" <<
    ", printPageNumber" << ": " << fPrintPageNumber <<
    ", braillePageNumber" << ": " << fBraillePageNumber <<
    ", linesPerPage" << ": " << fLinesPerPage <<
    ", elements: " << fPageElementsList.size () <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void bsrPage::print (std::ostream& os) const
{
  os <<
    "Page" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", lineContentsNumber: " << fetchLineContentsNumber () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  // print the page numbers
  constexpr int fieldWidth = 18;

  os << std::left <<
  /* JMI
    std::setw (fieldWidth) <<
    "printPageNumber" << ": " << fPrintPageNumber <<
    std::endl <<
    */
    std::setw (fieldWidth) <<
    "braillePageNumber" << ": " << fBraillePageNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "linesPerPage" << ": " << fLinesPerPage <<
    std::endl;
  os << std::endl;

  // print the page elements if any
  size_t pageElementsListSize = fPageElementsList.size ();

  if (pageElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      std::setw (fieldWidth) <<
      "PageElementsList" <<
    ", " <<
    mfSingularOrPlural (
      pageElementsListSize, "pageElement", "pageElements");
    if (pageElementsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_bsrPageElement>::const_iterator
        iBegin = fPageElementsList.begin (),
        iEnd   = fPageElementsList.end (),
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
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrPage& elt)
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
