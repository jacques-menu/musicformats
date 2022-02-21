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

#include "lpsrPapers.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int             inputLineNumber,
  S_msrScaling    scaling,
  S_msrPageLayout pageLayout)
{
  lpsrPaper* o =
    new lpsrPaper (
      inputLineNumber,
      scaling,
      pageLayout);
  assert (o != nullptr);
  return o;
}

lpsrPaper::lpsrPaper (
  int             inputLineNumber,
  S_msrScaling    scaling,
  S_msrPageLayout pageLayout)
    : lpsrElement (inputLineNumber)
{
  // sanity checks

  // those always fail, without any consequences... Why??? JMI
//   mfAssert (
//     __FILE__, __LINE__,
//     scaling != nullptr,
//     "scaling is null");
//   mfAssert (
//     __FILE__, __LINE__,
//     pageLayout != nullptr,
//     "pageLayout is null");

  fScaling = scaling;
  fPageLayout = pageLayout;

  fPageCount = -1;
  fSystemCount = -1;
}

S_lpsrPaper lpsrPaper::createPaperNewbornClone ()
{
  S_lpsrPaper
    newbornClone =
      lpsrPaper::create (
        fInputLineNumber,
        fScaling,
        fPageLayout);

  // indents
  newbornClone->fHorizontalShift =
    fHorizontalShift;
  newbornClone->fIndent =
    fIndent;
  newbornClone->fShortIndent =
    fShortIndent;

  // spaces
  newbornClone->fMarkupSystemSpacingPadding =
    fMarkupSystemSpacingPadding;
  newbornClone->fBetweenSystemSpace =
    fBetweenSystemSpace;
  newbornClone->fPageTopSpace =
    fPageTopSpace;

  // counts
  newbornClone->fPageCount =
    fPageCount;
  newbornClone->fSystemCount =
    fSystemCount;

  // headers and footers
  newbornClone->fOddHeaderMarkup =
    fOddHeaderMarkup;
  newbornClone->fEvenHeaderMarkup =
    fEvenHeaderMarkup;
  newbornClone->fOddFooterMarkup =
    fOddFooterMarkup;
  newbornClone->fEvenFooterMarkup =
    fEvenFooterMarkup;

  return newbornClone;
}

lpsrPaper::~lpsrPaper ()
{}

void lpsrPaper::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPaper::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPaper::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPaper::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPaper::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPaper::browseData ()" <<
      endl;
  }
#endif

  // browse the scaling
  if (fScaling) {
    msrBrowser<msrScaling> browser (v);
    browser.browse (*fScaling);
  }

  // browse the page layout
  if (fPageLayout) {
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrPaper::browseData ()" <<
      endl;
  }
#endif
}

void lpsrPaper::print (ostream& os) const
{
  os <<
    "Paper" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

/*
  // scaling

  os << left <<
    setw (fieldWidth) <<
    "fScaling" << " : ";
  if (fScaling) {
    os << fScaling;
  }
  else {
    os << "none";
  }
  os << endl;

  // page layout

  os << left <<
    setw (fieldWidth) <<
    "fPageLayout" << " : ";
  if (fPageLayout) {
    os << fPageLayout;
  }
  else {
    os << "none";
  }
  os << endl;

  // indents

  os << left <<
    setw (fieldWidth) <<
    "fHorizontalShift" << " : ";
  if (fHorizontalShift) {
    os << fHorizontalShift;
  }
  else {
    os << "none";
  }
  os << endl;
*/

  os << left <<
    setw (fieldWidth) <<
    "fIndent" << " : ";
  if (fIndent) {
    os << fIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fShortIndent" << " : ";
  if (fShortIndent) {
    os << fShortIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  // spaces

  os << left <<
    setw (fieldWidth) <<
    "fMarkupSystemSpacingPadding" << " : ";
  if (fMarkupSystemSpacingPadding) {
    os << fMarkupSystemSpacingPadding;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fBetweenSystemSpace" << " : ";
  if (fBetweenSystemSpace) {
    os << fBetweenSystemSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fPageTopSpace" << " : ";
  if (fPageTopSpace) {
    os << fPageTopSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  // counts

  os << left <<
    setw (fieldWidth) <<
    "fPageCount" << " : ";
  if (fPageCount) {
    os << fPageCount;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fSystemCount" << " : ";
  if (fSystemCount) {
    os << fSystemCount;
  }
  else {
    os << "none";
  }
  os << endl;

  // booleans

  os << left <<
    setw (fieldWidth) <<
    "fRaggedBottom" << " : " << fRaggedBottom <<
    endl <<
    setw (fieldWidth) <<
    "fRaggedLastBottom" << " : " << fRaggedLastBottom <<
    endl;

  // headers and footers

  os << left <<
    setw (fieldWidth) <<
    "fOddHeaderMarkup" << " : \"" <<
    fOddHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fEvenHeaderMarkup" << " : " <<
    fEvenHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fOddFooterMarkup" << " : " <<
    fOddFooterMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fEvenFooterMarkup" << " : " <<
    fEvenFooterMarkup <<
    "\"" <<
    endl;

  --gIndenter;
}

void lpsrPaper::printShort (ostream& os) const
{
  os <<
    "Paper" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

  // scaling

  os << left <<
    setw (fieldWidth) <<
    "fScaling" << " : ";
  if (fScaling) {
    os << fScaling;
  }
  else {
    os << "none";
  }
  os << endl;

  // page layout

  os << left <<
    setw (fieldWidth) <<
    "fPageLayout" << " : ";
  if (fPageLayout) {
    os << fPageLayout;
  }
  else {
    os << "none";
  }
  os << endl;

  // indents

  os << left <<
    setw (fieldWidth) <<
    "fHorizontalShift" << " : ";
  if (fHorizontalShift) {
    os << fHorizontalShift;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fIndent" << " : ";
  if (fIndent) {
    os << fIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fShortIndent" << " : ";
  if (fShortIndent) {
    os << fShortIndent;
  }
  else {
    os << "none";
  }
  os << endl;

  // spaces

  os << left <<
    setw (fieldWidth) <<
    "fMarkupSystemSpacingPadding" << " : ";
  if (fMarkupSystemSpacingPadding) {
    os << fMarkupSystemSpacingPadding;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fBetweenSystemSpace" << " : ";
  if (fBetweenSystemSpace) {
    os << fBetweenSystemSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fPageTopSpace" << " : ";
  if (fPageTopSpace) {
    os << fPageTopSpace;
  }
  else {
    os << "none";
  }
  os << endl;

  // counts

  os << left <<
    setw (fieldWidth) <<
    "fPageCount" << " : ";
  if (fPageCount) {
    os << fPageCount;
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fSystemCount" << " : ";
  if (fSystemCount) {
    os << fSystemCount;
  }
  else {
    os << "none";
  }
  os << endl;

  // booleans

  os << left <<
    setw (fieldWidth) <<
    "fRaggedBottom" << " : " << fRaggedBottom <<
    endl <<
    setw (fieldWidth) <<
    "fRaggedLastBottom" << " : " << fRaggedLastBottom <<
    endl;

  // headers and footers

  os << left <<
    setw (fieldWidth) <<
    "oddHeaderMarkup" << " : \"" <<
    fOddHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "evenHeaderMarkup" << " : " <<
    fEvenHeaderMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "oddFooterMarkup" << " : " <<
    fOddFooterMarkup <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "evenFooterMarkup" << " : " <<
    fEvenFooterMarkup <<
    "\"" <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}


}
