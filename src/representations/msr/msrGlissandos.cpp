/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrGlissandos.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrGlissando msrGlissando::create (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  const string&        glissandoTextValue)
{
  msrGlissando* o =
    new msrGlissando (
      inputLineNumber,
      glissandoNumber,
      glissandoTypeKind,
      glissandoLineTypeKind,
      glissandoTextValue);
  assert (o != nullptr);
  return o;
}

msrGlissando::msrGlissando (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  const string&        glissandoTextValue)
    : msrElement (inputLineNumber)
{
  fGlissandoNumber = glissandoNumber;

  fGlissandoTypeKind     = glissandoTypeKind;
  fGlissandoLineTypeKind = glissandoLineTypeKind;

  fGlissandoTextValue = glissandoTextValue;
}

msrGlissando::~msrGlissando ()
{}

S_msrGlissando msrGlissando::createGlissandoNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "Creating a newborn clone of glissando '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrGlissando
    newbornClone =
      msrGlissando::create (
        fInputLineNumber,
        fGlissandoNumber,
        fGlissandoTypeKind,
        fGlissandoLineTypeKind,
        fGlissandoTextValue);

  return newbornClone;
}

string msrGlissando::msrGlissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind)
{
  string result;

  switch (glissandoTypeKind) {
    case msrGlissandoTypeKind::kGlissandoTypeNone:
      result = "kGlissandoTypeNone";
      break;
    case msrGlissandoTypeKind::kGlissandoTypeStart:
      result = "kGlissandoTypeStart";
      break;
    case msrGlissandoTypeKind::kGlissandoTypeStop:
      result = "kGlissandoTypeStop";
      break;
  } // switch

  return result;
}

void msrGlissando::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGlissando::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGlissando::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGlissando::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGlissando::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGlissando::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGlissando::browseData (basevisitor* v)
{}

string msrGlissando::asString () const
{
  stringstream s;

  s <<
    "[Glissando" <<
    ", fGlissandoNumber " << fGlissandoNumber <<
    ", fGlissandoTypeKind: " << fGlissandoTypeKind <<
    ", fGlissandoLineTypeKind: " << fGlissandoLineTypeKind <<
    ", fGlissandoTextValue: \"" << fGlissandoTextValue << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrGlissando::print (ostream& os) const
{
  os <<
    "[Glissando" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "fGlissandoNumber: " << fGlissandoNumber <<
    endl <<
    setw (fieldWidth) <<
    "fGlissandoTypeKind: " <<  fGlissandoTypeKind <<
    endl <<
    setw (fieldWidth) <<
    "fGlissandoLineTypeKind: " << fGlissandoLineTypeKind <<
    endl <<
    setw (fieldWidth) <<
    "fGlissandoTextValue: \"" << fGlissandoTextValue << "\"" <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrGlissando& elt)
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
