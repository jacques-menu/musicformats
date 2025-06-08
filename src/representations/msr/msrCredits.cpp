/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "msrCredits.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
std::string msrCreditTypeKindAsString (
  msrCreditTypeKind creditTypeKind)
{
  std::string result;

  switch (creditTypeKind) {
    case msrCreditTypeKind::kCreditType_UNKNOWN_:
      result = "kCreditType_UNKNOWN_";
      break;
    case msrCreditTypeKind::kCreditTypeNumber:
      result = "kCreditTypeNumber";
      break;
    case msrCreditTypeKind::kCreditTypeTitle:
      result = "kCreditTypeTitle";
      break;
    case msrCreditTypeKind::kCreditTypeSubtitle:
      result = "kCreditTypeSubtitle";
      break;
    case msrCreditTypeKind::kCreditTypeComposer:
      result = "kCreditTypeComposer";
      break;
    case msrCreditTypeKind::kCreditTypeArranger:
      result = "kCreditTypeArranger";
      break;
    case msrCreditTypeKind::kCreditTypeLyricist:
      result = "kCreditTypeLyricist";
      break;
    case msrCreditTypeKind::kCreditTypeRights:
      result = "kCreditTypeRights";
      break;
    case msrCreditTypeKind::kCreditTypePartName:
      result = "kCreditTypePartName";
      break;
    case msrCreditTypeKind::kCreditTypeOther:
      result = "kCreditTypeOther";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrCreditTypeKind& elt)
{
  os << msrCreditTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCreditWords msrCreditWords::create (
  const mfInputLineNumber& inputLineNumber,
  const std::string&    creditWordsContents,
  float                      creditWordsDefaultX,
  float                      creditWordsDefaultY,
  const std::string&    creditWordsFontFamily,
  float                      creditWordsFontSize,
  msrFontWeightKind          creditWordsFontWeightKind,
  msrFontStyleKind           creditWordsFontStyleKind,
  msrJustifyKind             creditWordsJustifyKind,
  msrHorizontalAlignmentKind creditWordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   creditWordsVerticalAlignmentKind,
  msrXMLLangKind             creditWordsXMLLang)
{
  msrCreditWords* obj =
    new msrCreditWords (
      inputLineNumber,
      creditWordsContents,
      creditWordsDefaultX,
      creditWordsDefaultY,
      creditWordsFontFamily,
      creditWordsFontSize,
      creditWordsFontWeightKind,
      creditWordsFontStyleKind,
      creditWordsJustifyKind,
      creditWordsHorizontalAlignmentKind,
      creditWordsVerticalAlignmentKind,
      creditWordsXMLLang);
  assert (obj != nullptr);
  return obj;
}

msrCreditWords::msrCreditWords (
  const mfInputLineNumber& inputLineNumber,
  const std::string&    creditWordsContents,
  float                      creditWordsDefaultX,
  float                      creditWordsDefaultY,
  const std::string&    creditWordsFontFamily,
  float                      creditWordsFontSize,
  msrFontWeightKind          creditWordsFontWeightKind,
  msrFontStyleKind           creditWordsFontStyleKind,
  msrJustifyKind             creditWordsJustifyKind,
  msrHorizontalAlignmentKind creditWordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   creditWordsVerticalAlignmentKind,
  msrXMLLangKind             creditWordsXMLLang)
    : msrElement (inputLineNumber)
{
  fCreditWordsContents = creditWordsContents;

  fCreditWordsDefaultX = creditWordsDefaultX;
  fCreditWordsDefaultY = creditWordsDefaultY;

  fCreditWordsFontFamily = creditWordsFontFamily;
  fCreditWordsFontSize = creditWordsFontSize;
  fCreditWordsFontWeightKind = creditWordsFontWeightKind;
  fCreditWordsFontStyleKind = creditWordsFontStyleKind;

  fCreditWordsJustifyKind = creditWordsJustifyKind;

  fCreditWordsHorizontalAlignmentKind = creditWordsHorizontalAlignmentKind;
  fCreditWordsVerticalAlignmentKind = creditWordsVerticalAlignmentKind;

  fCreditWordsXMLLang = creditWordsXMLLang;
}

msrCreditWords::~msrCreditWords ()
{}

void msrCreditWords::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCreditWords::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCreditWords::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCreditWords::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCreditWords::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrCreditWords::browseData (basevisitor* v)
{}

void msrCreditWords::print (std::ostream& os) const
{
  os <<
    "[CreditWords" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 35;

  os <<
    std::setw (fieldWidth) <<
    "fCreditWordsContents" << ": \"" <<
    fCreditWordsContents <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCreditWordsDefaultX" << ": " <<
    fCreditWordsDefaultX <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditWordsDefaultY" << ": " <<
    fCreditWordsDefaultY <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCreditWordsFontFamily" << ": \"" <<
    fCreditWordsFontFamily <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditWordsFontSize" << ": " <<
    fCreditWordsFontSize <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditWordsFontWeightKind" << ": " <<
    msrFontWeightKindAsString (
      fCreditWordsFontWeightKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditWordsFontStyleKind" << ": " <<
    msrFontStyleKindAsString (fCreditWordsFontStyleKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCreditWordsJustifyKind" << ": " <<
    msrJustifyKindAsString (fCreditWordsJustifyKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCreditWordsHorizontalAlignmentKind" << ": " <<
    msrHorizontalAlignmentKindAsString (
      fCreditWordsHorizontalAlignmentKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditWordsVerticalAlignmentKind" << ": " <<
    msrVerticalAlignmentKindAsString (
      fCreditWordsVerticalAlignmentKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCreditWordsXMLLang" << ": \"" <<
    msrXMLLangKindAsString (fCreditWordsXMLLang) <<
    "\"" <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrCreditWords& elt)
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
S_msrCredit msrCredit::create (
  const mfInputLineNumber& inputLineNumber,
  int creditPageNumber)
{
  msrCredit* obj =
    new msrCredit (
      inputLineNumber, creditPageNumber);
  assert (obj != nullptr);
  return obj;
}

msrCredit::msrCredit (
  const mfInputLineNumber& inputLineNumber,
  int creditPageNumber)
    : msrElement (inputLineNumber)
{
  fCreditPageNumber = creditPageNumber;

  fCreditTypeKind = msrCreditTypeKind::kCreditType_UNKNOWN_;  // default value
}

msrCredit::~msrCredit ()
{}

void msrCredit::appendCreditWordsToCredit (
  const S_msrCreditWords& creditWords)
{
  fCreditWordsList.push_back (creditWords);
}

void msrCredit::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCredit::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCredit::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCredit::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCredit::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrCredit::browseData (basevisitor* v)
{
  int n1 = fCreditWordsList.size ();

  for (int i = 0; i < n1; ++i ) {
    // browse the credit words
    msrBrowser<msrCreditWords> browser (v);
    browser.browse (*fCreditWordsList [i]);
  } // for
}

std::string msrCredit::asString () const
{
  std::stringstream ss;

  ss <<
    "Credit" <<
    ", fCreditPageNumber: " <<
    fCreditPageNumber <<
    ", fCreditTypeKind: " <<
    fCreditTypeKind;

  if (fCreditWordsList.size ()) {
    ss << " [";

    std::vector <S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << "\"" << (*i)->getCreditWordsContents () << "\"";
      if (++i == iEnd) break;
      ss << ", ";
    } // for

    ss << ']';
  }

  else
    ss << "no credit words";

  return ss.str ();
}

void msrCredit::print (std::ostream& os) const
{
  os <<
    "[Credit" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 33;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fCreditPageNumber" << ": " << fCreditPageNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCreditTypeKind" << ": " << fCreditTypeKind <<
    std::endl;

  if (fCreditWordsList.size ()) {
    std::vector <S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrCredit& elt)
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
