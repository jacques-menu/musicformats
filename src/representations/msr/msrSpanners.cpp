/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"



#include "msrSpanners.h"

#include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind)
{
  string result;

  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerType_NO_:
      result = "kSpannerType_NO_";
      break;
    case msrSpannerTypeKind::kSpannerTypeStart:
      result = "kSpannerTypeStart";
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      result = "kSpannerTypeStop";
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      result = "kSpannerTypeContinue";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSpannerTypeKind& elt)
{
  os << msrSpannerTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSpanner msrSpanner::create (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerUpLinkToNote)
{
  msrSpanner* o =
    new msrSpanner (
      inputLineNumber,
      spannerNumber,
      spannerKind,
      spannerTypeKind,
      spannerPlacementKind,
      spannerUpLinkToNote);
  assert (o != nullptr);
  return o;
}

msrSpanner::msrSpanner (
  int                inputLineNumber,
  int                spannerNumber,
  msrSpannerKind     spannerKind,
  msrSpannerTypeKind spannerTypeKind,
  msrPlacementKind   spannerPlacementKind,
  S_msrNote          spannerUpLinkToNote)
    : msrElement (inputLineNumber)
{
  fSpannerUpLinkToNote = spannerUpLinkToNote;

  fSpannerNumber = spannerNumber;

  fSpannerKind = spannerKind;

  fSpannerTypeKind = spannerTypeKind;

  fSpannerPlacementKind = spannerPlacementKind;
}

msrSpanner::~msrSpanner ()
{}

void msrSpanner::setSpannerSideLinkToOtherEnd (
  S_msrSpanner sideLinkToOtherEnd)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    sideLinkToOtherEnd != nullptr,
    "sideLinkToOtherEnd is null");

  // set the two-way sideLink between both ends of the spanner
  fSpannerSideLinkToOtherEnd =
    sideLinkToOtherEnd;

  sideLinkToOtherEnd->fSpannerSideLinkToOtherEnd =
    this;
}

void msrSpanner::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSpanner::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSpanner::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSpanner::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSpanner::browseData (basevisitor* v)
{}

string msrSpannerKindAsString (
  msrSpannerKind spannerKind)
{
  string result;

  switch (spannerKind) {
    case msrSpannerKind::kSpannerDashes:
      result = "kSpannerDashes";
      break;
    case msrSpannerKind::kSpannerWavyLine:
      result = "kSpannerWavyLine";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSpannerKind& elt)
{
  os << msrSpannerKindAsString (elt);
  return os;
}

string msrSpanner::asShortString () const
{
  stringstream s;

  s <<
    "[Spanner" <<
    ", fSpannerNumber: " << fSpannerNumber <<
    ", fSpannerKind: " << fSpannerKind <<
    ", fSpannerPlacementKind: " << fSpannerPlacementKind <<
    ", fSpannerTypeKind: " << fSpannerTypeKind <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrSpanner::print (ostream& os) const
{
  os <<
    "[Spanner" <<
    ", fSpannerNumber = " << fSpannerNumber <<
    ", fSpannerKind: " << fSpannerKind <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "spannerPlacementKind" << " : " <<
    spannerPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerTypeKind" << " : " <<
    fSpannerTypeKind <<
    endl <<
    setw (fieldWidth) <<
    "spannerUpLinkToNote" << " : " <<
    fSpannerUpLinkToNote->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spannerBeginText" << " : \"" <<
    fSpannerBeginText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "spannerMiddleText" << " : \"" <<
    fSpannerMiddleText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "spannerEndText" << " : \"" <<
    fSpannerEndText <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "spannerUpLinkToNote" << " : " <<
    spannerPlacementKindAsString () <<
    endl;
  ++gIndenter;
    os <<
      fSpannerUpLinkToNote->asString () <<
      endl;
  --gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "spannerSideLinkToOtherEnd";
  if (fSpannerSideLinkToOtherEnd) {
    os <<
      endl <<
      fSpannerSideLinkToOtherEnd->asShortString ();
  }
  else {
    os <<
    " : " << "[NONE]" <<
    endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrSpanner& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
