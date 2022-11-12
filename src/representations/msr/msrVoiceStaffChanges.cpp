/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrMeasures.h"

#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrVoiceStaffChange msrVoiceStaffChange::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  S_msrStaff   staffToChangeTo)
{
  msrVoiceStaffChange* o =
    new msrVoiceStaffChange (
      inputLineNumber,
      upLinkToMeasure,
      staffToChangeTo);
  assert (o != nullptr);
  return o;
}

msrVoiceStaffChange::msrVoiceStaffChange (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  S_msrStaff   staffToChangeTo)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{
  fStaffToChangeTo = staffToChangeTo;
}

msrVoiceStaffChange::~msrVoiceStaffChange ()
{}

S_msrVoiceStaffChange msrVoiceStaffChange::createStaffChangeNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Creating a newborn clone of voice staff change '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

 S_msrVoiceStaffChange
    newbornClone =
      msrVoiceStaffChange::create (
        fInputLineNumber,
        nullptr, // will be set when voice staff change is appended to a measure JMI v0.9.66 PIM
        fStaffToChangeTo);

  return newbornClone;
}

void msrVoiceStaffChange::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoiceStaffChange::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoiceStaffChange::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrVoiceStaffChange::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoiceStaffChange::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrVoiceStaffChange>*
    p =
      dynamic_cast<visitor<S_msrVoiceStaffChange>*> (v)) {
        S_msrVoiceStaffChange elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoiceStaffChange::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoiceStaffChange::browseData (basevisitor* v)
{}

std::string msrVoiceStaffChange::asString () const
{
  std::stringstream s;

  s <<
    "VoiceStaffChange" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "staffToChangeTo: \"" << fStaffToChangeTo->getStaffName () << "\"";

  return s.str ();
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
