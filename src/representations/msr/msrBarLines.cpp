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
#include "msrMeasureConstants.h"
#include "mfStringsHandling.h"

#include "msrBarLines.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarLine msrBarLine::create (
  int                           inputLineNumber,
  const S_msrMeasure&           upLinkToMeasure,
  msrBarLineLocationKind        barLineLocationKind,
  msrBarLineStyleKind           barLineStyleKind,
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
  msrBarLineEndingTypeKind      barLineEndingTypeKind,
  const std::string&            endingNumber,
  int                           barLineTimes,
  msrBarLineCategoryKind        barLineCategoryKind,
  msrBarLineHasSegnoKind        barLineHasSegnoKind,
  msrBarLineHasCodaKind         barLineHasCodaKind,
  msrBarLineRepeatWingedKind    barLineRepeatWingedKind)
{
  msrBarLine* obj =
    new msrBarLine (
      inputLineNumber,
      upLinkToMeasure,
      barLineLocationKind,
      barLineStyleKind,
      barLineRepeatDirectionKind,
      barLineEndingTypeKind,
      endingNumber,
      barLineTimes,
      barLineCategoryKind,
      barLineHasSegnoKind,
      barLineHasCodaKind,
      barLineRepeatWingedKind);
  assert (obj != nullptr);
  return obj;
}

S_msrBarLine msrBarLine::create (
  int                           inputLineNumber,
  msrBarLineLocationKind        barLineLocationKind,
  msrBarLineStyleKind           barLineStyleKind,
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
  msrBarLineEndingTypeKind      barLineEndingTypeKind,
  const std::string&            endingNumber,
  int                           barLineTimes,
  msrBarLineCategoryKind        barLineCategoryKind,
  msrBarLineHasSegnoKind        barLineHasSegnoKind,
  msrBarLineHasCodaKind         barLineHasCodaKind,
  msrBarLineRepeatWingedKind    barLineRepeatWingedKind)
{
  return
    msrBarLine::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      barLineLocationKind,
      barLineStyleKind,
      barLineRepeatDirectionKind,
      barLineEndingTypeKind,
      endingNumber,
      barLineTimes,
      barLineCategoryKind,
      barLineHasSegnoKind,
      barLineHasCodaKind,
      barLineRepeatWingedKind);
}

msrBarLine::msrBarLine (
  int                           inputLineNumber,
  const S_msrMeasure&           upLinkToMeasure,
  msrBarLineLocationKind        barLineLocationKind,
  msrBarLineStyleKind           barLineStyleKind,
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
  msrBarLineEndingTypeKind      barLineEndingTypeKind,
  const std::string&            endingNumber,
  int                           barLineTimes,
  msrBarLineCategoryKind        barLineCategoryKind,
  msrBarLineHasSegnoKind        barLineHasSegnoKind,
  msrBarLineHasCodaKind         barLineHasCodaKind,
  msrBarLineRepeatWingedKind    barLineRepeatWingedKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fLocationKind        = barLineLocationKind;

  fStyleKind           = barLineStyleKind;

  fRepeatDirectionKind = barLineRepeatDirectionKind;

  fEndingTypeKind      = barLineEndingTypeKind;
  fEndingNumber        = endingNumber;

  fBarLineTimes        = barLineTimes;

  fBarLineCategoryKind = barLineCategoryKind;

  fBarLineHasSegnoKind = barLineHasSegnoKind;
  fBarLineHasCodaKind  = barLineHasCodaKind;

  fRepeatWingedKind    = barLineRepeatWingedKind;

  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    mfExtractNumbersFromString (
      fEndingNumber, false); // 'true' to debug it
}

msrBarLine::~msrBarLine ()
{}

// void msrBarLine::setBarLineUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTraceBarLines ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of bar line " <<
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
//   fBarLineUpLinkToMeasure = measure;
// }

S_msrBarLine msrBarLine::createFinalBarLine (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  return
    msrBarLine::create (
      inputLineNumber,
      upLinkToMeasure,
      msrBarLineLocationKind::kBarLineLocationRight,
      msrBarLineStyleKind::kBarLineStyleLightHeavy,
      msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone,
      msrBarLineEndingTypeKind::kBarLineEndingTypeNone,
      "", // endingNumber
      0,  // barLineTimes
      msrBarLineCategoryKind::kBarLineCategoryStandalone,
      msrBarLineHasSegnoKind::kBarLineHasSegnoNo,
      msrBarLineHasCodaKind::kBarLineHasCodaNo,
      msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone);
}

S_msrBarLine msrBarLine::createDoubleBarLine (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  return
    msrBarLine::create (
      inputLineNumber,
      upLinkToMeasure,
      msrBarLineLocationKind::kBarLineLocationRight,
      msrBarLineStyleKind::kBarLineStyleLightLight,
      msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone,
      msrBarLineEndingTypeKind::kBarLineEndingTypeNone,
      "", // endingNumber
      0,  // barLineTimes
      msrBarLineCategoryKind::kBarLineCategoryStandalone,
      msrBarLineHasSegnoKind::kBarLineHasSegnoNo,
       msrBarLineHasCodaKind::kBarLineHasCodaNo,
      msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone);
}

void msrBarLine::setBarLineCategory (
  msrBarLineCategoryKind barLineCategoryKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Setting barLine category of " <<
      this->asString () <<
      " to " <<
      msrBarLineCategoryKindAsString (barLineCategoryKind);

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBarLineCategoryKind = barLineCategoryKind;
}

Bool msrBarLine::barLineIsAFinalBar () const
{
  return
    fLocationKind == msrBarLineLocationKind::kBarLineLocationRight
      &&
    fStyleKind == msrBarLineStyleKind::kBarLineStyleLightHeavy
      &&
    fRepeatDirectionKind == msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone
      &&
    fEndingTypeKind == msrBarLineEndingTypeKind::kBarLineEndingTypeNone
      &&
    fBarLineCategoryKind == msrBarLineCategoryKind::kBarLineCategoryStandalone;
}

Bool msrBarLine::barLineIsADoubleBar () const
{
  return
    fLocationKind == msrBarLineLocationKind::kBarLineLocationRight
      &&
    fStyleKind == msrBarLineStyleKind::kBarLineStyleLightLight
      &&
    fRepeatDirectionKind == msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone
      &&
    fEndingTypeKind == msrBarLineEndingTypeKind::kBarLineEndingTypeNone
      &&
    fBarLineCategoryKind == msrBarLineCategoryKind::kBarLineCategoryStandalone;
}

void msrBarLine::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarLine::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarLine::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBarLine::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarLine::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarLine::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBarLine::browseData (basevisitor* v)
{}

std::string msrBarLineLocationKindAsString (
  msrBarLineLocationKind barLineLocationKind)
{
  std::string result;

  switch (barLineLocationKind) {
    case msrBarLineLocationKind::kBarLineLocationNone:
      result = "kBarLineLocationNone";
      break;
    case msrBarLineLocationKind::kBarLineLocationLeft:
      result = "kBarLineLocationLeft";
      break;
    case msrBarLineLocationKind::kBarLineLocationMiddle:
      result = "kBarLineLocationMiddle";
      break;
    case msrBarLineLocationKind::kBarLineLocationRight:
      result = "kBarLineLocationRight";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineLocationKind& elt)
{
  os << msrBarLineLocationKindAsString (elt);
  return os;
}

std::string msrBarLineCategoryKindAsString (
  msrBarLineCategoryKind barLineCategoryKind)
{
  std::string result;

  switch (barLineCategoryKind) {
    case msrBarLineCategoryKind::kBarLineCategory_UNKNOWN_:
      result = "kBarLineCategory_UNKNOWN_";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryStandalone:
      result = "kBarLineCategoryStandalone";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryRepeatStart:
      result = "kBarLineCategoryRepeatStart";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryRepeatEnd:
      result = "kBarLineCategoryRepeatEnd";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingStart:
      result = "kBarLineCategoryHookedEndingStart";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingEnd:
      result = "kBarLineCategoryHookedEndingEnd";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingStart:
      result = "kBarLineCategoryHooklessEndingStart";
      break;
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingEnd:
      result = "kBarLineCategoryHooklessEndingEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineCategoryKind& elt)
{
  os << msrBarLineCategoryKindAsString (elt);
  return os;
}

std::string msrBarLineHasSegnoKindAsString (
  msrBarLineHasSegnoKind barLineHasSegnoKind)
{
  std::string result;

  switch (barLineHasSegnoKind) {
    case msrBarLineHasSegnoKind::kBarLineHasSegnoYes:
      result = "kBarLineHasSegnoYes";
      break;
    case msrBarLineHasSegnoKind::kBarLineHasSegnoNo:
      result = "kBarLineHasSegnoNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineHasSegnoKind& elt)
{
  os << msrBarLineHasSegnoKindAsString (elt);
  return os;
}

std::string msrBarLineHasCodaKindAsString (
  msrBarLineHasCodaKind barLineHasCodaKind)
{
  std::string result;

  switch (barLineHasCodaKind) {
    case msrBarLineHasCodaKind::kBarLineHasCodaYes:
      result = "kBarLineHasCodaYes";
      break;
    case msrBarLineHasCodaKind::kBarLineHasCodaNo:
      result = "kBarLineHasCodaNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineHasCodaKind& elt)
{
  os << msrBarLineHasCodaKindAsString (elt);
  return os;
}

std::string msrBarLineStyleKindAsString (
  msrBarLineStyleKind barLineStyleKind)
{
  std::string result;

  switch (barLineStyleKind) {
    case msrBarLineStyleKind::kBarLineStyleNone:
      result = "kBarLineStyleNone";
      break;
    case msrBarLineStyleKind::kBarLineStyleRegular:
      result = "kBarLineStyleRegular";
      break;
    case msrBarLineStyleKind::kBarLineStyleDotted:
      result = "kBarLineStyleDotted";
      break;
    case msrBarLineStyleKind::kBarLineStyleDashed:
      result = "kBarLineStyleDashed";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavy:
      result = "kBarLineStyleHeavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleLightLight:
      result = "kBarLineStyleLightLight";
      break;
    case msrBarLineStyleKind::kBarLineStyleLightHeavy:
      result = "kBarLineStyleLightHeavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyLight:
      result = "kBarLineStyleHeavyLight";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
      result = "kBarLineStyleHeavyHeavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleTick:
      result = "kBarLineStyleTick";
      break;
    case msrBarLineStyleKind::kBarLineStyleShort:
      result = "kBarLineStyleShort";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineStyleKind& elt)
{
  os << msrBarLineStyleKindAsString (elt);
  return os;
}

std::string msrBarLineEndingTypeKindAsString (
  msrBarLineEndingTypeKind barLineEndingTypeKind)
{
  std::string result;

  switch (barLineEndingTypeKind) {
    case msrBarLineEndingTypeKind::kBarLineEndingTypeNone:
      result = "kBarLineEndingTypeNone";
      break;
    case msrBarLineEndingTypeKind::kBarLineEndingTypeStart:
      result = "kBarLineEndingTypeStart";
      break;
    case msrBarLineEndingTypeKind::kBarLineEndingTypeStop:
      result = "kBarLineEndingTypeStop";
      break;
    case msrBarLineEndingTypeKind::kBarLineEndingTypeDiscontinue:
      result = "kBarLineEndingTypeDiscontinue";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineEndingTypeKind& elt)
{
  os << msrBarLineEndingTypeKindAsString (elt);
  return os;
}

std::string msrBarLineRepeatDirectionKindAsString (
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind)
{
  std::string result;

  switch (barLineRepeatDirectionKind) {
    case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone:
      result ="barLineRepeatDirectionNone";
      break;
    case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionForward:
      result ="barLineRepeatDirectionForward";
      break;
    case msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionBackward:
      result ="barLineRepeatDirectionBackward";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineRepeatDirectionKind& elt)
{
  os << msrBarLineRepeatDirectionKindAsString (elt);
  return os;
}

std::string msrBarLineRepeatWingedKindAsString (
  msrBarLineRepeatWingedKind barLineRepeatWingedKind)
{
  std::string result;

  switch (barLineRepeatWingedKind) {
    case msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone:
      result = "kBarLineRepeatWingedNone";
      break;
    case msrBarLineRepeatWingedKind::kBarLineRepeatWingedStraight:
      result = "kBarLineRepeatWingedStraight";
      break;
    case msrBarLineRepeatWingedKind::kBarLineRepeatWingedCurved:
      result = "kBarLineRepeatWingedCurved";
      break;
    case msrBarLineRepeatWingedKind::kBarLineRepeatWingedDoubleStraight:
      result = "kBarLineRepeatWingedDoubleStraight";
      break;
    case msrBarLineRepeatWingedKind::kBarLineRepeatWingedDoubleCurved:
      result = "kBarLineRepeatWingedDoubleCurved";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarLineRepeatWingedKind& elt)
{
  os << msrBarLineRepeatWingedKindAsString (elt);
  return os;
}

std::string msrBarLine::endingNumbersListAsString () const
{
  std::stringstream ss;

  if (fEndingNumbersList.size ()) {
    std::list <int>::const_iterator
      iBegin = fEndingNumbersList.begin (),
      iEnd   = fEndingNumbersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  return ss.str ();
}

std::string msrBarLine::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[BarLine, " <<
    fBarLineCategoryKind <<
    ", measureNumber: ";
  if (fMeasureElementUpLinkToMeasure) {
    ss <<
      fMeasureElementUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    ss << "[UNKNOWN]";
  }

  ss <<
    ", fMeasureElementMeasurePosition " <<
    fMeasureElementMeasurePosition.asString () <<

/* JMI
    ", " <<
    msrBarLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    ", " <<
    msrBarLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<

    ", " <<
    msrBarLineLocationKindAsString (fLocationKind) <<
    ", " <<
    msrBarLineStyleKindAsString (fStyleKind) <<
    ", " <<
    msrBarLineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    ", endingNumbersList: " <<
    endingNumbersListAsString () <<

    ", " <<
    msrBarLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    msrBarLineRepeatWingedKindAsString (fRepeatWingedKind) <<
    */
    ", " <<
    ", fBarLineTimes: \"" <<
    fBarLineTimes << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrBarLine::asString () const
{
  std::stringstream ss;

  ss <<
    "[BarLine, " <<
    fBarLineCategoryKind <<
    ", measureElementMeasureNumber: ";

    if (fMeasureElementUpLinkToMeasure) {
      ss <<
        fMeasureElementUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      ss << "[UNKNOWN]";
    }

  ss <<
    ", fMeasureElementMeasurePosition: " << fMeasureElementMeasurePosition.asString () <<

    ", " <<
    msrBarLineLocationKindAsString (fLocationKind) <<
    ", " <<
    msrBarLineStyleKindAsString (fStyleKind) <<

    ", " <<
    msrBarLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<

    ", " <<
    msrBarLineRepeatWingedKindAsString (fRepeatWingedKind) <<

    ", " <<
    msrBarLineEndingTypeKindAsString (fEndingTypeKind) <<
    ", endingNumbersList: " <<
    endingNumbersListAsString () <<

    ", fBarLineTimes: \"" <<
    fBarLineTimes << "\"" <<

    ", " <<
    msrBarLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    ", " <<
    msrBarLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<

    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrBarLine::printFull (std::ostream& os) const
{
  os <<
    "[BarLine" <<
    ", fBarLineCategoryKind: " <<
    fBarLineCategoryKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 33;

  os << std::left <<
    "fMeasureElementUpLinkToMeasure" << ": ";
    if (fMeasureElementUpLinkToMeasure) {
      os <<  std::endl;
      ++gIndenter;
      os <<
        fMeasureElementUpLinkToMeasure->asShortString ();
      --gIndenter;
    }
    else {
      os << "[NULL]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << ": " <<
    fMeasureElementMeasurePosition.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLocationKind" << ": " <<
    fLocationKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStyleKind" << ": " <<
    fStyleKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fRepeatDirectionKind" << ": " <<
    fRepeatDirectionKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fRepeatWingedKind" << ": " <<
    fRepeatWingedKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEndingTypeKind" << ": " <<
    fEndingTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEndingNumber" << ": " <<
    fEndingNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBarLineTimes" << ": " <<
    fBarLineTimes <<
    std::endl <<

/* JMI
    std::setw (fieldWidth) <<
    "fBarLineCategoryKind" << ": " <<
    fBarLineCategoryKind <<
    std::endl <<
*/

    std::setw (fieldWidth) <<
    "fBarLineHasSegnoKind" << ": " <<
    fBarLineHasSegnoKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fBarLineHasCodaKind" << ": " <<
    fBarLineHasCodaKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "endingNumbersList" << ": " <<
    endingNumbersListAsString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "barLineUpLinkToMeasureMeasureNumber" << ": ";

    if (fMeasureElementUpLinkToMeasure) {
      os <<  std::endl;
      ++gIndenter;
      os <<
        fMeasureElementUpLinkToMeasure->getMeasureNumber ();
      --gIndenter;
    }
    else {
      os << "[NULL]" << std::endl;
    }

  --gIndenter;

  os <<
    ']' <<
    std::endl;
}

void msrBarLine::print (std::ostream& os) const
{
  os <<
    "[BarLine" <<
    ", fBarLineCategoryKind: " << fBarLineCategoryKind <<
    ", fStyleKind: " << fStyleKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 31;
  os <<
    std::setw (fieldWidth) <<
   "measureElementMeasureNumber: ";

  if (fMeasureElementUpLinkToMeasure) {
    os <<
      fMeasureElementUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    os << "[UNKNOWN]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" <<
    fMeasureElementMeasurePosition.asString () <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBarLine& elt)
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
