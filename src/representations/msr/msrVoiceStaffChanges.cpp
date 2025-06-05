/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrVoiceStaffChange msrVoiceStaffChange::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrStaff&   takeOffStaff,
  const S_msrStaff&   landingStaff)
{
  msrVoiceStaffChange* obj =
    new msrVoiceStaffChange (
      inputLineNumber,
      upLinkToMeasure,
      takeOffStaff,
      landingStaff);
  assert (obj != nullptr);
  return obj;
}

msrVoiceStaffChange::msrVoiceStaffChange (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrStaff&   takeOffStaff,
  const S_msrStaff&   landingStaff)
    : msrMeasureElement (
        inputLineNumber)
{
  fTakeOffStaff = takeOffStaff;
  fLandingStaff = landingStaff;
}

msrVoiceStaffChange::~msrVoiceStaffChange ()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChanges ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of voice staff change '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 S_msrVoiceStaffChange
    newbornClone =
      msrVoiceStaffChange::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fTakeOffStaff,
        fLandingStaff);

  return newbornClone;
}

// void msrVoiceStaffChange::setVoiceStaffChangeUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceStaffChanges ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of voice staff change " <<
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
//   fVoiceStaffChangeUpLinkToMeasure = measure;
// }

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoiceStaffChange::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoiceStaffChange::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoiceStaffChange::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrVoiceStaffChange::browseData (basevisitor* v)
{}

std::string msrVoiceStaffChange::asString () const
{
  std::stringstream ss;

  ss <<
    "[VoiceStaffChange" <<
    ", fTakeOffStaff: " << fTakeOffStaff->getStaffPathLikeName () <<
    ", fLandingStaff: " << fLandingStaff->getStaffPathLikeName () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrVoiceStaffChange::print (std::ostream& os) const
{
  os <<
    "[VoiceStaffChange" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 14;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fTakeOffStaff" << ": " << fTakeOffStaff->getStaffPathLikeName () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingStaff" << ": " << fLandingStaff->getStaffPathLikeName () <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrVoiceStaffChange& elt)
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
