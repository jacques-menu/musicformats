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

#include "msrDynamics.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrDynamicKindAsString (
  msrDynamicKind dynamicKind)
{
/*
https://lilypond.org/doc/v2.24/Documentation/notation/expressive-marks-attached-to-notes.en.html#dynamics

The available dynamic marks are:
  \ppppp, \pppp, \ppp, \pp, \p,
  \mp,
  \mf,
  \f, \ff, \fff, \ffff, \fffff,
  \fp, \sf, \sff,
  \sp, \spp,
  \sfz, \rfz,
  and \n
*/

  std::string result;

  switch (dynamicKind) {
    case msrDynamicKind::kDynamic_UNKNOWN_:
      result = "*kDynamic_UNKNOWN_*";
      break;

    case msrDynamicKind::kDynamicF:
      result = "f";
      break;
    case msrDynamicKind::kDynamicFF:
      result = "ff";
      break;
    case msrDynamicKind::kDynamicFFF:
      result = "fff";
      break;
    case msrDynamicKind::kDynamicFFFF:
      result = "ffff";
      break;
    case msrDynamicKind::kDynamicFFFFF:
      result = "fffff";
      break;
    case msrDynamicKind::kDynamicFFFFFF:
      result = "ffffff";
      break;

    case msrDynamicKind::kDynamicP:
      result = "p";
      break;
    case msrDynamicKind::kDynamicPP:
      result = "pp";
      break;
    case msrDynamicKind::kDynamicPPP:
      result = "ppp";
      break;
    case msrDynamicKind::kDynamicPPPP:
      result = "pppp";
      break;
    case msrDynamicKind::kDynamicPPPPP:
      result = "ppppp";
      break;
    case msrDynamicKind::kDynamicPPPPPP:
      result = "pppppp";
      break;

    case msrDynamicKind::kDynamicMF:
      result = "mf";
      break;
    case msrDynamicKind::kDynamicMP:
      result = "mp";
      break;

/*
https://lilypond.org/doc/v2.24/Documentation/notation/expressive-marks-attached-to-notes.en.html#dynamics

The available dynamic marks are:
  \ppppp, \pppp, \ppp, \pp, \p,
  \mp,
  \mf,
  \f, \ff, \fff, \ffff, \fffff,
  \fp, \sf, \sff,
  \sp, \spp,
  \sfz, \rfz,
  and \n
*/

    case msrDynamicKind::kDynamicFP:
      result = "fp";
      break;
    case msrDynamicKind::kDynamicFZ:
      result = "fz";
      break;
    case msrDynamicKind::kDynamicPF:
      result = "pf";
      break;
    case msrDynamicKind::kDynamicRF:
      result = "rf";
      break;
    case msrDynamicKind::kDynamicSF:
      result = "sf";
      break;

    case msrDynamicKind::kDynamicRFZ:
      result = "rfz";
      break;
    case msrDynamicKind::kDynamicSFZ:
      result = "sfz";
      break;
    case msrDynamicKind::kDynamicSFP:
      result = "sfp";
      break;
    case msrDynamicKind::kDynamicSFPP:
      result = "sfpp";
      break;
    case msrDynamicKind::kDynamicSFFZ:
      result = "sffz";
      break;
    case msrDynamicKind::kDynamicSFZP:
      result = "sfzp";
      break;
    case msrDynamicKind::kDynamicN:
      result = "n";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDynamicKind& elt)
{
  os << msrDynamicKindAsString (elt);
  return os;
}

msrDynamicKind dynamicFromString (const std::string& theString)
{
  msrDynamicKind result = msrDynamicKind::kDynamic_UNKNOWN_;

  if (theString == "f") {
    result = msrDynamicKind::kDynamicF;
  }
  else if (theString == "f") {
    result = msrDynamicKind::kDynamicF;
  }
  else if (theString == "fff") {
    result = msrDynamicKind::kDynamicFFF;
  }
  else if (theString == "ffff") {
    result = msrDynamicKind::kDynamicFFFF;
  }
  else if (theString == "fffff") {
    result = msrDynamicKind::kDynamicFFFFF;
  }
  else if (theString == "ffffff") {
    result = msrDynamicKind::kDynamicFFFFFF;
  }

  else if (theString == "p") {
    result = msrDynamicKind::kDynamicP;
  }
  else if (theString == "pp") {
    result = msrDynamicKind::kDynamicPP;
  }
  else if (theString == "ppp") {
    result = msrDynamicKind::kDynamicPPP;
  }
  else if (theString == "pppp") {
    result = msrDynamicKind::kDynamicPPPP;
  }
  else if (theString == "ppppp") {
    result = msrDynamicKind::kDynamicPPPPP;
  }
  else if (theString == "pppppp") {
    result = msrDynamicKind::kDynamicPPPPPP;
  }

  else if (theString == "mf") {
    result = msrDynamicKind::kDynamicMF;
  }
  else if (theString == "mp") {
    result = msrDynamicKind::kDynamicMP;
  }
  else if (theString == "fp") {
    result = msrDynamicKind::kDynamicFP;
  }
  else if (theString == "fz") {
    result = msrDynamicKind::kDynamicFZ;
  }
  else if (theString == "pf") {
    result = msrDynamicKind::kDynamicPF;
  }
  else if (theString == "rf") {
    result = msrDynamicKind::kDynamicRF;
  }
  else if (theString == "sf") {
    result = msrDynamicKind::kDynamicSF;
  }

  else if (theString == "rfz") {
    result = msrDynamicKind::kDynamicRFZ;
  }
  else if (theString == "sfz") {
    result = msrDynamicKind::kDynamicSFZ;
  }
  else if (theString == "sfp") {
    result = msrDynamicKind::kDynamicSFP;
  }
  else if (theString == "sfpp") {
    result = msrDynamicKind::kDynamicSFPP;
  }
  else if (theString == "sffz") {
    result = msrDynamicKind::kDynamicSFFZ;
  }
  else if (theString == "sfzp") {
    result = msrDynamicKind::kDynamicSFZP;
  }
  else if (theString == "n") {
    result = msrDynamicKind::kDynamicN;
  }

  return result;
}

//______________________________________________________________________________
S_msrDynamic msrDynamic::create (
  int              inputLineNumber,
  msrDynamicKind   dynamicKind,
  msrPlacementKind dynamicPlacementKind)
{
  msrDynamic* obj =
    new msrDynamic (
      inputLineNumber,
      dynamicKind,
      dynamicPlacementKind);
    assert (obj != nullptr);
  return obj;
}

S_msrDynamic msrDynamic::createDynamicFromString (
  int                inputLineNumber,
  const std::string& dynamicsString,
  msrPlacementKind   dynamicPlacementKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Creating dynamic from string \"" <<
      dynamicsString <<
//       "\", dynamicsMeasureNumber: '" << dynamicsMeasureNumber << JMI 0.9.67
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return
    msrDynamic::create (
      inputLineNumber,
      dynamicFromString (dynamicsString),
      dynamicPlacementKind);
}

msrDynamic::msrDynamic (
  int              inputLineNumber,
  msrDynamicKind   dynamicKind,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDynamic::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDynamic>*
    p =
      dynamic_cast<visitor<S_msrDynamic>*> (v)) {
        S_msrDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDynamic::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrDynamic::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDynamic::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDynamic>*
    p =
      dynamic_cast<visitor<S_msrDynamic>*> (v)) {
        S_msrDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDynamic::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}


void msrDynamic::browseData (basevisitor* v)
{}

void msrDynamic::print (std::ostream& os) const
{
  os <<
    "Dynamics " <<
    fDynamicKind <<
    ", fDynamicPlacementKind: " <<
    fDynamicPlacementKind <<
    ", line " << fInputLineNumber <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDynamic& elt)
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
S_msrOtherDynamic msrOtherDynamic::create (
  int                inputLineNumber,
  const std::string& otherDynamicString,
  msrPlacementKind   otherDynamicPlacementKind)
{
  msrOtherDynamic* obj =
    new msrOtherDynamic (
      inputLineNumber,
      otherDynamicString,
      otherDynamicPlacementKind);
    assert (obj != nullptr);
  return obj;
}

msrOtherDynamic::msrOtherDynamic (
  int                inputLineNumber,
  const std::string& otherDynamicString,
  msrPlacementKind   otherDynamicPlacementKind)
    : msrElement (inputLineNumber)
{
  fOtherDynamicsString = otherDynamicString;

  fOtherDynamicPlacementKind = otherDynamicPlacementKind;
}

msrOtherDynamic::~msrOtherDynamic ()
{}

void msrOtherDynamic::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOtherDynamic::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOtherDynamic>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamic>*> (v)) {
        S_msrOtherDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOtherDynamic::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrOtherDynamic::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOtherDynamic::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOtherDynamic>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamic>*> (v)) {
        S_msrOtherDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOtherDynamic::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}


void msrOtherDynamic::browseData (basevisitor* v)
{}

std::string msrOtherDynamic::asString () const
{
  std::stringstream ss;

  ss <<
    "OtherDynamics '" <<
    ", fOtherDynamicsString: " <<
    fOtherDynamicsString <<
    ", fOtherDynamicPlacementKind: " <<
    fOtherDynamicPlacementKind <<
    "', line " << fInputLineNumber;

  return ss.str ();
}

void msrOtherDynamic::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrOtherDynamic& elt)
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
std::string msrCrescDecrescKindAsString (
  msrCrescDecrescKind crescDecrescKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrCrescDecrescKind& elt)
{
  os << msrCrescDecrescKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCrescDecresc msrCrescDecresc::create (
  int                 inputLineNumber,
  msrCrescDecrescKind crescDecrescKind)
{
  msrCrescDecresc* obj =
    new msrCrescDecresc (
      inputLineNumber,
      crescDecrescKind);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCrescDecresc::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCrescDecresc>*
    p =
      dynamic_cast<visitor<S_msrCrescDecresc>*> (v)) {
        S_msrCrescDecresc elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCrescDecresc::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrCrescDecresc::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCrescDecresc::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCrescDecresc>*
    p =
      dynamic_cast<visitor<S_msrCrescDecresc>*> (v)) {
        S_msrCrescDecresc elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCrescDecresc::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrCrescDecresc::browseData (basevisitor* v)
{}

void msrCrescDecresc::print (std::ostream& os) const
{
  os <<
    "[CrescDecresc " << fCrescDecrescKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrCrescDecresc& elt)
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
std::string msrWedgeKindAsString (
  msrWedgeKind wedgeKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrWedgeKind& elt)
{
  os << msrWedgeKindAsString (elt);
  return os;
}

std::string msrWedgeNienteKindAsString (
  msrWedgeNienteKind wedgeNienteKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrWedgeNienteKind& elt)
{
  os << msrWedgeNienteKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int                inputLineNumber,
  int                wedgeNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind,
  int                wedgeVoiceNumber)
{
  msrWedge* obj =
    new msrWedge (
      inputLineNumber,
      wedgeNumber,
      wedgeKind,
      wedgeNienteKind,
      wedgeLineTypeKind,
      wedgePlacementKind,
      wedgeVoiceNumber);
  assert (obj != nullptr);
  return obj;
}

msrWedge::msrWedge (
  int                inputLineNumber,
  int                wedgeNumber,
  msrWedgeKind       wedgeKind,
  msrWedgeNienteKind wedgeNienteKind,
  msrLineTypeKind    wedgeLineTypeKind,
  msrPlacementKind   wedgePlacementKind,
  int                wedgeVoiceNumber)
    : msrElement (inputLineNumber)
{
  fWedgeNumber = wedgeNumber;

  fWedgeKind = wedgeKind;

  fWedgeNienteKind = wedgeNienteKind;
  fWedgeLineTypeKind = wedgeLineTypeKind;
  fWedgePlacementKind = wedgePlacementKind;

  fWedgeVoiceNumber = wedgeVoiceNumber;
}

msrWedge::~msrWedge ()
{}

void msrWedge::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrWedge::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrWedge::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrWedge::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrWedge::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

std::string msrWedge::asString () const
{
  std::stringstream ss;

  ss <<
    "[Wedge " <<
    ", fWedgeNumber: " << fWedgeNumber <<
    ", fWedgeKind: " << fWedgeKind <<
    ", fWedgeVoiceNumber: " << fWedgeVoiceNumber <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrWedge::print (std::ostream& os) const
{
  os <<
    asString () <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 19;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fWedgeNienteKind" << ": " <<
    fWedgeNienteKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWedgeLineTypeKind" << ": " <<
    fWedgeLineTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWedgePlacementKind" << ": " <<
    fWedgePlacementKind <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrWedge& elt)
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
