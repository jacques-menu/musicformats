/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfStringsHandling.h"

#include "msrLyrics.h"
#include "msrTechnicals.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrTechnicalKindAsString (
  msrTechnicalKind technicalKind)
{
  std::string result;

  switch (technicalKind) {
    case msrTechnicalKind::kTechnicalArrow:
      result = "kTechnicalArrow";
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      result = "kTechnicalDoubleTongue";
      break;
    case msrTechnicalKind::kTechnicalDownBow:
      result = "kTechnicalDownBow";
      break;
    case msrTechnicalKind::kTechnicalFingernails:
      result = "kTechnicalFingernails";
      break;
    case msrTechnicalKind::kTechnicalHarmonic:
      result = "kTechnicalHarmonic";
      break;
    case msrTechnicalKind::kTechnicalHeel:
      result = "kTechnicalHeel";
      break;
    case msrTechnicalKind::kTechnicalHole:
      result = "kTechnicalHole";
      break;
    case msrTechnicalKind::kTechnicalOpenString:
      result = "kTechnicalOpenString";
      break;
    case msrTechnicalKind::kTechnicalSnapPizzicato:
      result = "kTechnicalSnapPizzicato";
      break;
    case msrTechnicalKind::kTechnicalStopped:
      result = "kTechnicalStopped";
      break;
    case msrTechnicalKind::kTechnicalTap:
      result = "kTechnicalTap";
      break;
    case msrTechnicalKind::kTechnicalThumbPosition:
      result = "kTechnicalThumbPosition";
      break;
    case msrTechnicalKind::kTechnicalToe:
      result = "kTechnicalToe";
      break;
    case msrTechnicalKind::kTechnicalTripleTongue:
      result = "kTechnicalTripleTongue";
      break;
    case msrTechnicalKind::kTechnicalUpBow:
      result = "kTechnicalUpBow";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTechnicalKind& elt)
{
  os << msrTechnicalKindAsString (elt);
  return os;
}

std::string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind)
{
  std::string result;

  switch (technicalTypeKind) {
    case msrTechnicalTypeKind::kTechnicalType_UNKNOWN_:
      result = "kTechnicalType_UNKNOWN_";
      break;
    case msrTechnicalTypeKind::kTechnicalTypeStart:
      result = "kTechnicalTypeStart";
      break;
    case msrTechnicalTypeKind::kTechnicalTypeStop:
      result = "kTechnicalTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTechnicalTypeKind& elt)
{
  os << msrTechnicalTypeKindAsString (elt);
  return os;
}

std::string msrTechnicalWithIntegerKindAsString (
  msrTechnicalWithIntegerKind technicalWithIntegerKind)
{
  std::string result;

  switch (technicalWithIntegerKind) {
    case msrTechnicalWithIntegerKind::kFingering:
      result = "kFingering";
      break;
    case msrTechnicalWithIntegerKind::kFret:
      result = "kFret";
      break;
    case msrTechnicalWithIntegerKind::kString:
      result = "kString";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTechnicalWithIntegerKind& elt)
{
  os << msrTechnicalWithIntegerKindAsString (elt);
  return os;
}

std::string msrTechnicalWithFloatKindAsString (
  msrTechnicalWithFloatKind technicalWithFloatKind)
{
  std::string result;

  switch (technicalWithFloatKind) {
    case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
      result = "kTechnicalWithFloatBend";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTechnicalWithFloatKind& elt)
{
  os << msrTechnicalWithFloatKindAsString (elt);
  return os;
}

std::string msrTechnicalWithStringKindAsString (
  msrTechnicalWithStringKind technicalWithStringKind)
{
  std::string result;

  switch (technicalWithStringKind) {
    case msrTechnicalWithStringKind::kHammerOn:
      result = "kHammerOn";
      break;
    case msrTechnicalWithStringKind::kHandbell:
      result = "kHandbell";
      break;
    case msrTechnicalWithStringKind::kOtherTechnical:
      result = "kOtherTechnical";
      break;
    case msrTechnicalWithStringKind::kPluck:
      result = "kPluck";
      break;
    case msrTechnicalWithStringKind::kPullOff:
      result = "kPullOff";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTechnicalWithStringKind& elt)
{
  os << msrTechnicalWithStringKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrTechnical msrTechnical::create (
  int              inputLineNumber,
  msrTechnicalKind technicalKind,
  msrPlacementKind technicalPlacementKind)
{
  msrTechnical* obj =
    new msrTechnical (
      inputLineNumber,
      technicalKind,
      technicalPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrTechnical::msrTechnical (
  int              inputLineNumber,
  msrTechnicalKind technicalKind,
  msrPlacementKind technicalPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalKind = technicalKind;

  fTechnicalPlacementKind = technicalPlacementKind;
}

msrTechnical::~msrTechnical ()
{}

void msrTechnical::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnical::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnical::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTechnical::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnical::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnical::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrTechnical::browseData (basevisitor* v)
{}

std::string msrTechnical::asString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalPlacementKind);
}

void msrTechnical::print (std::ostream& os) const
{
  os <<
    "Technical " <<
    asString () <<
    ", line " << fInputStartLineNumber <<
//    ", accidental mark" << ": " << technicalAccidentalMarkKindAsString () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTechnical& elt)
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
S_msrTechnicalWithInteger msrTechnicalWithInteger::create (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrPlacementKind            technicalWithIntegerPlacementKind)
{
  msrTechnicalWithInteger* obj =
    new msrTechnicalWithInteger (
      inputLineNumber,
      technicalWithIntegerKind,
      technicalWithIntegerValue,
      technicalWithIntegerPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrTechnicalWithInteger::msrTechnicalWithInteger (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrPlacementKind            technicalWithIntegerPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithIntegerKind = technicalWithIntegerKind;

  fTechnicalWithIntegerValue = technicalWithIntegerValue;

  fTechnicalWithIntegerPlacementKind = technicalWithIntegerPlacementKind;
}

msrTechnicalWithInteger::~msrTechnicalWithInteger ()
{}

void msrTechnicalWithInteger::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithInteger::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithInteger::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTechnicalWithInteger::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithInteger::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithInteger::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrTechnicalWithInteger::browseData (basevisitor* v)
{}

std::string msrTechnicalWithInteger::asString () const
{
  std::stringstream ss;

  ss <<
    "[TechnicalWithInteger"
    ", fTechnicalWithIntegerKind: " << fTechnicalWithIntegerKind <<
    ", fTechnicalWithIntegerValue: " <<  fTechnicalWithIntegerValue <<
    ", fTechnicalWithIntegerPlacementKind: " << fTechnicalWithIntegerPlacementKind <<
    ']';

  return ss.str ();
}

void msrTechnicalWithInteger::print (std::ostream& os) const
{
  os <<
    "[TechnicalWithInteger" <<
    ", fTechnicalWithIntegerKind: " << fTechnicalWithIntegerKind <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTechnicalWithIntegerValue" << ": " << fTechnicalWithIntegerValue <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTechnicalWithIntegerPlacementKind" << ": " << fTechnicalWithIntegerPlacementKind <<
    std::endl <<
    ']' <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithInteger& elt)
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
S_msrTechnicalWithFloat msrTechnicalWithFloat::create (
  int                       inputLineNumber,
  msrTechnicalWithFloatKind technicalWithFloatKind,
  float                     technicalWithFloatValue,
  msrPlacementKind          technicalWithFloatPlacementKind)
{
  msrTechnicalWithFloat* obj =
    new msrTechnicalWithFloat (
      inputLineNumber,
      technicalWithFloatKind,
      technicalWithFloatValue,
      technicalWithFloatPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrTechnicalWithFloat::msrTechnicalWithFloat (
  int                       inputLineNumber,
  msrTechnicalWithFloatKind technicalWithFloatKind,
  float                     technicalWithFloatValue,
  msrPlacementKind          technicalWithFloatPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithFloatKind = technicalWithFloatKind;

  fTechnicalWithFloatValue = technicalWithFloatValue;

  fTechnicalWithFloatPlacementKind = technicalWithFloatPlacementKind;
}

msrTechnicalWithFloat::~msrTechnicalWithFloat ()
{}

void msrTechnicalWithFloat::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithFloat::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithFloat>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithFloat>*> (v)) {
        S_msrTechnicalWithFloat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithFloat::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTechnicalWithFloat::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithFloat::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithFloat>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithFloat>*> (v)) {
        S_msrTechnicalWithFloat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithFloat::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrTechnicalWithFloat::browseData (basevisitor* v)
{}

std::string msrTechnicalWithFloat::asString () const
{
  std::stringstream ss;

  ss <<
    "TechnicalWithFloat" <<
    ", fTechnicalWithFloatKind: " << fTechnicalWithFloatKind <<
    ", fTechnicalWithFloatValue :'" << fTechnicalWithFloatValue << '\'' <<
    ", fTechnicalWithFloatPlacementKind: " << fTechnicalWithFloatPlacementKind <<
    ']';

  return ss.str ();
}

void msrTechnicalWithFloat::print (std::ostream& os) const
{
  os <<
    "[TechnicalWithFloat" <<
    ", fTechnicalWithFloatKind: " << fTechnicalWithFloatKind <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTechnicalWithFloatValue" << ": " << fTechnicalWithFloatValue <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTechnicalWithFloatPlacementKind" << ": " << fTechnicalWithFloatPlacementKind <<
    std::endl <<
    ']' <<
    std::endl <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithFloat& elt)
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
S_msrTechnicalWithString msrTechnicalWithString::create (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  const std::string&    technicalWithStringValue,
  msrPlacementKind           technicalWithStringPlacementKind)
{
  msrTechnicalWithString* obj =
    new msrTechnicalWithString (
      inputLineNumber,
      technicalWithStringKind,
      technicalWithStringTypeKind,
      technicalWithStringValue,
      technicalWithStringPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrTechnicalWithString::msrTechnicalWithString (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  const std::string&    technicalWithStringValue,
  msrPlacementKind           technicalWithStringPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithStringKind = technicalWithStringKind;

  fTechnicalWithStringTypeKind = technicalWithStringTypeKind;

  fTechnicalWithStringValue = technicalWithStringValue;

  fTechnicalWithStringPlacementKind = technicalWithStringPlacementKind;
}

msrTechnicalWithString::~msrTechnicalWithString ()
{}

void msrTechnicalWithString::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithString::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithString::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTechnicalWithString::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTechnicalWithString::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTechnicalWithString::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrTechnicalWithString::browseData (basevisitor* v)
{}

std::string msrTechnicalWithString::asString () const
{
  std::stringstream ss;

  ss <<
    "[TechnicalWithString" <<
    ", fTechnicalWithStringKind: " << fTechnicalWithStringKind <<
    ", fTechnicalWithStringTypeKind: " << fTechnicalWithStringTypeKind <<
    ", fTechnicalWithStringValue: \"" << fTechnicalWithStringValue << "\"" <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrTechnicalWithString::print (std::ostream& os) const
{
  os <<
    "[msrTechnicalWithString" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << std::left <<
    std::setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTechnicalWithStringValue" << ": \"" << fTechnicalWithStringValue << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind" << ": \"" << fTechnicalWithStringPlacementKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithString& elt)
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
