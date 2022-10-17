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

string msrSpanner::spannerKindAsString (
  msrSpannerKind spannerKind)
{
  string result;

  switch (spannerKind) {
    case msrSpanner::kSpannerDashes:
      result = "spannerDashes";
      break;
    case msrSpanner::kSpannerWavyLine:
      result = "spannerWavyLine";
      break;
  } // switch

  return result;
}

string msrSpanner::spannerTypeKindAsString () const
{
  return
    msrSpannerTypeKindAsString (
      fSpannerTypeKind);
}

string msrSpanner::spannerKindAsString () const
{
  return
    spannerKindAsString (
      fSpannerKind);
}

string msrSpanner::spannerPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSpannerPlacementKind);
}

string msrSpanner::asShortString () const
{
  stringstream s;

  s <<
    "Spanner" <<
    ", spannerNumber = " << fSpannerNumber <<
    ", " << spannerKindAsString () <<
    ", line " << fInputLineNumber <<
    ", spannerPlacementKind" << " : " <<
    spannerPlacementKindAsString () <<
    ", spannerTypeKind" << " : " <<
    spannerTypeKindAsString ();

  return s.str ();
}

void msrSpanner::print (ostream& os) const
{
  os <<
    "[Spanner" <<
    ", spannerNumber = " << fSpannerNumber <<
    ", " << spannerKindAsString () <<
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
    spannerTypeKindAsString () <<
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
    " : " << "***none***" <<
    endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrSpanner& elt)
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
