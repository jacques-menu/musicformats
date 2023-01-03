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

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "msrPitchesNames.h"

#include "msrStavesDetails.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrStaffTypeKindAsString (
  msrStaffTypeKind staffTypeKind)
{
  std::string result;

  switch (staffTypeKind) {
    case msrStaffTypeKind::kStaffTypeRegular:
      result = "kStaffTypeRegular";
      break;
    case msrStaffTypeKind::kStaffTypeOssia:
      result = "kStaffTypeOssia";
      break;
    case msrStaffTypeKind::kStaffTypeCue:
      result = "kStaffTypeCue";
      break;
    case msrStaffTypeKind::kStaffTypeEditorial:
      result = "kStaffTypeEditorial";
      break;
    case msrStaffTypeKind::kStaffTypeAlternate:
      result = "kStaffTypeAlternate";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrStaffTypeKind& elt)
{
  os << msrStaffTypeKindAsString (elt);
  return os;
}

std::string msrShowFretsKindAsString (
  msrShowFretsKind showFretsKind)
{
  std::string result;

  switch (showFretsKind) {
    case msrShowFretsKind::kShowFretsNumbers:
      result = "kShowFretsNumbers";
      break;
    case msrShowFretsKind::kShowFretsLetters:
      result = "kShowFretsLetters";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrShowFretsKind& elt)
{
  os << msrShowFretsKindAsString (elt);
  return os;
}

std::string msrPrintSpacingKindAsString (
  msrPrintSpacingKind printSpacingKind)
{
  std::string result;

  switch (printSpacingKind) {
    case msrPrintSpacingKind::kPrintSpacingYes:
      result = "kPrintSpacingYes";
      break;
    case msrPrintSpacingKind::kPrintSpacingNo:
      result = "kPrintSpacingNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPrintSpacingKind& elt)
{
  os << msrPrintSpacingKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
  msrOctaveKind            staffTuningOctaveKind)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      staffTuningQuarterTonesPitchKind,
      staffTuningOctaveKind);
  assert (o != nullptr);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
  msrOctaveKind            staffTuningOctaveKind)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber            = staffTuningLineNumber;
  fStaffTuningQuarterTonesPitchKind = staffTuningQuarterTonesPitchKind;
  fStaffTuningOctaveKind            = staffTuningOctaveKind;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningNewbornClone ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Creating a newborn clone of staff tuning '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

 S_msrStaffTuning
    newbornClone =
      msrStaffTuning::create (
        fInputLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuarterTonesPitchKind,
        fStaffTuningOctaveKind);

  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffTuning::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffTuning::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

std::string msrStaffTuning::asString () const
{
  std::stringstream s;

  s <<
    "[StaffTuning" <<
    ", fStaffTuningQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", fStaffTuningOctaveKind: " <<
    fStaffTuningOctaveKind <<
    ", line " << fStaffTuningLineNumber <<
    ']';

  return s.str ();
}

void msrStaffTuning::print (std::ostream& os) const
{
  os <<
    "[StaffTuning" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffTuningLineNumber" << ": " <<
    fStaffTuningLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffTuningQuarterTonesPitchKind" << ": " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffTuningOctaveKind" << ": " <<
    fStaffTuningOctaveKind <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrStaffTuning& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
{
  msrStaffDetails* o =
    new msrStaffDetails (
      inputLineNumber,
      upLinkToMeasure,
      staffTypeKind,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert (o != nullptr);
  return o;
}

S_msrStaffDetails msrStaffDetails::create (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
{
  return
    msrStaffDetails::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      staffTypeKind,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
}

msrStaffDetails::msrStaffDetails (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fStaffTypeKind    = staffTypeKind;

  fStaffLinesNumber = 5; // default value JMI ???

  fShowFretsKind    = showFretsKind;

  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails ()
{}

void msrStaffDetails::setStaffDetailsUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of staff details " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fStaffDetailsUpLinkToMeasure = measure;
}

void msrStaffDetails::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffDetails::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffDetails::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffTuningsList.size ()) {
    for (
      std::list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      ++i
  ) {
        // browse the staff tuning
        msrBrowser<msrStaffTuning> browser (v);
        browser.browse (*(*i));
    } // for
  }
}

std::string msrStaffDetails::asShortString () const
{
  std::stringstream s;

  s <<
    "[StaffDetails" <<
    ", fStaffTypeKind: " <<
    fStaffTypeKind <<
    ", line " << fInputLineNumber;

  // print the staff lines number
  s << ", fStaffLinesNumber: " << fStaffLinesNumber;

  // print the staff tunings if any
  s <<
    ", fStaffTuningsList: " << fStaffTuningsList.size ();

  s <<
    ", fShowFretsKind: " << fShowFretsKind <<
    ", fPrintObjectKind: " << fPrintObjectKind <<
    ", fPrintSpacingKind: " << fPrintSpacingKind <<
    ']';

  return s.str ();
}

void msrStaffDetails::print (std::ostream& os) const
{
  os <<
    "[StaffDetails" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "staffTypeKind" << ": " <<
    msrStaffTypeKindAsString (fStaffTypeKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "staffLinesNumber" << ": " << fStaffLinesNumber <<
    std::endl;

  // print the staff tunings if any
  if (fStaffTuningsList.size ()) {
    os << std::endl;

    std::list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << std::left <<
      std::setw (fieldWidth) <<
      "staffTunings" << ": " << "[EMPTY]" <<
      std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "showFretsKind" << ": " <<
    msrShowFretsKindAsString (fShowFretsKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "printObjectKind" << ": " <<
    msrPrintObjectKindAsString (fPrintObjectKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "printSpacingKind" << ": " <<
    msrPrintSpacingKindAsString (fPrintSpacingKind) <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrStaffDetails& elt)
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
