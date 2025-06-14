/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "msrBarChecks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  const mfInputLineNumber& inputLineNumber)
{
  return
    msrBarCheck::create (
      inputLineNumber,
      gNullMeasure); // JMI 0.9.74
}

S_msrBarCheck msrBarCheck::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrBarCheck* obj =
    new msrBarCheck (
      inputLineNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
}

S_msrBarCheck msrBarCheck::createWithNextBarPuristNumber (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   nextBarOriginalNumber,
  int                      nextBarPuristNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrBarCheck* obj =
    new msrBarCheck (
      inputLineNumber,
      nextBarOriginalNumber,
      nextBarPuristNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
}

msrBarCheck::msrBarCheck (const mfInputLineNumber& inputLineNumber)
  : msrMeasureElement (
      inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Creating a bare bar check" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrBarCheck::msrBarCheck (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Creating a bar check" <<
      ", upLinkToMeasure: " <<
      upLinkToMeasure->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureElementUpLinkToMeasure = upLinkToMeasure;
}

msrBarCheck::msrBarCheck (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   nextBarOriginalNumber,
  int                      nextBarPuristNumber,
  const S_msrMeasure&      upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Creating a bar check" <<
      ", nextBarOriginalNumber " <<
      nextBarOriginalNumber <<
      ", fNextBarPuristNumber " <<
      fNextBarPuristNumber <<
      ", upLinkToMeasure: " <<
      upLinkToMeasure->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextBarOriginalNumber = nextBarOriginalNumber;
  fNextBarPuristNumber = nextBarPuristNumber;

  fMeasureElementUpLinkToMeasure = upLinkToMeasure;
}

msrBarCheck::~msrBarCheck ()
{}

// void msrBarCheck::setBarCheckUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceBarChecks ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of bar check " <<
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
//   fMeasureElementUpLinkToMeasure = measure;
// }

void msrBarCheck::setNextBarPuristNumber (int puristNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Setting bar check next bar number to '" <<
      puristNumber <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextBarPuristNumber = puristNumber;
}

void msrBarCheck::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarCheck::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarCheck::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarCheck::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarCheck::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

std::string msrBarCheck::asString () const
{
  std::stringstream ss;

  ss <<
    "[BarCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrBarCheck::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBarCheck& elt)
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
