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

#include "visitor.h"

#include "mfPreprocessorSettings.h"

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
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating hiddenMeasureAndBarLine" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrHiddenMeasureAndBarLine* obj =
    new msrHiddenMeasureAndBarLine (
      inputLineNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
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

// void msrHiddenMeasureAndBarLine::setHiddenMeasureAndBarLineUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures () || gTraceOahGroup->getTraceBarLines ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of hidden measure and bar line " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fHiddenMeasureAndBarLineUpLinkToMeasure = measure;
// }

void msrHiddenMeasureAndBarLine::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHiddenMeasureAndBarLine::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrHiddenMeasureAndBarLine::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHiddenMeasureAndBarLine::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
    ", measurePosition: " << fMeasureElementMeasurePosition.asString () <<
    ", line " << fInputStartLineNumber;

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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
