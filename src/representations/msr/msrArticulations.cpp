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

#include "visitor.h"

#include "msrArticulations.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  const mfInputLineNumber& inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
{
  msrArticulation* obj =
    new msrArticulation (
      inputLineNumber,
      articulationKind,
      articulationPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrArticulation::msrArticulation (
  const mfInputLineNumber& inputLineNumber,
  msrArticulationKind articulationKind,
  msrPlacementKind    articulationPlacementKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;

  fArticulationPlacementKind = articulationPlacementKind;
}

msrArticulation::~msrArticulation ()
{}

std::string msrArticulationKindAsString (
  msrArticulationKind articulationKind)
{
  std::string result;

  switch (articulationKind) {
    case msrArticulationKind::kArticulation_UNKNOWN_:
      result = "kArticulation_UNKNOWN_";
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

std::ostream& operator << (std::ostream& os, const msrArticulationKind& elt)
{
  os << msrArticulationKindAsString (elt);
  return os;
}

void msrArticulation::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrArticulation::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrArticulation::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrArticulation::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrArticulation::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

std::string msrArticulation::asString () const
{
  std::stringstream ss;

  ss <<
    "[Articulation " <<
    ", fArticulationKind: " << fArticulationKind <<
    ", fArticulationPlacementKind: " << fArticulationPlacementKind <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrArticulation::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrArticulation& elt)
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
S_msrFermata msrFermata::create (
  const mfInputLineNumber& inputLineNumber,
  msrFermataShapeKind        fermataShapeKind,
  msrArticulationFermataType fermataTypeKind)
{
  msrFermata* obj =
    new msrFermata (
      inputLineNumber,
      fermataShapeKind,
      fermataTypeKind);
  assert (obj != nullptr);
  return obj;
}

msrFermata::msrFermata (
  const mfInputLineNumber& inputLineNumber,
  msrFermataShapeKind        fermataShapeKind,
  msrArticulationFermataType fermataTypeKind)
    : msrArticulation (
      inputLineNumber,
      msrArticulationKind::kArticulationFermata,
      msrPlacementKind::kPlacement_UNKNOWN_) // temporary, JMI TEMP
{
  fFermataShapeKind = fermataShapeKind;
  fFermataTypeKind = fermataTypeKind;
}

msrFermata::~msrFermata ()
{}

void msrFermata::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFermata::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFermata::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrFermata::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFermata::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFermata>*
    p =
      dynamic_cast<visitor<S_msrFermata>*> (v)) {
        S_msrFermata elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFermata::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrFermata::browseData (basevisitor* v)
{}

std::string msrFermataShapeKindAsString (
  msrFermataShapeKind fermataShapeKind)
{
  std::string result;

  switch (fermataShapeKind) {
    case msrFermataShapeKind::kArticulationFermataNormal:
      result = "kArticulationFermataNormal";
      break;
    case msrFermataShapeKind::kArticulationFermataAngled:
      result = "kArticulationFermataAngled";
      break;
    case msrFermataShapeKind::kArticulationFermataSquare:
      result = "kArticulationFermataSquare";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrFermataShapeKind& elt)
{
  os << msrFermataShapeKindAsString (elt);
  return os;
}

std::string msrArticulationFermataTypeAsString (
  msrArticulationFermataType fermataTypeKind)
{
  std::string result;

  switch (fermataTypeKind) {
    case msrArticulationFermataType::kArticulationFermataTypeNone:
      result = "kArticulationFermataTypeNone";
      break;
    case msrArticulationFermataType::kArticulationFermataTypeUpright:
      result = "kArticulationFermataTypeUpright";
      break;
    case msrArticulationFermataType::kArticulationFermataTypeInverted:
      result = "kArticulationFermataTypeInverted";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrArticulationFermataType& elt)
{
  os << msrArticulationFermataTypeAsString (elt);
  return os;
}

std::string msrFermata::asString () const
{
  std::stringstream ss;

  ss <<
    "[Fermata" <<
    ", fFermataShapeKind: " << fFermataShapeKind <<
    ", fFermataTypeKind: " << fFermataTypeKind <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrFermata::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrFermata& elt)
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
S_msrArpeggiato msrArpeggiato::create (
  const mfInputLineNumber& inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
{
  msrArpeggiato* obj =
    new msrArpeggiato (
      inputLineNumber,
      arpeggiatoPlacementKind,
      arpeggiatoDirectionKind,
      arpeggiatoNumber);
  assert (obj != nullptr);
  return obj;
}

msrArpeggiato::msrArpeggiato (
  const mfInputLineNumber& inputLineNumber,
  msrPlacementKind arpeggiatoPlacementKind,
  msrDirectionKind arpeggiatoDirectionKind,
  int              arpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      msrArticulationKind::kArticulationArpeggiato,
      arpeggiatoPlacementKind)
{
  fArpeggiatoDirectionKind = arpeggiatoDirectionKind;

  fArpeggiatoNumber = arpeggiatoNumber;
}

msrArpeggiato::~msrArpeggiato ()
{}

std::string msrArpeggiato::arpeggiatoDirectionKindAsString () const
{
  return
    msrDirectionKindAsString (fArpeggiatoDirectionKind);
}

void msrArpeggiato::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrArpeggiato::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrArpeggiato::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrArpeggiato::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrArpeggiato::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrArpeggiato>*> (v)) {
        S_msrArpeggiato elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrArpeggiato::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrArpeggiato::browseData (basevisitor* v)
{}

std::string msrArpeggiato::asString () const
{
  std::stringstream ss;

  ss <<
    "[Arpeggiato " <<
    ", fArticulationPlacementKind: " << fArticulationPlacementKind <<
    ", " << arpeggiatoDirectionKindAsString () <<
    ", fArpeggiatoNumber: " << fArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrArpeggiato::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrArpeggiato& elt)
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
S_msrNonArpeggiato msrNonArpeggiato::create (
  const mfInputLineNumber& inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
{
  msrNonArpeggiato* obj =
    new msrNonArpeggiato (
      inputLineNumber,
      nonArpeggiatoPlacementKind,
      nonArpeggiatoTypeKind,
      nonArpeggiatoNumber);
  assert (obj != nullptr);
  return obj;
}

msrNonArpeggiato::msrNonArpeggiato (
  const mfInputLineNumber& inputLineNumber,
  msrPlacementKind         nonArpeggiatoPlacementKind,
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
  int                      nonArpeggiatoNumber)
    : msrArticulation (
      inputLineNumber,
      msrArticulationKind::kArticulationNonArpeggiato,
      nonArpeggiatoPlacementKind)
{
  fNonArpeggiatoTypeKind = nonArpeggiatoTypeKind;

  fNonArpeggiatoNumber = nonArpeggiatoNumber;
}

msrNonArpeggiato::~msrNonArpeggiato ()
{}

std::string msrNonArpeggiatoTypeKindAsString (
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind)
{
  std::string result;

  switch (nonArpeggiatoTypeKind) {
    case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeNone:
      result = "kArticulationNonArpeggiatoTypeNone";
      break;
    case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeTop:
      result = "kArticulationNonArpeggiatoTypeTop";
      break;
    case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeBottom:
      result = "kArticulationNonArpeggiatoTypeBottom";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNonArpeggiatoTypeKind& elt)
{
  os << msrNonArpeggiatoTypeKindAsString (elt);
  return os;
}

void msrNonArpeggiato::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNonArpeggiato::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNonArpeggiato::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrNonArpeggiato::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNonArpeggiato::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
  if (visitor<S_msrNonArpeggiato>*
    p =
      dynamic_cast<visitor<S_msrNonArpeggiato>*> (v)) {
        S_msrNonArpeggiato elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNonArpeggiato::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrNonArpeggiato::browseData (basevisitor* v)
{}

std::string msrNonArpeggiato::asString () const
{
  std::stringstream ss;

  ss <<
    "[NonArpeggiato " <<
    ", fArticulationPlacementKind: " << fArticulationPlacementKind <<
    ", fNonArpeggiatoTypeKind: " << fNonArpeggiatoTypeKind <<
    ", fNonArpeggiatoNumber: " << fNonArpeggiatoNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrNonArpeggiato::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrNonArpeggiato& elt)
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
