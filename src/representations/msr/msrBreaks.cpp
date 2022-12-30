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

#include "visitor.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"

#include "msrBreaks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrUserChosenLineBreakKindAsString (
  msrUserChosenLineBreakKind userChosenLineBreakKind)
{
  std::string result;

  switch (userChosenLineBreakKind) {
    case msrUserChosenLineBreakKind::kUserChosenLineBreakYes:
      result = "kUserChosenLineBreakYes";
      break;
    case msrUserChosenLineBreakKind::kUserChosenLineBreakNo:
      result = "kUserChosenLineBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarNumber,
  msrUserChosenLineBreakKind
                      userChosenLineBreakKind)
{
  msrLineBreak* o =
    new msrLineBreak (
      inputLineNumber,
      upLinkToMeasure,
      nextBarNumber,
      userChosenLineBreakKind);
  assert (o != nullptr);
  return o;
}

S_msrLineBreak msrLineBreak::create (
  int                inputLineNumber,
  const std::string& nextBarNumber,
  msrUserChosenLineBreakKind
                     userChosenLineBreakKind)
{
  return
    msrLineBreak::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      nextBarNumber,
      userChosenLineBreakKind);
}

msrLineBreak::msrLineBreak (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarNumber,
  msrUserChosenLineBreakKind
                      userChosenLineBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fNextBarNumber = nextBarNumber;

  fUserChosenLineBreakKind = userChosenLineBreakKind;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Constructing a break before measure " << fNextBarNumber <<
      ", fUserChosenLineBreakKind: " <<
      msrUserChosenLineBreakKindAsString (
        fUserChosenLineBreakKind) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif
}

msrLineBreak::~msrLineBreak ()
{}

void msrLineBreak::setLineBreakUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of line break " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fLineBreakUpLinkToMeasure = measure;
}

void msrLineBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLineBreak::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLineBreak::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLineBreak::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLineBreak::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

std::string msrLineBreak::asString () const
{
  std::stringstream s;

  s <<
    "LineBreak" <<
    ", nextBarNumber = \"" << fNextBarNumber << "\"" <<
    ", fUserChosenLineBreakKind: " <<
    msrUserChosenLineBreakKindAsString (
      fUserChosenLineBreakKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrLineBreak::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrLineBreak& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
std::string msrUserChosenPageBreakKindAsString (
  msrUserChosenPageBreakKind userChosenPageBreakKind)
{
  std::string result;

  switch (userChosenPageBreakKind) {
    case msrUserChosenPageBreakKind::kUserChosenPageBreakYes:
      result = "kUserChosenPageBreakYes";
      break;
    case msrUserChosenPageBreakKind::kUserChosenPageBreakNo:
      result = "kUserChosenPageBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrUserChosenPageBreakKind
                      userChosenPageBreakKind)
{
  msrPageBreak* o =
    new msrPageBreak (
      inputLineNumber,
      upLinkToMeasure,
      userChosenPageBreakKind);
  assert (o != nullptr);
  return o;
}

S_msrPageBreak msrPageBreak::create (
  int                 inputLineNumber,
  msrUserChosenPageBreakKind
                      userChosenPageBreakKind)
{
  return
    msrPageBreak::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      userChosenPageBreakKind);
}

msrPageBreak::msrPageBreak (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrUserChosenPageBreakKind
                      userChosenPageBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Constructing a page break" <<
      ", fUserChosenPageBreakKind: " <<
      msrUserChosenPageBreakKindAsString (
        fUserChosenPageBreakKind) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fUserChosenPageBreakKind = userChosenPageBreakKind;
}

msrPageBreak::~msrPageBreak ()
{}

void msrPageBreak::setPageBreakUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of page break " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fPageBreakUpLinkToMeasure = measure;
}

void msrPageBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPageBreak::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPageBreak::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPageBreak::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPageBreak::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

std::string msrPageBreak::asString () const
{
  std::stringstream s;

  s <<
    "PageBreak" <<
    ", fUserChosenPageBreakKind: " <<
    msrUserChosenPageBreakKindAsString (
      fUserChosenPageBreakKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrPageBreak::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrPageBreak& elt)
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
