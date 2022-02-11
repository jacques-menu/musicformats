/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "bsrPageHeadings.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrPageHeading bsrPageHeading::create (
  int             inputLineNumber,
  const string&   pageHeadingTitle,
  S_bsrPagination pageHeadingPagination,
  int             pageHeadingNumber)
{
  bsrPageHeading* o =
    new bsrPageHeading (
      inputLineNumber,
      pageHeadingTitle, pageHeadingPagination, pageHeadingNumber);
  assert (o != nullptr);
  return o;
}

bsrPageHeading::bsrPageHeading (
  int             inputLineNumber,
  const string&   pageHeadingTitle,
  S_bsrPagination pageHeadingPagination,
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
      bsrNumber::kNumberSignIsNeededYes)->
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPageHeading::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPageHeading::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrPageHeading::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPageHeading::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrPageHeading>*
    p =
      dynamic_cast<visitor<S_bsrPageHeading>*> (v)) {
        S_bsrPageHeading elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPageHeading::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrPageHeading::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fPageHeadingPagination) {
    // browse the pagination
    msrBrowser<bsrPagination> browser (v);
    browser.browse (*fPageHeadingPagination);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

string bsrPageHeading::asString () const
{
  stringstream s;

  s <<
    "PageHeading" <<
    ", pageHeadingTitle: \"" << fPageHeadingTitle << "\"" <<
      ", pageHeadingPagination: ";

  if (fPageHeadingPagination) {
    s <<
      fPageHeadingPagination->asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
    ", pageHeadingNumber: " << fPageHeadingNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrPageHeading::asDebugString () const
{
  stringstream s;

  s <<
    "[PH " << fPageHeadingTitle << "\"";

  if (fPageHeadingPagination) {
    s <<
      fPageHeadingPagination->asDebugString ();
  }

  s <<
    ", pageHeadingNumber: " << fPageHeadingNumber <<
    "]";

  return s.str ();
}

void bsrPageHeading::print (ostream& os) const
{
  os <<
    "PageHeading" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "pageHeadingTitle" << " : \"" << fPageHeadingTitle << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "pageHeadingPagination" <<
    endl;

  if (fPageHeadingPagination) {
    ++gIndenter;

    os <<
      fPageHeadingPagination;

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
    endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "pageHeadingNumber" << " : " << fPageHeadingNumber <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrPageHeading& elt)
{
  elt->print (os);
  return os;
}


}
