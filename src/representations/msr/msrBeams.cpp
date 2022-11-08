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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBeams.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

// beams
//______________________________________________________________________________
string msrBeamKindAsString (
  msrBeamKind beamKind)
{
  string result;

  switch (beamKind) {
    case msrBeamKind::kBeam_NO_:
      result = "***kBeam_NO_***";
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

ostream& operator << (ostream& os, const msrBeamKind& elt)
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Creating beam '" <<
      this->asString () <<
      "'" <<
      endl;
  }
#endif
}

msrBeam::~msrBeam ()
{}

void msrBeam::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeam::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeam::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeam::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeam::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

string msrBeam::asString () const
{
  stringstream s;

  s <<
    "[Beam " <<
    msrBeamKindAsString (fBeamKind) <<
    ", number: " << fBeamNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBeam::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrBeam& elt)
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
