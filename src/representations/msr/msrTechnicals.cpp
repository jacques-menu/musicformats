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

#include "mfStringsHandling.h"

#include "msrLyrics.h"
#include "msrTechnicals.h"

#include "oahOah.h"
#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string msrTechnicalKindAsString (
  msrTechnicalKind technicalKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrTechnicalKind& elt)
{
  os << msrTechnicalKindAsString (elt);
  return os;
}

string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind)
{
  string result;

  switch (technicalTypeKind) {
    case msrTechnicalTypeKind::kTechnicalType_NO_:
      result = "kTechnicalType_NO_";
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

ostream& operator << (ostream& os, const msrTechnicalTypeKind& elt)
{
  os << msrTechnicalTypeKindAsString (elt);
  return os;
}

string msrTechnicalWithIntegerKindAsString (
  msrTechnicalWithIntegerKind technicalWithIntegerKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrTechnicalWithIntegerKind& elt)
{
  os << msrTechnicalWithIntegerKindAsString (elt);
  return os;
}

string msrTechnicalWithFloatKindAsString (
  msrTechnicalWithFloatKind technicalWithFloatKind)
{
  string result;

  switch (technicalWithFloatKind) {
    case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
      result = "kTechnicalWithFloatBend";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTechnicalWithFloatKind& elt)
{
  os << msrTechnicalWithFloatKindAsString (elt);
  return os;
}

string msrTechnicalWithStringKindAsString (
  msrTechnicalWithStringKind technicalWithStringKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrTechnicalWithStringKind& elt)
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
  msrTechnical* o =
    new msrTechnical (
      inputLineNumber,
      technicalKind,
      technicalPlacementKind);
  assert (o != nullptr);
  return o;
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnical::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnical::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnical::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnical::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnical::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnical::browseData (basevisitor* v)
{}

string msrTechnical::asString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalPlacementKind);
}

void msrTechnical::print (ostream& os) const
{
  os <<
    "Technical " <<
    asString () <<
    ", line " << fInputLineNumber <<
//    ", accidental mark" << " = " << technicalAccidentalMarkKindAsString () <<
    endl;
}

ostream& operator << (ostream& os, const S_msrTechnical& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
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
  msrTechnicalWithInteger* o =
    new msrTechnicalWithInteger (
      inputLineNumber,
      technicalWithIntegerKind,
      technicalWithIntegerValue,
      technicalWithIntegerPlacementKind);
  assert (o != nullptr);
  return o;
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithInteger::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithInteger::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnicalWithInteger::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithInteger::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithInteger::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnicalWithInteger::browseData (basevisitor* v)
{}

string msrTechnicalWithInteger::asString () const
{
  stringstream s;

  s <<
    "[TechnicalWithInteger"
    ", fTechnicalWithIntegerKind: " << fTechnicalWithIntegerKind <<
    ", fTechnicalWithIntegerValue: " <<  fTechnicalWithIntegerValue <<
    ", fTechnicalWithIntegerPlacementKind: " << fTechnicalWithIntegerPlacementKind <<
    ']';

  return s.str ();
}

void msrTechnicalWithInteger::print (ostream& os) const
{
  os <<
    "[TechnicalWithInteger" <<
    ", fTechnicalWithIntegerKind: " << fTechnicalWithIntegerKind <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "fTechnicalWithIntegerValue" << " : " << fTechnicalWithIntegerValue <<
    endl <<

    setw (fieldWidth) <<
    "fTechnicalWithIntegerPlacementKind" << " : " << fTechnicalWithIntegerPlacementKind <<
    endl <<
    ']' <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrTechnicalWithInteger& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
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
  msrTechnicalWithFloat* o =
    new msrTechnicalWithFloat (
      inputLineNumber,
      technicalWithFloatKind,
      technicalWithFloatValue,
      technicalWithFloatPlacementKind);
  assert (o != nullptr);
  return o;
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithFloat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithFloat>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithFloat>*> (v)) {
        S_msrTechnicalWithFloat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithFloat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnicalWithFloat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithFloat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithFloat>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithFloat>*> (v)) {
        S_msrTechnicalWithFloat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithFloat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnicalWithFloat::browseData (basevisitor* v)
{}

string msrTechnicalWithFloat::asString () const
{
  stringstream s;

  s <<
    "TechnicalWithFloat" <<
    ", fTechnicalWithFloatKind: " << fTechnicalWithFloatKind <<
    ", fTechnicalWithFloatValue :'" << fTechnicalWithFloatValue << '\'' <<
    ", fTechnicalWithFloatPlacementKind: " << fTechnicalWithFloatPlacementKind <<
    ']';

  return s.str ();
}

void msrTechnicalWithFloat::print (ostream& os) const
{
  os <<
    "[TechnicalWithFloat" <<
    ", fTechnicalWithFloatKind: " << fTechnicalWithFloatKind <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "fTechnicalWithFloatValue" << " : " << fTechnicalWithFloatValue <<
    endl <<

    setw (fieldWidth) <<
    "fTechnicalWithFloatPlacementKind" << " : " << fTechnicalWithFloatPlacementKind <<
    endl <<
    ']' <<
    endl <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrTechnicalWithFloat& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTechnicalWithString msrTechnicalWithString::create (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  const string&              technicalWithStringValue,
  msrPlacementKind           technicalWithStringPlacementKind)
{
  msrTechnicalWithString* o =
    new msrTechnicalWithString (
      inputLineNumber,
      technicalWithStringKind,
      technicalWithStringTypeKind,
      technicalWithStringValue,
      technicalWithStringPlacementKind);
  assert (o != nullptr);
  return o;
}

msrTechnicalWithString::msrTechnicalWithString (
  int                        inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  msrTechnicalTypeKind       technicalWithStringTypeKind,
  const string&              technicalWithStringValue,
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithString::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithString::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTechnicalWithString::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTechnicalWithString::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTechnicalWithString::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTechnicalWithString::browseData (basevisitor* v)
{}

string msrTechnicalWithString::asString () const
{
  stringstream s;

  s <<
    "[TechnicalWithString" <<
    ", fTechnicalWithStringKind: " << fTechnicalWithStringKind <<
    ", fTechnicalWithStringTypeKind: " << fTechnicalWithStringTypeKind <<
    ", fTechnicalWithStringValue: \"" << fTechnicalWithStringValue << "\"" <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrTechnicalWithString::print (ostream& os) const
{
  os <<
    "[msrTechnicalWithString" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    endl <<
    setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind " << fTechnicalWithStringPlacementKind <<
    endl <<
    setw (fieldWidth) <<
    "fTechnicalWithStringValue" << " : \"" << fTechnicalWithStringValue << "\"" <<
    endl <<
    setw (fieldWidth) <<
    ", fTechnicalWithStringPlacementKind" << " : \"" << fTechnicalWithStringPlacementKind <<
    endl <<

    setw (fieldWidth) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrTechnicalWithString& elt)
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
