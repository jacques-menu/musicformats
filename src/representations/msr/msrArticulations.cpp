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

#include "msrArticulations.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber,
      articulationKind,
      articulationPlacementKind);
  assert (o != nullptr);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;

  fArticulationPlacementKind = articulationPlacementKind;
}

msrArticulation::~msrArticulation ()
{}

string msrArticulationKindAsString (
  msrArticulationKind articulationKind)
{
  string result;

  switch (articulationKind) {
    case msrArticulationKind::k_NoArticulation:
      result = "k_NoArticulation";
      break;

    case msrArticulationKind::kArticulationAccent:
      result = "kArticulationAccent";
      break;
    case msrArticulationKind::kArticulationBreathMark:
      result = "kArticulationBreathMark";
      break;
    case msrArticulationKind::kArticulationCaesura:
      result = "kArticulationCaesura";
      break;
    case msrArticulationKind::kArticulationSpiccato:
      result = "kArticulationSpiccato";
      break;
    case msrArticulationKind::kArticulationStaccato:
      result = "kArticulationStaccato";
      break;
    case msrArticulationKind::kArticulationStaccatissimo:
      result = "kArticulationStaccatissimo";
      break;
    case msrArticulationKind::kArticulationStress:
      result = "kArticulationStress";
      break;
    case msrArticulationKind::kArticulationUnstress:
      result = "kArticulationUnstress";
      break;
    case msrArticulationKind::kArticulationDetachedLegato:
      result = "kArticulationDetachedLegato";
      break;
    case msrArticulationKind::kArticulationStrongAccent:
      result = "kArticulationStrongAccent";
      break;
    case msrArticulationKind::kArticulationTenuto:
      result = "kArticulationTenuto";
      break;
    case msrArticulationKind::kArticulationFermata:
      result = "kArticulationFermata";
      break;
    case msrArticulationKind::kArticulationArpeggiato:
      result = "kArticulationArpeggiato";
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      result = "kArticulationNonArpeggiato";
      break;
    case msrArticulationKind::kArticulationDoit:
      result = "kArticulationDoit";
      break;
    case msrArticulationKind::kArticulationFalloff:
      result = "kArticulationFalloff";
      break;
    case msrArticulationKind::kArticulationPlop:
      result = "kArticulationPlop";
      break;
    case msrArticulationKind::kArticulationScoop:
      result = "kArticulationScoop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrArticulationKind& elt)
{
  os << msrArticulationKindAsString (elt);
  return os;
}

string msrArticulation::msrArticulationKindAsString () const
{
  return
    msrArticulationKindAsString (
      fArticulationKind);
}

string msrArticulation::articulationPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fArticulationPlacementKind);
}

void msrArticulation::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrArticulation::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrArticulation::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrArticulation::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrArticulation::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

string msrArticulation::asString () const
{
  stringstream s;

  s <<
    "[Articulation " <<
    msrArticulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrArticulation::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrArticulation& elt)
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
S_msrFermata msrFermata::create (
  int                inputLineNumber,
  msrFermataKind     fermataKind,
  msrFermataTypeKind fermataTypeKind)
{
  msrFermata* o =
    new msrFermata (
      inputLineNumber,
      fermataKind,
      fermataTypeKind);
  assert (o != nullptr);
  return o;
}

msrFermata::msrFermata (
    int                inputLineNumber,
    msrFermataKind     fermataKind,
    msrFermataTypeKind fermataTypeKind)
    : msrArticulation (
      inputLineNumber,
      msrArticulationKind::kArticulationFermata,
      msrPlacementKind::k_NoPlacement) // temporary, JMI TEMP
{
  fFermataKind = fermataKind;
  fFermataTypeKind = fermataTypeKind;
}

msrFermata::~msrFermata ()
{}

void msrFermata::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFermata::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFermata::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFermata::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFermata::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFermata::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFermata::browseData (basevisitor* v)
{}

string msrFermata::msrFermataKindAsString (
  msrFermataKind fermataKind)
{
  string result;

  switch (fermataKind) {
    case msrFermataKind::kArticulationFermataNormal:
      result = "kArticulationFermataNormal";
      break;
    case msrFermataKind::kArticulationFermataAngled:
      result = "kArticulationFermataAngled";
      break;
    case msrFermataKind::kArticulationFermataSquare:
      result = "kArticulationFermataSquare";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrFermataKind& elt)
{
  os << msrFermataKindAsString (elt);
  return os;
}

string msrFermata::msrFermataTypeKindAsString (
  msrFermataTypeKind fermataTypeKind)
{
  string result;

  switch (fermataTypeKind) {
    case msrFermataTypeKind::kArticulationFermataTypeNone:
      result = "kArticulationFermataTypeNone";
      break;
    case msrFermataTypeKind::kArticulationFermataTypeUpright:
      result = "kArticulationFermataTypeUpright";
      break;
    case msrFermataTypeKind::kArticulationFermataTypeInverted:
      result = "kArticulationFermataTypeInverted";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrFermataTypeKind& elt)
{
  os << msrFermataTypeKindAsString (elt);
  return os;
}

string msrFermata::asString () const
{
  stringstream s;

  s <<
    "[Fermata" <<
    ", " <<
    msrFermataKindAsString (fFermataKind) <<
    ", " <<
    msrFermataTypeKindAsString (fFermataTypeKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrFermata::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrFermata& elt)
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
S_msrArpeggiato msrArpeggiato::create (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
{
  msrArpeggiato* o =
    new msrArpeggiato (
      inputLineNumber,
      arpeggiatoPlacementKind,
      arpeggiatoDirectionKind,
      arpeggiatoNumber);
  assert (o != nullptr);
  return o;
}

msrArpeggiato::msrArpeggiato (
  int              inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kArticulationArpeggiato,
      arpeggiatoPlacementKind)
{
  fArpeggiatoDirectionKind = arpeggiatoDirectionKind;

  fArpeggiatoNumber = arpeggiatoNumber;
}

msrArpeggiato::~msrArpeggiato ()
{}

string msrArpeggiato::arpeggiatoDirectionKindAsString () const
{
  return
    msrDirectionKindAsString (fArpeggiatoDirectionKind);
}

void msrArpeggiato::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrArpeggiato::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrArpeggiato::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrArpeggiato::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrArpeggiato::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrArpeggiato::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrArpeggiato::browseData (basevisitor* v)
{}

string msrArpeggiato::asString () const
{
  stringstream s;

  s <<
    "[Arpeggiato " <<
    msrArticulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    arpeggiatoDirectionKindAsString () <<
    ", number: " <<
    fArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrArpeggiato::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrArpeggiato& elt)
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
S_msrNonArpeggiato msrNonArpeggiato::create (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
{
  msrNonArpeggiato* o =
    new msrNonArpeggiato (
      inputLineNumber,
      nonArpeggiatoPlacementKind,
      nonArpeggiatoTypeKind,
      nonArpeggiatoNumber);
  assert (o != nullptr);
  return o;
}

msrNonArpeggiato::msrNonArpeggiato (
  int                      inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      kArticulationNonArpeggiato,
      nonArpeggiatoPlacementKind)
{
  fNonArpeggiatoTypeKind = nonArpeggiatoTypeKind;

  fNonArpeggiatoNumber = nonArpeggiatoNumber;
}

msrNonArpeggiato::~msrNonArpeggiato ()
{}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString (
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind)
{
  string result;

  switch (nonArpeggiatoTypeKind) {
    case msrNonArpeggiato::kArticulationNonArpeggiatoTypeNone:
      result = "nonArpeggiatoTypeNone";
      break;
    case msrNonArpeggiato::kArticulationNonArpeggiatoTypeTop:
      result = "nonArpeggiatoTypeTop";
      break;
    case msrNonArpeggiato::kArticulationNonArpeggiatoTypeBottom:
      result = "nonArpeggiatoTypeBottom";
      break;
  } // switch

  return result;
}

string msrNonArpeggiato::nonArpeggiatoTypeKindAsString () const
{
  return
    nonArpeggiatoTypeKindAsString (fNonArpeggiatoTypeKind);
}

void msrNonArpeggiato::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNonArpeggiato::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNonArpeggiato::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNonArpeggiato::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNonArpeggiato::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNonArpeggiato::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNonArpeggiato::browseData (basevisitor* v)
{}

string msrNonArpeggiato::asString () const
{
  stringstream s;

  s <<
    "NonArpeggiato " <<
    msrArticulationKindAsString () <<
    ", " <<
    articulationPlacementKindAsString () <<
    ", " <<
    nonArpeggiatoTypeKindAsString () <<
    ", number: " <<
    fNonArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrNonArpeggiato::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrNonArpeggiato& elt)
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
