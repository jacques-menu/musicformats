/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "msrGlissandos.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrGlissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrGlissandoTypeKind& elt)
{
  os << msrGlissandoTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrGlissando msrGlissando::create (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  const std::string&   glissandoTextValue)
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
  const std::string&   glissandoTextValue)
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "Creating a newborn clone of glissando '" <<
      asString () <<
      "'" <<
      std::endl;
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

void msrGlissando::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGlissando::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGlissando::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrGlissando::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGlissando::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGlissando::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrGlissando::browseData (basevisitor* v)
{}

std::string msrGlissando::asString () const
{
  std::stringstream s;

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

void msrGlissando::print (std::ostream& os) const
{
  os <<
    "[Glissando" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGlissandoNumber: " << fGlissandoNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGlissandoTypeKind: " <<  fGlissandoTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGlissandoLineTypeKind: " << fGlissandoLineTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGlissandoTextValue: \"" << fGlissandoTextValue << "\"" <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrGlissando& elt)
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
