/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"

#include "msrBreaks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrUserSelectedLineBreakKindAsString (
  msrUserSelectedLineBreakKind userSelectedLineBreakKind)
{
  std::string result;

  switch (userSelectedLineBreakKind) {
    case msrUserSelectedLineBreakKind::kUserSelectedLineBreakYes:
      result = "kUserSelectedLineBreakYes";
      break;
    case msrUserSelectedLineBreakKind::kUserSelectedLineBreakNo:
      result = "kUserSelectedLineBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarNumber,
  msrUserSelectedLineBreakKind
                      userSelectedLineBreakKind)
{
  msrLineBreak* obj =
    new msrLineBreak (
      inputLineNumber,
      upLinkToMeasure,
      nextBarNumber,
      userSelectedLineBreakKind);
  assert (obj != nullptr);
  return obj;
}

S_msrLineBreak msrLineBreak::create (
  int                inputLineNumber,
  const std::string& nextBarNumber,
  msrUserSelectedLineBreakKind
                     userSelectedLineBreakKind)
{
  return
    msrLineBreak::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      nextBarNumber,
      userSelectedLineBreakKind);
}

msrLineBreak::msrLineBreak (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarNumber,
  msrUserSelectedLineBreakKind
                      userSelectedLineBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fNextBarNumber = nextBarNumber;

  fUserSelectedLineBreakKind = userSelectedLineBreakKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Constructing a break before measure " << fNextBarNumber <<
      ", fUserSelectedLineBreakKind: " <<
      msrUserSelectedLineBreakKindAsString (
        fUserSelectedLineBreakKind) <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrLineBreak::~msrLineBreak ()
{}

void msrLineBreak::setLineBreakUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of line break " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fLineBreakUpLinkToMeasure = measure;
}

void msrLineBreak::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineBreak::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineBreak::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineBreak::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineBreak::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

std::string msrLineBreak::asString () const
{
  std::stringstream ss;

  ss <<
    "LineBreak" <<
    ", nextBarNumber = \"" << fNextBarNumber << "\"" <<
    ", fUserSelectedLineBreakKind: " <<
    msrUserSelectedLineBreakKindAsString (
      fUserSelectedLineBreakKind) <<
    ", line " << fInputLineNumber;

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
std::string msrUserSelectedPageBreakKindAsString (
  msrUserSelectedPageBreakKind userSelectedPageBreakKind)
{
  std::string result;

  switch (userSelectedPageBreakKind) {
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakYes:
      result = "kUserSelectedPageBreakYes";
      break;
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakNo:
      result = "kUserSelectedPageBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
{
  msrPageBreak* obj =
    new msrPageBreak (
      inputLineNumber,
      upLinkToMeasure,
      userSelectedPageBreakKind);
  assert (obj != nullptr);
  return obj;
}

S_msrPageBreak msrPageBreak::create (
  int                 inputLineNumber,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
{
  return
    msrPageBreak::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      userSelectedPageBreakKind);
}

msrPageBreak::msrPageBreak (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Constructing a page break" <<
      ", fUserSelectedPageBreakKind: " <<
      msrUserSelectedPageBreakKindAsString (
        fUserSelectedPageBreakKind) <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fUserSelectedPageBreakKind = userSelectedPageBreakKind;
}

msrPageBreak::~msrPageBreak ()
{}

void msrPageBreak::setPageBreakUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of page break " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fPageBreakUpLinkToMeasure = measure;
}

void msrPageBreak::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageBreak::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageBreak::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageBreak::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageBreak::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

std::string msrPageBreak::asString () const
{
  std::stringstream ss;

  ss <<
    "PageBreak" <<
    ", fUserSelectedPageBreakKind: " <<
    msrUserSelectedPageBreakKindAsString (
      fUserSelectedPageBreakKind) <<
    ", line " << fInputLineNumber;

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
