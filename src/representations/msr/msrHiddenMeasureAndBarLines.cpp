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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrHiddenMeasureAndBarLines.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLine msrHiddenMeasureAndBarLine::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasures ()) {
		std::stringstream ss;

    ss <<
      "Creating hiddenMeasureAndBarLine" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  msrHiddenMeasureAndBarLine* o =
    new msrHiddenMeasureAndBarLine (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrHiddenMeasureAndBarLine msrHiddenMeasureAndBarLine::create (
  int                 inputLineNumber)
{
  return
    msrHiddenMeasureAndBarLine::create (
      inputLineNumber,
      gNullMeasure); // set later in setMeasureElementUpLinkToMeasure()
}

msrHiddenMeasureAndBarLine::msrHiddenMeasureAndBarLine (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrHiddenMeasureAndBarLine::~msrHiddenMeasureAndBarLine ()
{}

void msrHiddenMeasureAndBarLine::setHiddenMeasureAndBarLineUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLog <<
      "==> Setting the uplink to measure of hidden measure and bar line " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fHiddenMeasureAndBarLineUpLinkToMeasure = measure;
}

void msrHiddenMeasureAndBarLine::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHiddenMeasureAndBarLine::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrHiddenMeasureAndBarLine::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHiddenMeasureAndBarLine::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrHiddenMeasureAndBarLine::browseData (basevisitor* v)
{}

std::string msrHiddenMeasureAndBarLine::asString () const
{
  std::stringstream ss;

  ss <<
    "HiddenMeasureAndBarLine" <<
    ", measurePosition: " << fMeasurePosition <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrHiddenMeasureAndBarLine::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHiddenMeasureAndBarLine& elt)
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
