/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrPageHeadings.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrPageHeading bsrPageHeading::create (
  int             inputLineNumber,
  const std::string&   pageHeadingTitle,
  const S_bsrPagination& pageHeadingPagination,
  int             pageHeadingNumber)
{
  bsrPageHeading* obj =
    new bsrPageHeading (
      inputLineNumber,
      pageHeadingTitle, pageHeadingPagination, pageHeadingNumber);
  assert (obj != nullptr);
  return obj;
}

bsrPageHeading::bsrPageHeading (
  int             inputLineNumber,
  const std::string&   pageHeadingTitle,
  const S_bsrPagination& pageHeadingPagination,
  int             pageHeadingNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gGlobalBrailleGenerationOahGroup->getCellsPerLine ())
{
  fPageHeadingTitle = pageHeadingTitle;

  fPageHeadingPagination = pageHeadingPagination;
  fPageHeadingNumber     = pageHeadingNumber;
}

bsrPageHeading::~bsrPageHeading ()
{}

S_bsrCellsList bsrPageHeading::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the pagination to result
  result->appendCellsListToCellsList (
    fPageHeadingPagination->fetchCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        fetchCellsList ());

  // append the title to result
  result->appendCellsListToCellsList (
    bsrWords::create (
      fInputLineNumber, fPageHeadingTitle)->
        fetchCellsList ());

  // append 3 spaces to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 3)->
        fetchCellsList ());

  // append the number to result
  result->appendCellsListToCellsList (
    bsrNumber::create (
      fInputLineNumber,
      fPageHeadingNumber,
      bsrNumberSignIsNeededKind::kNumberSignIsNeededYes)->
        fetchCellsList ());

  // append 1 space to result
  result->appendCellsListToCellsList (
    bsrSpaces::create (
      fInputLineNumber, 1)->
        fetchCellsList ());

  return result;
}

void bsrPageHeading::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPageHeading::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPageHeading::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrPageHeading::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrPageHeading::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrPageHeading::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrPageHeading::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrScore::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fPageHeadingPagination) {
    // browse the pagination
    msrBrowser<bsrPagination> browser (v);
    browser.browse (*fPageHeadingPagination);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% <== bsrScore::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string bsrPageHeading::asString () const
{
  std::stringstream ss;

  ss <<
    "PageHeading" <<
    ", pageHeadingTitle: \"" << fPageHeadingTitle << "\"" <<
      ", pageHeadingPagination: ";

  if (fPageHeadingPagination) {
    ss <<
      fPageHeadingPagination->asShortString ();
  }
  else {
    ss <<
      "[NULL]";
  }

  ss <<
    ", pageHeadingNumber: " << fPageHeadingNumber <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrPageHeading::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "[PH " << fPageHeadingTitle << "\"";

  if (fPageHeadingPagination) {
    ss <<
      fPageHeadingPagination->asDebugString ();
  }

  ss <<
    ", pageHeadingNumber: " << fPageHeadingNumber <<
    ']';

  return ss.str ();
}

void bsrPageHeading::print (std::ostream& os) const
{
  os <<
    "PageHeading" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "pageHeadingTitle" << ": \"" << fPageHeadingTitle << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "pageHeadingPagination" <<
    std::endl;

  if (fPageHeadingPagination) {
    ++gIndenter;

    os <<
      fPageHeadingPagination;

    --gIndenter;
  }
  else {
    os <<
      ": " << "[NULL]" <<
    std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "pageHeadingNumber" << ": " << fPageHeadingNumber <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrPageHeading& elt)
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
