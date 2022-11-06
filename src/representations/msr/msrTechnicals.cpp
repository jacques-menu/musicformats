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

#include "msrTechnicals.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

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

string msrTechnical::msrTechnicalKindAsString () const
{
  string result;

  switch (fTechnicalKind) {
    case msrTechnical::kArrow:
      result = "kArrow";
      break;
    case msrTechnical::kDoubleTongue:
      result = "kDoubleTongue";
      break;
    case msrTechnical::kDownBow:
      result = "kDownBow";
      break;
    case msrTechnical::kFingernails:
      result = "kFingernails";
      break;
    case msrTechnical::kHarmonic:
      result = "kHarmonic";
      break;
    case msrTechnical::kHeel:
      result = "kHeel";
      break;
    case msrTechnical::kHole:
      result = "kHole";
      break;
    case msrTechnical::kOpenString:
      result = "kOpenString";
      break;
    case msrTechnical::kSnapPizzicato:
      result = "kSnapPizzicato";
      break;
    case msrTechnical::kStopped:
      result = "kStopped";
      break;
    case msrTechnical::kTap:
      result = "kTap";
      break;
    case msrTechnical::kThumbPosition:
      result = "kThumbPosition";
      break;
    case msrTechnical::kToe:
      result = "kToe";
      break;
    case msrTechnical::kTripleTongue:
      result = "kTripleTongue";
      break;
    case msrTechnical::kUpBow:
      result = "kUpBow";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSyllableKind& elt)
{
  os << msrSyllableKindAsString (elt);
  return os;
}

string msrTechnical::technicalPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalPlacementKind);
}

/*
string msrTechnical::technicalAccidentalMarkKindAsString () const
{
  string result;

  switch (fTechnicalAccidentalMarkKind) {
    case msrTechnical::msrAlterationKind::kAlterationNatural:
      result = "natural";
      break;
    case msrTechnical::msrAlterationKind::kAlterationSharp:
      result = "sharp";
      break;
    case msrTechnical::msrAlterationKind::kAlterationFlat:
      result = "flat";
      break;
  } // switch

  return result;
}
*/

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

string msrTechnicalWithIntegerKindAsString (
  msrTechnicalWithIntegerKind technicalWithIntegerKind) const
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

ostream& operator << (ostream& os, const msrSyllableKind& elt)
{
  os << msrSyllableKindAsString (elt);
  return os;
}

string msrTechnicalWithInteger::technicalWithIntegerPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalWithIntegerPlacementKind);
}

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
    msrTechnicalWithIntegerKindAsString () <<
    ", fTechnicalWithIntegerValue: '" <<
    fTechnicalWithIntegerValue <<
    "', placement " <<
    technicalWithIntegerPlacementKindAsString () <<
    ']';

  return s.str ();
}

void msrTechnicalWithInteger::print (ostream& os) const
{
  os <<
    "TechnicalWithInteger" <<
    ", " << msrTechnicalWithIntegerKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "value" << " : " <<
    fTechnicalWithIntegerValue <<
    endl <<

    setw (fieldWidth) <<
    "placement" << " : " <<
    technicalWithIntegerPlacementKindAsString () <<
    endl <<

    setw (fieldWidth) <<
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

string msrTechnicalWithFloat::msrTechnicalWithFloatKindAsString () const
{
  string result;

  switch (fTechnicalWithFloatKind) {
    case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
      result = "bend";
      break;
  } // switch

  return result;
}

string msrTechnicalWithFloat::technicalWithFloatPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalWithFloatPlacementKind);
}

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
    msrTechnicalWithFloatKindAsString () <<
    ", value '" <<
    fTechnicalWithFloatValue <<
    "', placement " <<
    technicalWithFloatPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithFloat::print (ostream& os) const
{
  os <<
    "TechnicalWithFloat" <<
    ", " << msrTechnicalWithFloatKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "value" << " : " <<
    fTechnicalWithFloatValue <<
    endl <<

    setw (fieldWidth) <<
    "placement" << " : " <<
    technicalWithFloatPlacementKindAsString () <<
    endl <<

    setw (fieldWidth) <<
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

string msrTechnicalWithString::msrTechnicalWithStringKindAsString () const
{
  string result;

  switch (fTechnicalWithStringKind) {
    case msrTechnicalWithStringKind::kHammerOn:
      result = "HammerOn";
      break;
    case msrTechnicalWithStringKind::kHandbell:
      result = "Handbell";
      break;
    case msrTechnicalWithStringKind::kOtherTechnical:
      result = "OtherTechnical";
      break;
    case msrTechnicalWithStringKind::kPluck:
      result = "Pluck";
      break;
    case msrTechnicalWithStringKind::kPullOff:
      result = "PullOff";
      break;
  } // switch

  result +=
    " \"" + fTechnicalWithStringValue + "\"";

  return result;
}

string msrTechnicalWithString::technicalWithStringTypeKindAsString () const
{
  return
    msrTechnicalTypeKindAsString (
      fTechnicalWithStringTypeKind);
}

string msrTechnicalWithString::technicalWithStringPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fTechnicalWithStringPlacementKind);
}

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
    msrTechnicalWithStringKindAsString () <<
    ", " << technicalWithStringTypeKindAsString () <<
    ", value \"" <<
    fTechnicalWithStringValue <<
    "\", placement " <<
    technicalWithStringPlacementKindAsString ();

  return s.str ();
}

void msrTechnicalWithString::print (ostream& os) const
{
  os <<
    "TechnicalWithString" <<
    ", " << msrTechnicalWithStringKindAsString () <<
    ", " << technicalWithStringTypeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "value" << " : \"" <<
    fTechnicalWithStringValue <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "placement" << " : " <<
    technicalWithStringPlacementKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    endl;

  --gIndenter;
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
