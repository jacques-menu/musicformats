/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfAssert.h"

#include "msrSpanners.h"

#include "oahOah.h"
#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind)
{
  std::string result;

  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerType_UNKNOWN:
      result = "kSpannerType_UNKNOWN";
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

std::ostream& operator << (std::ostream& os, const msrSpannerTypeKind& elt)
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
  const S_msrNote&   spannerUpLinkToNote)
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
  const S_msrNote&   spannerUpLinkToNote)
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
  const S_msrSpanner& sideLinkToOtherEnd)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    sideLinkToOtherEnd != nullptr,
    "sideLinkToOtherEnd is null");
#endif

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
      std::endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrSpanner::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSpanner::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrSpanner>*
    p =
      dynamic_cast<visitor<S_msrSpanner>*> (v)) {
        S_msrSpanner elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSpanner::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrSpanner::browseData (basevisitor* v)
{}

std::string msrSpannerKindAsString (
  msrSpannerKind spannerKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrSpannerKind& elt)
{
  os << msrSpannerKindAsString (elt);
  return os;
}

std::string msrSpanner::asShortString () const
{
  std::stringstream s;

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

void msrSpanner::print (std::ostream& os) const
{
  os <<
    "[Spanner" <<
    ", fSpannerNumber: " << fSpannerNumber <<
    ", fSpannerKind: " << fSpannerKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSpannerPlacementKind" << ": " << fSpannerPlacementKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpannerTypeKind" << ": " << fSpannerTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpannerUpLinkToNote" << ": " << fSpannerUpLinkToNote->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpannerBeginText" << ": \"" << fSpannerBeginText <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpannerMiddleText" << ": \"" << fSpannerMiddleText <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpannerEndText" << ": \"" << fSpannerEndText <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSpannerUpLinkToNote" << ": " << // JMI v0.9.66
    std::endl;
  ++gIndenter;
    os <<
      fSpannerUpLinkToNote->asString () <<
      std::endl;
  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSpannerSideLinkToOtherEnd";
  if (fSpannerSideLinkToOtherEnd) {
    os <<
      std::endl <<
      fSpannerSideLinkToOtherEnd->asShortString ();
  }
  else {
    os <<
    ": " << "[NONE]" <<
    std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSpanner& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
