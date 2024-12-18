/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrPaginations.h"

#include "bsrNumbers.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrPagination bsrPagination::create (
  int inputLineNumber,
  int printPageNumber,
  int braillePageNumber)
{
  bsrPagination* obj =
    new bsrPagination (
      inputLineNumber, printPageNumber, braillePageNumber);
  assert (obj != nullptr);
  return obj;
}

bsrPagination::bsrPagination (
  int inputLineNumber,
  int printPageNumber,
  int braillePageNumber)
    : bsrLineContentsElement (inputLineNumber)
{
  fPrintPageNumber   = printPageNumber;
  fBraillePageNumber = braillePageNumber;

  fPaginationCellsList = buildCellsList ();
}

bsrPagination::~bsrPagination ()
{}

S_bsrCellsList bsrPagination::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputStartLineNumber);

  // create the pagination sign
  S_bsrCellsList
    paginationSign =
      bsrCellsList::create (
        fInputStartLineNumber,
        bsrCellKind::kDots5, bsrCellKind::kDots25 );

  // append it to result
  result->appendCellsListToCellsList (
    paginationSign);

  // create the print page number
  S_bsrNumber
    printPageNumber =
      bsrNumber::create (
        fInputStartLineNumber,
        fPrintPageNumber,
        bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

  // append it to result
  result->appendCellsListToCellsList (
    printPageNumber->fetchCellsList ());

  // create the braille page number
  S_bsrNumber
    braillePageNumber =
      bsrNumber::create (
        fInputStartLineNumber,
        fPrintPageNumber,
        bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

  // append it to result
  result->appendCellsListToCellsList (
    braillePageNumber->fetchCellsList ());
  return result;
}

int bsrPagination::fetchCellsNumber () const
{
  return fPaginationCellsList->fetchCellsNumber ();
}

void bsrPagination::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPagination::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPagination>*
    p =
      dynamic_cast<visitor<S_bsrPagination>*> (v)) {
        S_bsrPagination elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPagination::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrPagination::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPagination::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPagination>*
    p =
      dynamic_cast<visitor<S_bsrPagination>*> (v)) {
        S_bsrPagination elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPagination::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrPagination::browseData (basevisitor* v)
{}

std::string bsrPagination::asString () const
{
  std::stringstream ss;

  ss <<
    "Pagination" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", braillePageNumber: " << fBraillePageNumber <<
    ", spacesCellsList: " << fPaginationCellsList <<
    ", line " << fInputStartLineNumber;

  return ss.str ();
}

std::string bsrPagination::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "PGNT" <<
    ", printPageNumber: " << fPrintPageNumber <<
    ", braillePageNumber: " << fBraillePageNumber;

  return ss.str ();
}

void bsrPagination::print (std::ostream& os) const
{
  os <<
    "Pagination" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

  os <<
    std::setw (fieldWidth) <<
    "printPageNumber" << ": " << fPrintPageNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "braillePageNumber" << ": " << fBraillePageNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "paginationCellsList" << ": " << fPaginationCellsList <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrPagination& elt)
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
