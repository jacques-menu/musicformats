/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrSlides.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string msrSlideTypeKindAsString (
  msrSlideTypeKind slideTypeKind)
{
  string result;

  switch (slideTypeKind) {
    case msrSlideTypeKind::kSlideTypeNone:
      result = "kSlideTypeNone";
      break;
    case msrSlideTypeKind::kSlideTypeStart:
      result = "kSlideTypeStart";
      break;
    case msrSlideTypeKind::kSlideTypeStop:
      result = "kSlideTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSlideTypeKind& elt)
{
  os << msrSlideTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSlide msrSlide::create (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind,
  const string&    slideTextValue)
{
  msrSlide* o =
    new msrSlide (
      inputLineNumber,
      slideNumber,
      slideTypeKind,
      slideLineTypeKind,
      slideTextValue);
  assert (o != nullptr);
  return o;
}

msrSlide::msrSlide (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind,
  const string&    slideTextValue)
    : msrElement (inputLineNumber)
{
  fSlideNumber = slideNumber;

  fSlideTypeKind     = slideTypeKind;
  fSlideLineTypeKind = slideLineTypeKind;

  fSlideTextValue = slideTextValue;
}

msrSlide::~msrSlide ()
{}

S_msrSlide msrSlide::createSlideNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlides ()) {
    gLogStream <<
      "Creating a newborn clone of slide '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrSlide
    newbornClone =
      msrSlide::create (
        fInputLineNumber,
        fSlideNumber,
        fSlideTypeKind,
        fSlideLineTypeKind,
        fSlideTextValue);

  return newbornClone;
}

void msrSlide::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlide::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlide::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlide::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlide::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlide::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSlide::browseData (basevisitor* v)
{}

string msrSlide::asString () const
{
  stringstream s;

  s <<
    "Slide" <<
    ", fSlideNumber " << fSlideNumber <<
    ", fSlideTypeKind: " << fSlideTypeKind <<
    ", fSlideLineTypeKind: " << fSlideLineTypeKind <<
    ", fSlideTextValue: \"" << fSlideTextValue << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrSlide::print (ostream& os) const
{
  os <<
    "[Slide" <<
    ", line " << fInputLineNumber <<
    ']' <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "fSlideNumber" << fSlideNumber <<
    endl <<
    setw (fieldWidth) <<
    "fSlideTypeKind" << fSlideTypeKind <<
    endl <<
    setw (fieldWidth) <<
    "fSlideLineTypeKind" << fSlideLineTypeKind <<
    endl <<
    setw (fieldWidth) <<
    "fSlideTextValue" << " : \"" << fSlideTextValue << "\"" <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrSlide& elt)
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
