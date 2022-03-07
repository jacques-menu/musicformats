/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif



#include "msrScaling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "lpsrOah.h"


using namespace std;

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

#ifdef TRACING_IS_ENABLED
    const float
      optionsStaffGlobalSizeDefaultValue =
        gGlobalLpsrOahGroup->getStaffGlobalSizeDefaultValue ();

    if (gGlobalTracingOahGroup->getTraceGeometry ()) {
      gLogStream <<
        "fetchGlobalStaffSize():" <<
        endl;

      ++gIndenter;

      const int fieldWidth = 32;

      gLogStream << left <<
        setw (fieldWidth) <<
        "optionsGlobalStaffSize" << " : " <<
        optionsGlobalStaffSize <<
        endl <<
        setw (fieldWidth) <<
        "defaultTenthsToMillimetersRatio" << " : " <<
        defaultTenthsToMillimetersRatio <<
        endl <<
        setw (fieldWidth) <<
        "millimetersOverTenths" << " : " <<
        millimetersOverTenths <<
        endl <<

        setw (fieldWidth) <<
        "optionsStaffGlobalSizeDefaultValue" << " : " <<
        optionsStaffGlobalSizeDefaultValue <<
        endl <<
        setw (fieldWidth) <<
        "optionsGlobalStaffSize" << " : " <<
        optionsGlobalStaffSize <<
        endl <<
        setw (fieldWidth) <<
        "staffGlobalSizeHasBeenSet" << " : " <<
        staffGlobalSizeHasBeenSet <<
        endl <<

        setw (fieldWidth) <<
        "ratio" << " : " <<
        ratio <<
        endl <<

        setw (fieldWidth) <<
        "result" << " : " <<
        result <<
        endl;

      --gIndenter;
    }
#endif

    if (result < 1.0 || result > 100.0) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGeometry ()) {
        stringstream s;

        s <<
          "resulting staffsize " << result <<
          " is not between 1.0 and 100.0, replaced by 20.0:" <<
          endl;

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
      endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScaling::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScaling::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScaling::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScaling::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScaling::browseData (basevisitor* v)
{}

void msrScaling::print (ostream& os) const
{
  os <<
    "[Scaling" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 13;

  // relative to absolute lengths conversion
  os << left <<
    setw (fieldWidth) <<
    "millimeters" << " : " <<
    setprecision (2) << fMillimeters <<
    endl <<
    setw (fieldWidth) <<
    "tenths" << " : " <<
    setprecision (2) << fTenths <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrScaling& elt) {
  elt->print (os);
  return os;
}


}
