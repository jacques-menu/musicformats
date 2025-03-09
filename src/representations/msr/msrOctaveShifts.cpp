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

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasureConstants.h"

#include "msrOctaveShifts.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* obj =
    new msrOctaveShift (
      inputLineNumber,
      upLinkToMeasure,
      octaveShiftKind,
      octaveShiftSize);
  assert (obj != nullptr);
  return obj;
}

S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  return
    msrOctaveShift::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      octaveShiftKind,
      octaveShiftSize);
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrMeasureElement (
        inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift ()
{}

// void msrOctaveShift::setOctaveShiftUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceOctaveShifts ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of octave shift " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fOctaveShiftUpLinkToMeasure = measure;
// }

void msrOctaveShift::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOctaveShift::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOctaveShift::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOctaveShift::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOctaveShift::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

std::string msrOctaveShiftKindAsString (
  msrOctaveShiftKind octaveShiftKind)
{
  std::string result;

  switch (octaveShiftKind) {
    case msrOctaveShiftKind::kOctaveShiftNone:
      result = "kOctaveShiftNone";
      break;
    case msrOctaveShiftKind::kOctaveShiftUp:
      result = "kOctaveShiftUp";
      break;
    case msrOctaveShiftKind::kOctaveShiftDown:
      result = "kOctaveShiftDown";
      break;
    case msrOctaveShiftKind::kOctaveShiftStop:
      result = "kOctaveShiftStop";
      break;
    case msrOctaveShiftKind::kOctaveShiftContinue:
      result = "kOctaveShiftContinue";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrOctaveShiftKind& elt)
{
  os << msrOctaveShiftKindAsString (elt);
  return os;
}

std::string msrOctaveShift::asString () const
{
  std::stringstream ss;

  ss <<
    "[OctaveShift" <<
    ", fOctaveShiftKind: " << fOctaveShiftKind <<
    ", fOctaveShiftSize: " << fOctaveShiftSize <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrOctaveShift::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrOctaveShift& elt)
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
