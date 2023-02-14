/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfStaticSettings.h"

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
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrStaff&   staffToChangeTo)
{
  msrVoiceStaffChange* o =
    new msrVoiceStaffChange (
      inputLineNumber,
      upLinkToMeasure,
      staffToChangeTo);
  assert (o != nullptr);
  return o;
}

S_msrVoiceStaffChange msrVoiceStaffChange::create (
  int                 inputLineNumber,
  const S_msrStaff&   staffToChangeTo)
{
  return
    msrVoiceStaffChange::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      staffToChangeTo);
}

msrVoiceStaffChange::msrVoiceStaffChange (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrStaff&   staffToChangeTo)
    : msrMeasureElement (
        inputLineNumber)
{
  fStaffToChangeTo = staffToChangeTo;
}

msrVoiceStaffChange::~msrVoiceStaffChange ()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffChanges ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of voice staff change '" <<
      asString () <<
      "'" <<
      std::endl;

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
        fStaffToChangeTo);

  return newbornClone;
}

void msrVoiceStaffChange::setVoiceStaffChangeUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLog <<
      "==> Setting the uplink to measure of voice staff change " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceStaffChangeUpLinkToMeasure = measure;
}

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoiceStaffChange::acceptIn ()" <<
      std::endl;

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
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()" <<
            std::endl;

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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoiceStaffChange::acceptOut ()" <<
      std::endl;

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
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoiceStaffChange::visitEnd ()" <<
            std::endl;

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
    "VoiceStaffChange" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "staffToChangeTo: \"" << fStaffToChangeTo->getStaffName () << "\"";

  return ss.str ();
}

void msrVoiceStaffChange::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrVoiceStaffChange& elt)
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
