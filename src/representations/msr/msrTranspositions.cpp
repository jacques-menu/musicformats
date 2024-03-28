/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasureConstants.h"

#include "msrTranspositions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrTransposition msrTransposition::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int           transposeDiatonic,
  int           transposeChromatic,
  int           transposeOctaveChange,
  Bool          transposeDouble)
{
  msrTransposition* obj =
    new msrTransposition (
      inputLineNumber,
      upLinkToMeasure,
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
  assert (obj != nullptr);
  return obj;
}

S_msrTransposition msrTransposition::create (
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic,
  int           transposeOctaveChange,
  Bool          transposeDouble)
{
  return
    msrTransposition::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
}

msrTransposition::msrTransposition (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int           transposeDiatonic,
  int           transposeChromatic,
  int           transposeOctaveChange,
  Bool          transposeDouble)
    : msrMeasureElement (
        inputLineNumber)
{
  fTranspositionDiatonic     = transposeDiatonic;
  fTranspositionChromatic    = transposeChromatic;
  fTranspositionOctaveChange = transposeOctaveChange;
  fTranspositionDouble       = transposeDouble;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      "Creating transpose '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrTransposition::~msrTransposition ()
{}

// void msrTransposition::setTranspositionUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTraceTranspositions ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of transposition " <<
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
//   fTranspositionUpLinkToMeasure = measure;
// }

Bool msrTransposition::isEqualTo (
  const S_msrTransposition& otherTransposition) const
{
  if (! otherTransposition) {
    return false;
  }

  return
    fTranspositionDiatonic ==
      otherTransposition->fTranspositionDiatonic
      &&
    fTranspositionChromatic ==
      otherTransposition->fTranspositionChromatic
      &&
    fTranspositionOctaveChange ==
      otherTransposition->fTranspositionOctaveChange
      &&
    fTranspositionDouble ==
      otherTransposition->fTranspositionDouble;
}

void msrTransposition::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTransposition::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTransposition>*
    p =
      dynamic_cast<visitor<S_msrTransposition>*> (v)) {
        S_msrTransposition elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTransposition::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTransposition::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTransposition::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTransposition>*
    p =
      dynamic_cast<visitor<S_msrTransposition>*> (v)) {
        S_msrTransposition elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTransposition::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTransposition::browseData (basevisitor* v)
{}

std::string msrTransposition::asString () const
{
  std::stringstream ss;

  ss <<
    "[Transpose" <<
    ", fTranspositionDiatonic: " << fTranspositionDiatonic <<
    ", fTranspositionChromatic: " << fTranspositionChromatic <<
    ", fTranspositionOctaveChange: " << fTranspositionOctaveChange <<
    ", fTranspositionDouble: " << fTranspositionDouble <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrTransposition::print (std::ostream& os) const
{
  const int fieldWidth = 22;

  os <<
    "[Transpose" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTranspositionDiatonic" << ": " << fTranspositionDiatonic <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTranspositionChromatic" << ": " << fTranspositionChromatic <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTranspositionOctaveChange" << ": " << fTranspositionOctaveChange <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTranspositionDouble" << ": " << fTranspositionDouble <<
    std::endl << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTransposition& elt)
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
