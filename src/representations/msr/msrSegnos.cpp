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

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrSegnos.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrDalSegnoKindAsString (
  msrDalSegnoKind dalSegnoKind)
{
  std::string result;

  switch (dalSegnoKind) {
    case msrDalSegnoKind::kDalSegnoNone:
      result = "kDalSegnoNone";
      break;
    case msrDalSegnoKind::kDalSegno:
      result = "kDalSegno";
      break;
    case msrDalSegnoKind::kDalSegnoAlFine:
      result = "kDalSegnoAlFine";
      break;
    case msrDalSegnoKind::kDalSegnoAlCoda:
      result = "kDalSegnoAlCoda";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDalSegnoKind& elt)
{
  os << msrDalSegnoKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSegno msrSegno::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const mfStaffNumber& staffNumber)
{
  msrSegno* obj =
    new msrSegno (
      inputLineNumber,
      upLinkToMeasure,
      staffNumber);
  assert (obj != nullptr);
  return obj;
}

S_msrSegno msrSegno::create (
  const mfInputLineNumber& inputLineNumber,
  const mfStaffNumber& staffNumber)
{
  return
    msrSegno::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      staffNumber);
}

msrSegno::msrSegno (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const mfStaffNumber& staffNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fStaffNumber = staffNumber;
}

msrSegno::~msrSegno ()
{}

// void msrSegno::setSegnoUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegnos ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of segno " <<
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
//   fSegnoUpLinkToMeasure = measure;
// }

void msrSegno::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegno::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegno::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegno::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegno::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

std::string msrSegno::asString () const
{
  std::stringstream ss;

  ss <<
    "Segno" <<
    ", fDalSegnoString :" << fStaffNumber <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrSegno::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSegno& elt)
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
S_msrDalSegno msrDalSegno::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDalSegnoKind     dalSegnoKind,
  const std::string&  dalSegnoString,
  const mfStaffNumber& staffNumber)
{
  msrDalSegno* obj =
    new msrDalSegno (
      inputLineNumber,
      upLinkToMeasure,
      dalSegnoKind,
      dalSegnoString,
      staffNumber);
  assert (obj != nullptr);
  return obj;
}

S_msrDalSegno msrDalSegno::create (
  const mfInputLineNumber& inputLineNumber,
  msrDalSegnoKind     dalSegnoKind,
  const std::string&  dalSegnoString,
  const mfStaffNumber& staffNumber)
{
  return
    msrDalSegno::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      dalSegnoKind,
      dalSegnoString,
      staffNumber);
}

msrDalSegno::msrDalSegno (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDalSegnoKind     dalSegnoKind,
  const std::string&  dalSegnoString,
  const mfStaffNumber& staffNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fDalSegnoKind = dalSegnoKind;

  fDalSegnoString = dalSegnoString;

  fStaffNumber = staffNumber;
}

msrDalSegno::~msrDalSegno ()
{}

// void msrDalSegno::setDalSegnoUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceDalSegnos ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of dal segno " <<
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
//   fDalSegnoUpLinkToMeasure = measure;
// }

void msrDalSegno::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDalSegno::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDalSegno::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrDalSegno::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDalSegno::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDalSegno::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrDalSegno::browseData (basevisitor* v)
{}

std::string msrDalSegno::asString () const
{
  std::stringstream ss;

  ss <<
    "[DalSegno" <<
    ", fDalSegnoKind: " << fDalSegnoKind <<
    ", fDalSegnoString: \"" << fDalSegnoString << "\"" <<
    ", fDalSegnoString: " << fStaffNumber <<
    ", fMeasureElementPositionInMeasure: " << fMeasureElementPositionInMeasure <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrDalSegno::print (std::ostream& os) const
{
  os <<
    "[DalSegno" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fDalSegnoKind" << ": " << fDalSegnoKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fDalSegnoString" << ": \"" << fDalSegnoString << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fDalSegnoString" << ": " << fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << ": " << fMeasureElementPositionInMeasure <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "voicePosition" << ": " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "line" << ": " << fInputLineNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDalSegno& elt)
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
