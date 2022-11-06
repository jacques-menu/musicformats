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

#include "msrDynamics.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrDynamic msrDynamic::create (
  int              inputLineNumber,
  msrDynamicKind  dynamicKind,
  msrPlacementKind dynamicPlacementKind)
{
  msrDynamic* o =
    new msrDynamic (
      inputLineNumber,
      dynamicKind,
      dynamicPlacementKind);
    assert (o != nullptr);
  return o;
}

S_msrDynamic msrDynamic::createDynamicFromString (
  int              inputLineNumber,
  const string&    dynamicsString,
  msrPlacementKind dynamicPlacementKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDynamicss ()) {
    gLogStream <<
      "Creating dynamic from string \"" <<
      dynamicsString <<
      "\", dynamicsMeasureNumber: '" << dynamicsMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return
    msrDynamic::create (
      inputLineNumber,
      dynamicFromString (dynamicsString),
      dynamicPlacementKind);
}

msrDynamic::msrDynamic (
  int              inputLineNumber,
  msrDynamicKind  dynamicKind,
  msrPlacementKind dynamicPlacementKind)
    : msrElement (inputLineNumber)
{
  fDynamicKind = dynamicKind;

  fDynamicPlacementKind = dynamicPlacementKind;
}

msrDynamic::~msrDynamic ()
{}

void msrDynamic::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDynamic::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDynamic>*
    p =
      dynamic_cast<visitor<S_msrDynamic>*> (v)) {
        S_msrDynamic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDynamic::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDynamic::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDynamic::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDynamic>*
    p =
      dynamic_cast<visitor<S_msrDynamic>*> (v)) {
        S_msrDynamic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDynamic::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrDynamic::browseData (basevisitor* v)
{}

void msrDynamic::print (ostream& os) const
{
  os <<
    "Dynamics " <<
    msrDynamicKindAsString (fDynamicKind) <<
    ", dynamicPlacementKind: " <<
    msrPlacementKindAsString (fDynamicPlacementKind) <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator << (ostream& os, const S_msrDynamic& elt)
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
S_msrOtherDynamic msrOtherDynamic::create (
  int              inputLineNumber,
  const string&    otherDynamicString,
  msrPlacementKind otherDynamicPlacementKind)
{
  msrOtherDynamic* o =
    new msrOtherDynamic (
      inputLineNumber,
      otherDynamicString,
      otherDynamicPlacementKind);
    assert (o != nullptr);
  return o;
}

msrOtherDynamic::msrOtherDynamic (
  int              inputLineNumber,
  const string&    otherDynamicString,
  msrPlacementKind otherDynamicPlacementKind)
    : msrElement (inputLineNumber)
{
  fOtherDynamicsString = otherDynamicString;

  fOtherDynamicPlacementKind = otherDynamicPlacementKind;
}

msrOtherDynamic::~msrOtherDynamic ()
{}

void msrOtherDynamic::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOtherDynamic::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamic>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamic>*> (v)) {
        S_msrOtherDynamic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOtherDynamic::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOtherDynamic::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOtherDynamic::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOtherDynamic>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamic>*> (v)) {
        S_msrOtherDynamic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOtherDynamic::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrOtherDynamic::browseData (basevisitor* v)
{}

string msrOtherDynamic::asString () const
{
  stringstream s;

  s <<
    "OtherDynamics '" <<
    ", otherDynamicString: " <<
    fOtherDynamicsString <<
    ", otherDynamicPlacementKind: " <<
    msrPlacementKindAsString (fOtherDynamicPlacementKind) <<
    "', line " << fInputLineNumber;

  return s.str ();
}

void msrOtherDynamic::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrOtherDynamic& elt)
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
string msrCrescDecrescKindAsString (
  msrCrescDecrescKind crescDecrescKind)
{
  string result;

  switch (crescDecrescKind) {
    case msrCrescDecrescKind::kCrescDecrescCrescendo:
      result = "kCrescDecrescCrescendo";
      break;
    case msrCrescDecrescKind::kCrescDecrescDecrescendo:
      result = "kCrescDecrescDecrescendo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrCrescDecrescKind& elt)
{
  os << msrCrescDecrescKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCrescDecresc msrCrescDecresc::create (
  int                 inputLineNumber,
  msrCrescDecrescKind crescDecrescKind)
{
  msrCrescDecresc* o =
    new msrCrescDecresc (
      inputLineNumber,
      crescDecrescKind);
  assert (o != nullptr);
  return o;
}

msrCrescDecresc::msrCrescDecresc (
  int                 inputLineNumber,
  msrCrescDecrescKind crescDecrescKind)
    : msrElement (inputLineNumber)
{
  fCrescDecrescKind = crescDecrescKind;
}

msrCrescDecresc::~msrCrescDecresc ()
{}

void msrCrescDecresc::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCrescDecresc::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrCrescDecresc>*
    p =
      dynamic_cast<visitor<S_msrCrescDecresc>*> (v)) {
        S_msrCrescDecresc elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCrescDecresc::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCrescDecresc::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCrescDecresc::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCrescDecresc>*
    p =
      dynamic_cast<visitor<S_msrCrescDecresc>*> (v)) {
        S_msrCrescDecresc elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCrescDecresc::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCrescDecresc::browseData (basevisitor* v)
{}

void msrCrescDecresc::print (ostream& os) const
{
  os <<
    "[CrescDecresc " << fCrescDecrescKind <<
    ", line " << fInputLineNumber <<
    endl;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrCrescDecresc& elt)
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
string msrWedgeKindAsString (
  msrWedgeKind wedgeKind)
{
  string result;

  switch (wedgeKind) {
    case msrWedgeKind::kWedgeKindNone:
      result = "kWedgeKindNone";
      break;
    case msrWedgeKind::kWedgeCrescendo:
      result = "kWedgeCrescendo";
      break;
    case msrWedgeKind::kWedgeDecrescendo:
      result = "kWedgeDecrescendo";
      break;
    case msrWedgeKind::kWedgeStop:
      result = "kWedgeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrWedgeKind& elt)
{
  os << msrWedgeKindAsString (elt);
  return os;
}

string msrWedgeNienteKindAsString (
  msrWedgeNienteKind wedgeNienteKind)
{
  string result;

  switch (wedgeNienteKind) {
    case msrWedgeNienteKind::kWedgeNienteYes:
      result = "kWedgeNienteYes";
      break;
    case msrWedgeNienteKind::kWedgeNienteNo:
      result = "kWedgeNienteNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrWedgeNienteKind& elt)
{
  os << msrWedgeNienteKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber,
      wedgeKind,
      wedgeNienteKind,
      wedgeLineTypeKind,
      wedgePlacementKind);
  assert (o != nullptr);
  return o;
}

msrWedge::msrWedge (
  int                inputLineNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind          = wedgeKind;
  fWedgeNienteKind    = wedgeNienteKind;
  fWedgeLineTypeKind  = wedgeLineTypeKind;
  fWedgePlacementKind = wedgePlacementKind;
}

msrWedge::~msrWedge ()
{}

void msrWedge::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrWedge::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrWedge::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrWedge::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrWedge::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

void msrWedge::print (ostream& os) const
{
  os <<
    "[Wedge " << fWedgeKind <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "wedgeNienteKind" << " : " <<
    msrWedgeNienteKindAsString (
      fWedgeNienteKind) <<
    endl <<
    setw (fieldWidth) <<
    "wedgeLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fWedgeLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "wedgePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fWedgePlacementKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrWedge& elt)
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
