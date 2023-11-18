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

#include "msrPitchesNames.h"

#include "msrStavesDetails.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


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
  msrStaffTuning* obj =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      staffTuningQuarterTonesPitchKind,
      staffTuningOctaveKind);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of staff tuning '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 S_msrStaffTuning
    newbornClone =
      msrStaffTuning::create (
        fInputStartLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuarterTonesPitchKind,
        fStaffTuningOctaveKind);

  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffTuning::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffTuning::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffTuning::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffTuning::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

std::string msrStaffTuning::asString () const
{
  std::stringstream ss;

  ss <<
    "[StaffTuning" <<
    ", fStaffTuningQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", fStaffTuningOctaveKind: " <<
    fStaffTuningOctaveKind <<
    ", line " << fStaffTuningLineNumber <<
    ']';

  return ss.str ();
}

void msrStaffTuning::print (std::ostream& os) const
{
  os <<
    "[StaffTuning" <<
    ", line " << fInputStartLineNumber <<
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
      gMsrOahGroup->
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
    os << "[NULL]" << std::endl;
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
  msrStaffDetails* obj =
    new msrStaffDetails (
      inputLineNumber,
      upLinkToMeasure,
      staffTypeKind,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert (obj != nullptr);
  return obj;
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
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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

// void msrStaffDetails::setStaffDetailsUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTraceStaves ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of staff details " <<
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
//   fStaffDetailsUpLinkToMeasure = measure;
// }

void msrStaffDetails::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffDetails::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffDetails::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffDetails::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffDetails::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "[StaffDetails" <<
    ", fStaffTypeKind: " <<
    fStaffTypeKind <<
    ", line " << fInputStartLineNumber;

  // print the staff lines number
  ss << ", fStaffLinesNumber: " << fStaffLinesNumber;

  // print the staff tunings if any
  ss <<
    ", fStaffTuningsList: " << fStaffTuningsList.size ();

  ss <<
    ", fShowFretsKind: " << fShowFretsKind <<
    ", fPrintObjectKind: " << fPrintObjectKind <<
    ", fPrintSpacingKind: " << fPrintSpacingKind <<
    ']';

  return ss.str ();
}

void msrStaffDetails::print (std::ostream& os) const
{
  os <<
    "[StaffDetails" <<
    ", line " << fInputStartLineNumber <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
