/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "msrSlides.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrSlideTypeKindAsString (
  msrSlideTypeKind slideTypeKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrSlideTypeKind& elt)
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
  const std::string&    slideTextValue)
{
  msrSlide* obj =
    new msrSlide (
      inputLineNumber,
      slideNumber,
      slideTypeKind,
      slideLineTypeKind,
      slideTextValue);
  assert (obj != nullptr);
  return obj;
}

msrSlide::msrSlide (
  int              inputLineNumber,
  int              slideNumber,
  msrSlideTypeKind slideTypeKind,
  msrLineTypeKind  slideLineTypeKind,
  const std::string&    slideTextValue)
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of slide '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlide::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlide::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSlide::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlide::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlide>*
    p =
      dynamic_cast<visitor<S_msrSlide>*> (v)) {
        S_msrSlide elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlide::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrSlide::browseData (basevisitor* v)
{}

std::string msrSlide::asString () const
{
  std::stringstream ss;

  ss <<
    "Slide" <<
    ", fSlideNumber " << fSlideNumber <<
    ", fSlideTypeKind: " << fSlideTypeKind <<
    ", fSlideLineTypeKind: " << fSlideLineTypeKind <<
    ", fSlideTextValue: \"" << fSlideTextValue << "\"" <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrSlide::print (std::ostream& os) const
{
  os <<
    "[Slide" <<
    ", line " << fInputLineNumber <<
    ']' <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSlideNumber" << fSlideNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSlideTypeKind" << fSlideTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSlideLineTypeKind" << fSlideLineTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSlideTextValue" << ": \"" << fSlideTextValue << "\"" <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSlide& elt)
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
