/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "msrCredits.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
string msrCreditTypeKindAsString (
  msrCreditTypeKind creditTypeKind)
{
  string result;

  switch (creditTypeKind) {
    case msrCreditTypeKind::k_NoCreditType:
      result = "k_NoCreditType";
      break;
    case msrCreditTypeKind::kPageNumber:
      result = "kPageNumber";
      break;
    case msrCreditTypeKind::kTitle:
      result = "kTitle";
      break;
    case msrCreditTypeKind::kSubtitle:
      result = "kSubtitle";
      break;
    case msrCreditTypeKind::kComposer:
      result = "kComposer";
      break;
    case msrCreditTypeKind::kArranger:
      result = "kArranger";
      break;
    case msrCreditTypeKind::kLyricist:
      result = "kLyricist";
      break;
    case msrCreditTypeKind::kRights:
      result = "kRights";
      break;
    case msrCreditTypeKind::kPartName:
      result = "kPartName";
      break;
    case msrCreditTypeKind::kOtherCreditType:
      result = "kOtherCreditType";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, const msrCreditTypeKind& elt)
{
  os << msrCreditTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCreditWords msrCreditWords::create (
  int                        inputLineNumber,
  const string&              creditWordsContents,
  float                      creditWordsDefaultX,
  float                      creditWordsDefaultY,
  const string&              creditWordsFontFamily,
  float                      creditWordsFontSize,
  msrFontWeightKind          creditWordsFontWeightKind,
  msrFontStyleKind           creditWordsFontStyleKind,
  msrJustifyKind             creditWordsJustifyKind,
  msrHorizontalAlignmentKind creditWordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   creditWordsVerticalAlignmentKind,
  msrXMLLangKind             creditWordsXMLLang)
{
  msrCreditWords* o =
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
  assert (o != nullptr);
  return o;
}

msrCreditWords::msrCreditWords (
  int                        inputLineNumber,
  const string&              creditWordsContents,
  float                      creditWordsDefaultX,
  float                      creditWordsDefaultY,
  const string&              creditWordsFontFamily,
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

  fCreditWordsFontFamily     = creditWordsFontFamily;
  fCreditWordsFontSize       = creditWordsFontSize;
  fCreditWordsFontWeightKind = creditWordsFontWeightKind;
  fCreditWordsFontStyleKind  = creditWordsFontStyleKind;

  fCreditWordsJustifyKind = creditWordsJustifyKind;

  fCreditWordsHorizontalAlignmentKind = creditWordsHorizontalAlignmentKind;
  fCreditWordsVerticalAlignmentKind   = creditWordsVerticalAlignmentKind;

  fCreditWordsXMLLang = creditWordsXMLLang;
}

msrCreditWords::~msrCreditWords ()
{}

void msrCreditWords::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCreditWords::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCreditWords::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCreditWords::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCreditWords::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCreditWords::browseData (basevisitor* v)
{}

void msrCreditWords::print (ostream& os) const
{
  os <<
    "[CreditWords" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 35;

  os <<
    setw (fieldWidth) <<
    "fCreditWordsContents" << " : \"" <<
    fCreditWordsContents <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fCreditWordsDefaultX" << " : " <<
    fCreditWordsDefaultX <<
    endl <<
    setw (fieldWidth) <<
    "fCreditWordsDefaultY" << " : " <<
    fCreditWordsDefaultY <<
    endl <<

    setw (fieldWidth) <<
    "fCreditWordsFontFamily" << " : \"" <<
    fCreditWordsFontFamily <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fCreditWordsFontSize" << " : " <<
    fCreditWordsFontSize <<
    endl <<
    setw (fieldWidth) <<
    "fCreditWordsFontWeightKind" << " : " <<
    msrFontWeightKindAsString (
      fCreditWordsFontWeightKind) <<
    endl <<
    setw (fieldWidth) <<
    "fCreditWordsFontStyleKind" << " : " <<
    msrFontStyleKindAsString (fCreditWordsFontStyleKind) <<
    endl <<

    setw (fieldWidth) <<
    "fCreditWordsJustifyKind" << " : " <<
    msrJustifyKindAsString (fCreditWordsJustifyKind) <<
    endl <<

    setw (fieldWidth) <<
    "fCreditWordsHorizontalAlignmentKind" << " : " <<
    msrHorizontalAlignmentKindAsString (
      fCreditWordsHorizontalAlignmentKind) <<
    endl <<
    setw (fieldWidth) <<
    "fCreditWordsVerticalAlignmentKind" << " : " <<
    msrVerticalAlignmentKindAsString (
      fCreditWordsVerticalAlignmentKind) <<
    endl <<

    setw (fieldWidth) <<
    "fCreditWordsXMLLang" << " : \"" <<
    msrXMLLangKindAsString (fCreditWordsXMLLang) <<
    "\"" <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrCreditWords& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrCredit msrCredit::create (
  int inputLineNumber,
  int creditPageNumber)
{
  msrCredit* o =
    new msrCredit (
      inputLineNumber, creditPageNumber);
  assert (o != nullptr);
  return o;
}

msrCredit::msrCredit (
  int inputLineNumber,
  int creditPageNumber)
    : msrElement (inputLineNumber)
{
  fCreditPageNumber = creditPageNumber;

  fCreditTypeKind = msrCreditTypeKind::k_NoCreditType;  // default value
}

msrCredit::~msrCredit ()
{}

void msrCredit::appendCreditWordsToCredit (
  S_msrCreditWords creditWords)
{
  fCreditWordsList.push_back (creditWords);
}

void msrCredit::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCredit::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCredit::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCredit::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCredit::visitEnd ()" <<
            endl;
        }
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

string msrCredit::asString () const
{
  stringstream s;

  s <<
    "Credit" <<
    ", fCreditPageNumber: " <<
    fCreditPageNumber <<
    ", fCreditTypeKind: " <<
    fCreditTypeKind;

  if (fCreditWordsList.size ()) {
    s << " [";

    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << "\"" << (*i)->getCreditWordsContents () << "\"";
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }

  else
    s << "no credit words";

  return s.str ();
}

void msrCredit::print (ostream& os) const
{
  os <<
    "[Credit" <<
    endl;

  ++gIndenter;

	const int fieldWidth = 33;

  os << left <<
  	setw (fieldWidth) <<
    "fCreditPageNumber" << " : " << fCreditPageNumber <<
    endl <<
  	setw (fieldWidth) <<
    "fCreditTypeKind" << " : " << fCreditTypeKind <<
    endl;

  if (fCreditWordsList.size ()) {
    vector<S_msrCreditWords>::const_iterator
      iBegin = fCreditWordsList.begin (),
      iEnd   = fCreditWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrCredit& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}


}
