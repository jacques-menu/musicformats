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

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif



#include "msrScaling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrScaling msrScaling::create (
  int   inputLineNumber,
  float millimeters,
  float tenths)
{
  msrScaling* o =
    new msrScaling (
      inputLineNumber,
      millimeters,
      tenths);
  assert (o != nullptr);
  return o;
}

msrScaling::msrScaling (
  int   inputLineNumber,
  float millimeters,
  float tenths)
    : msrElement (inputLineNumber)
{
  fMillimeters = millimeters;
  fTenths      = tenths;
}

msrScaling::~msrScaling ()
{}

S_msrScaling msrScaling::createMsrScalingNewbornClone ()
{
  S_msrScaling
    newbornClone =
      msrScaling::create (
        fInputLineNumber,
        fMillimeters,
        fTenths);

  return newbornClone;
}

float msrScaling::fetchGlobalStaffSize () const
{
  float result = 20; // JMI

  /* JMI
  const float optionsGlobalStaffSize =
      gGlobalLpsrOahGroup->getGlobalStaffSize ();

  const Bool
    staffGlobalSizeHasBeenSet =
      gGlobalLpsrOahGroup->getStaffGlobalSizeHasBeenSet ();

  float result = 0.0;

  if (staffGlobalSizeHasBeenSet) {
    // global-staff-size has been chosen by the user
    result = optionsGlobalStaffSize;
  }
  else {
    // global-staff-size has not been chosen by the user
    const float defaultTenthsToMillimetersRatio = 0.175f;

    float millimetersOverTenths =
      fMillimeters / fTenths;
    float ratio =
      millimetersOverTenths / defaultTenthsToMillimetersRatio;

    result = optionsGlobalStaffSize * ratio;

#ifdef OAH_TRACING_IS_ENABLED
    const float
      optionsStaffGlobalSizeDefaultValue =
        gGlobalLpsrOahGroup->getStaffGlobalSizeDefaultValue ();

    if (gGlobalTracingOahGroup->getTraceGeometry ()) {
      gLogStream <<
        "fetchGlobalStaffSize():" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 32;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "optionsGlobalStaffSize" << ": " <<
        optionsGlobalStaffSize <<
        std::endl <<
        std::setw (fieldWidth) <<
        "defaultTenthsToMillimetersRatio" << ": " <<
        defaultTenthsToMillimetersRatio <<
        std::endl <<
        std::setw (fieldWidth) <<
        "millimetersOverTenths" << ": " <<
        millimetersOverTenths <<
        std::endl <<

        std::setw (fieldWidth) <<
        "optionsStaffGlobalSizeDefaultValue" << ": " <<
        optionsStaffGlobalSizeDefaultValue <<
        std::endl <<
        std::setw (fieldWidth) <<
        "optionsGlobalStaffSize" << ": " <<
        optionsGlobalStaffSize <<
        std::endl <<
        std::setw (fieldWidth) <<
        "staffGlobalSizeHasBeenSet" << ": " <<
        staffGlobalSizeHasBeenSet <<
        std::endl <<

        std::setw (fieldWidth) <<
        "ratio" << ": " <<
        ratio <<
        std::endl <<

        std::setw (fieldWidth) <<
        "result" << ": " <<
        result <<
        std::endl;

      --gIndenter;
    }
#endif

    if (result < 1.0 || result > 100.0) {
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGeometry ()) {
        std::stringstream s;

        s <<
          "resulting staffsize " << result <<
          " is not between 1.0 and 100.0, replaced by 20.0:" <<
          std::endl;

        musicxmlWarning (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          s.str ());
      }
#endif

      result = optionsGlobalStaffSize;
    }
  }
*/

  return result;
}

void msrScaling::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScaling::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScaling::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrScaling::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScaling::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScaling::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrScaling::browseData (basevisitor* v)
{}

void msrScaling::print (std::ostream& os) const
{
  os <<
    "[Scaling" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 13;

  // relative to absolute lengths conversion
  os << std::left <<
    std::setw (fieldWidth) <<
    "millimeters" << ": " <<
    std::setprecision (2) << fMillimeters <<
    std::endl <<
    std::setw (fieldWidth) <<
    "tenths" << ": " <<
    std::setprecision (2) << fTenths <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrScaling& elt) {
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}


}
