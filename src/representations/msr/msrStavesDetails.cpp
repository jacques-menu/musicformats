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

#include "msrPitchesNames.h"

#include "msrMeasures.h"

#include "msrStavesDetails.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Creating a newborn clone of staff tuning '" <<
      asString () <<
      "'" <<
      endl;
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
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffTuning::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

string msrStaffTuning::asString () const
{
  stringstream s;

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

void msrStaffTuning::print (ostream& os) const
{
  os <<
    "[StaffTuning" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "fStaffTuningLineNumber" << " : " <<
    fStaffTuningLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "fStaffTuningQuarterTonesPitchKind" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "fStaffTuningOctaveKind" << " : " <<
    fStaffTuningOctaveKind <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrStaffTuning& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                 inputLineNumber,
  S_msrMeasure        upLinkToMeasure,
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

msrStaffDetails::msrStaffDetails (
  int                 inputLineNumber,
  S_msrMeasure        upLinkToMeasure,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fStaffTypeKind    = staffTypeKind;

  fStaffLinesNumber = 5; // default value JMI ???

  fShowFretsKind    = showFretsKind;

  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails ()
{}

void msrStaffDetails::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffDetails::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffDetails::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      ++i
  ) {
        // browse the staff tuning
        msrBrowser<msrStaffTuning> browser (v);
        browser.browse (*(*i));
    } // for
  }
}

string msrStaffDetails::msrStaffTypeKindAsString (
  msrStaffTypeKind staffTypeKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrStaffTypeKind& elt)
{
  os << msrStaffTypeKindAsString (elt);
  return os;
}

string msrStaffDetails::msrShowFretsKindAsString (
  msrShowFretsKind showFretsKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrShowFretsKind& elt)
{
  os << msrShowFretsKindAsString (elt);
  return os;
}

string msrStaffDetails::msrPrintSpacingKindAsString (
  msrPrintSpacingKind printSpacingKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrPrintSpacingKind& elt)
{
  os << msrPrintSpacingKindAsString (elt);
  return os;
}

string msrStaffDetails::asShortString () const
{
  stringstream s;

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

void msrStaffDetails::print (ostream& os) const
{
  os <<
    "[StaffDetails" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "staffTypeKind" << " : " <<
    msrStaffTypeKindAsString (fStaffTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "staffLinesNumber" << " : " << fStaffLinesNumber <<
    endl;

  // print the staff tunings if any
  if (fStaffTuningsList.size ()) {
    os << endl;

    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << left <<
      setw (fieldWidth) <<
      "staffTunings" << " : " << "[EMPTY]" <<
      endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "showFretsKind" << " : " <<
    msrShowFretsKindAsString (fShowFretsKind) <<
    endl <<
    setw (fieldWidth) <<
    "printObjectKind" << " : " <<
    msrPrintObjectKindAsString (fPrintObjectKind) <<
    endl <<
    setw (fieldWidth) <<
    "printSpacingKind" << " : " <<
    msrPrintSpacingKindAsString (fPrintSpacingKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrStaffDetails& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
