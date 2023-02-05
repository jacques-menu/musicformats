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

#include "msrBeams.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "msrOah.h"


namespace MusicFormats
{

// beams
//______________________________________________________________________________
std::string msrBeamKindAsString (
  msrBeamKind beamKind)
{
  std::string result;

  switch (beamKind) {
    case msrBeamKind::kBeam_UNKNOWN:
      result = "kBeam_UNKNOWN";
      break;
    case msrBeamKind::kBeamBegin:
      result = "kBeamBegin";
      break;
    case msrBeamKind::kBeamContinue:
      result = "kBeamContinue";
      break;
    case msrBeamKind::kBeamEnd:
      result = "kBeamEnd";
      break;
    case msrBeamKind::kBeamForwardHook:
      result = "kBeamForwardHook";
      break;
    case msrBeamKind::kBeamBackwardHook:
      result = "kBeamBackwardHook";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBeamKind& elt)
{
  os << msrBeamKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
{
  msrBeam* o =
    new msrBeam (
      inputLineNumber, number, beamKind);
  assert (o != nullptr);
  return o;
}

msrBeam::msrBeam (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
		std::stringstream ss;

    ss <<
      "Creating beam '" <<
      this->asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

msrBeam::~msrBeam ()
{}

void msrBeam::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrBeam::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeam::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrBeam::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeam::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

std::string msrBeam::asString () const
{
  std::stringstream ss;

  ss <<
    "[Beam " <<
    msrBeamKindAsString (fBeamKind) <<
    ", number: " << fBeamNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrBeam::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBeam& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
