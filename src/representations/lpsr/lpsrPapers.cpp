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

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "lpsrPapers.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPaper lpsrPaper::create (
  int                    inputLineNumber,
  const S_msrScaling&    scaling,
  const S_msrPageLayout& pageLayout)
{
  lpsrPaper* obj =
    new lpsrPaper (
      inputLineNumber,
      scaling,
      pageLayout);
  assert (obj != nullptr);
  return obj;
}

lpsrPaper::lpsrPaper (
  int                    inputLineNumber,
  const S_msrScaling&    scaling,
  const S_msrPageLayout& pageLayout)
    : lpsrElement (inputLineNumber)
{
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check

  // those always fail, without any consequences... Why??? JMI
//   mfAssert (
//     __FILE__, __LINE__,
//     scaling != nullptr,
//     "scaling is NULL");
//   mfAssert (
//     __FILE__, __LINE__,
//     pageLayout != nullptr,
//     "pageLayout is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

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
        fInputStartLineNumber,
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
  newbornClone->fPageTopSpacing =
    fPageTopSpacing;

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
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrPaper::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrPaper::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrPaper::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrPaper::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPaper>*
    p =
      dynamic_cast<visitor<S_lpsrPaper>*> (v)) {
        S_lpsrPaper elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrPaper::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrPaper::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrPaper::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrPaper::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsrPaper::printFull (std::ostream& os) const
{
  os <<
    "Paper" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

/*
  // scaling

  os << std::left <<
    std::setw (fieldWidth) <<
    "fScaling" << ": ";
  if (fScaling) {
    os << fScaling;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // page layout

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageLayout" << ": ";
  if (fPageLayout) {
    os << fPageLayout;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // indents

  os << std::left <<
    std::setw (fieldWidth) <<
    "fHorizontalShift" << ": ";
  if (fHorizontalShift) {
    os << fHorizontalShift;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fIndent" << ": ";
  if (fIndent) {
    os << fIndent;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fShortIndent" << ": ";
  if (fShortIndent) {
    os << fShortIndent;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // spaces

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMarkupSystemSpacingPadding" << ": ";
  if (fMarkupSystemSpacingPadding) {
    os << fMarkupSystemSpacingPadding;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBetweenSystemSpace" << ": ";
  if (fBetweenSystemSpace) {
    os << fBetweenSystemSpace;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageTopSpacing" << ": ";
  if (fPageTopSpacing) {
    os << fPageTopSpacing;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // counts

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageCount" << ": ";
  if (fPageCount) {
    os << fPageCount;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSystemCount" << ": ";
  if (fSystemCount) {
    os << fSystemCount;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // ragged output

//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fRaggedBottom" << ": " << fRaggedBottom <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fRaggedLast" << ": " << fRaggedLast <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fRaggedLastBottom" << ": " << fRaggedLastBottom <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fRaggedRight" << ": " << fRaggedRight <<
//     std::endl;

  // headers and footers

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOddHeaderMarkup" << ": \"" <<
    fOddHeaderMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEvenHeaderMarkup" << ": " <<
    fEvenHeaderMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOddFooterMarkup" << ": " <<
    fOddFooterMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEvenFooterMarkup" << ": " <<
    fEvenFooterMarkup <<
    "\"" <<
    std::endl;

  --gIndenter;
}

void lpsrPaper::print (std::ostream& os) const
{
  os <<
    "Paper" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  // scaling

  os << std::left <<
    std::setw (fieldWidth) <<
    "fScaling" << ": ";
  if (fScaling) {
    os << fScaling;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // page layout

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageLayout" << ": ";
  if (fPageLayout) {
    os << fPageLayout;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // indents

  os << std::left <<
    std::setw (fieldWidth) <<
    "fHorizontalShift" << ": ";
  if (fHorizontalShift) {
    os << fHorizontalShift;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fIndent" << ": ";
  if (fIndent) {
    os << fIndent;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fShortIndent" << ": ";
  if (fShortIndent) {
    os << fShortIndent;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // spaces

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMarkupSystemSpacingPadding" << ": ";
  if (fMarkupSystemSpacingPadding) {
    os << fMarkupSystemSpacingPadding;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBetweenSystemSpace" << ": ";
  if (fBetweenSystemSpace) {
    os << fBetweenSystemSpace;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageTopSpacing" << ": ";
  if (fPageTopSpacing) {
    os << fPageTopSpacing;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // counts

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageCount" << ": ";
  if (fPageCount > 0) {
    os << fPageCount;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSystemCount" << ": ";
  if (fSystemCount > 0) {
    os << fSystemCount;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // ragged output

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRaggedBottom" << ": " << fRaggedBottom <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRaggedLast" << ": " << fRaggedLast <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRaggedLastBottom" << ": " << fRaggedLastBottom <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRaggedRight" << ": " << fRaggedRight <<
    std::endl;

  // headers and footers

  os << std::left <<
    std::setw (fieldWidth) <<
    "oddHeaderMarkup" << ": \"" <<
    fOddHeaderMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "evenHeaderMarkup" << ": " <<
    fEvenHeaderMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "oddFooterMarkup" << ": " <<
    fOddFooterMarkup <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "evenFooterMarkup" << ": " <<
    fEvenFooterMarkup <<
    "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrPaper& pap) {
  pap->print (os);
  return os;
}


}
