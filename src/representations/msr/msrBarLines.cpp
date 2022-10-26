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

#include "mfStringsHandling.h"

#include "msrBarLines.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarLine msrBarLine::create (
  int                           inputLineNumber,
  msrBarLineLocationKind        barLineLocationKind,
  msrBarLineStyleKind           barLineStyleKind,
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
  msrBarLineEndingTypeKind      barLineEndingTypeKind,
  const string&                 endingNumber,
  int                           barLineTimes,
  msrBarLineCategoryKind        barLineCategoryKind,
  msrBarLineHasSegnoKind        barLineHasSegnoKind,
  msrBarLineHasCodaKind         barLineHasCodaKind,
  msrBarLineRepeatWingedKind    barLineRepeatWingedKind)
{
  msrBarLine* o =
    new msrBarLine (
      inputLineNumber,
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
  assert (o != nullptr);
  return o;
}

msrBarLine::msrBarLine (
  int                           inputLineNumber,
  msrBarLineLocationKind        barLineLocationKind,
  msrBarLineStyleKind           barLineStyleKind,
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
  msrBarLineEndingTypeKind      barLineEndingTypeKind,
  const string&                 endingNumber,
  int                           barLineTimes,
  msrBarLineCategoryKind        barLineCategoryKind,
  msrBarLineHasSegnoKind        barLineHasSegnoKind,
  msrBarLineHasCodaKind         barLineHasCodaKind,
  msrBarLineRepeatWingedKind    barLineRepeatWingedKind)
    : msrMeasureElement (inputLineNumber)
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

S_msrBarLine msrBarLine::createFinalBarLine (
int inputLineNumber)
{
  return
    msrBarLine::create (
      inputLineNumber,
      kBarLineLocationRight,
      kBarLineStyleLightHeavy,
      kBarLineRepeatDirectionNone,
      kBarLineEndingNone,
      "", // endingNumber
      0,  // barLineTimes
      kBarLineCategoryStandalone,
      kBarLineHasSegnoNo,
      kBarLineHasCodaNo,
      kBarLineRepeatWingedNone);
}

S_msrBarLine msrBarLine::createDoubleBarLine (
int inputLineNumber)
{
  return
    msrBarLine::create (
      inputLineNumber,
      kBarLineLocationRight,
      kBarLineStyleLightLight,
      kBarLineRepeatDirectionNone,
      kBarLineEndingNone,
      "", // endingNumber
      0,  // barLineTimes
      kBarLineCategoryStandalone,
      kBarLineHasSegnoNo,
      kBarLineHasCodaNo,
      kBarLineRepeatWingedNone);
}

void msrBarLine::setBarLineCategory (
  msrBarLineCategoryKind barLineCategoryKind)
{
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceBarLines ()) {
          gLogStream <<
            "Setting barLine category of " <<
            this->asString () <<
            " to '" <<
            barLineCategoryKindAsString (barLineCategoryKind) <<
            "'" <<
            endl;
        }
#endif

  fBarLineCategoryKind = barLineCategoryKind;
}

Bool msrBarLine::barLineIsAFinalBar () const
{
  return
    fLocationKind == kBarLineLocationRight
      &&
    fStyleKind == kBarLineStyleLightHeavy
      &&
    fRepeatDirectionKind == kBarLineRepeatDirectionNone
      &&
    fEndingTypeKind == kBarLineEndingNone
      &&
    fBarLineCategoryKind == kBarLineCategoryStandalone;
}

Bool msrBarLine::barLineIsADoubleBar () const
{
  return
    fLocationKind == kBarLineLocationRight
      &&
    fStyleKind == kBarLineStyleLightLight
      &&
    fRepeatDirectionKind == kBarLineRepeatDirectionNone
      &&
    fEndingTypeKind == kBarLineEndingNone
      &&
    fBarLineCategoryKind == kBarLineCategoryStandalone;
}

void msrBarLine::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarLine::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarLine::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarLine::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarLine::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarLine>*
    p =
      dynamic_cast<visitor<S_msrBarLine>*> (v)) {
        S_msrBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarLine::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarLine::browseData (basevisitor* v)
{}

string msrBarLine::barLineLocationKindAsString (
  msrBarLineLocationKind barLineLocationKind)
{
  string result;

  switch (barLineLocationKind) {
    case msrBarLine::kBarLineLocationNone:
      result = "kBarLineLocationNone";
      break;
    case msrBarLine::kBarLineLocationLeft:
      result = "kBarLineLocationLeft";
      break;
    case msrBarLine::kBarLineLocationMiddle:
      result = "kBarLineLocationMiddle";
      break;
    case msrBarLine::kBarLineLocationRight:
      result = "kBarLineLocationRight";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineCategoryKindAsString (
  msrBarLineCategoryKind barLineCategoryKind)
{
  string result;

  switch (barLineCategoryKind) {
    case msrBarLine::k_NoBarLineCategory:
      result = "k_NoBarLineCategory";
      break;
    case msrBarLine::kBarLineCategoryStandalone:
      result = "kBarLineCategoryStandalone";
      break;
    case msrBarLine::kBarLineCategoryRepeatStart:
      result = "kBarLineCategoryRepeatStart";
      break;
    case msrBarLine::kBarLineCategoryRepeatEnd:
      result = "kBarLineCategoryRepeatEnd";
      break;
    case msrBarLine::kBarLineCategoryHookedEndingStart:
      result = "kBarLineCategoryHookedEndingStart";
      break;
    case msrBarLine::kBarLineCategoryHookedEndingEnd:
      result = "kBarLineCategoryHookedEndingEnd";
      break;
    case msrBarLine::kBarLineCategoryHooklessEndingStart:
      result = "kBarLineCategoryHooklessEndingStart";
      break;
    case msrBarLine::kBarLineCategoryHooklessEndingEnd:
      result = "kBarLineCategoryHooklessEndingEnd";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineHasSegnoKindAsString (
  msrBarLineHasSegnoKind barLineHasSegnoKind)
{
  string result;

  switch (barLineHasSegnoKind) {
    case msrBarLine::kBarLineHasSegnoYes:
      result = "kBarLineHasSegnoYes";
      break;
    case msrBarLine::kBarLineHasSegnoNo:
      result = "kBarLineHasSegnoNo";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineHasCodaKindAsString (
  msrBarLineHasCodaKind barLineHasCodaKind)
{
  string result;

  switch (barLineHasCodaKind) {
    case msrBarLine::kBarLineHasCodaYes:
      result = "kBarLineHasCodaYes";
      break;
    case msrBarLine::kBarLineHasCodaNo:
      result = "kBarLineHasCodaNo";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineStyleKindAsString (
  msrBarLineStyleKind barLineStyleKind)
{
  string result;

  switch (barLineStyleKind) {
    case msrBarLine::kBarLineStyleNone:
      result = "kBarLineStyleNone";
      break;
    case msrBarLine::kBarLineStyleRegular:
      result = "kBarLineStyleRegular";
      break;
    case msrBarLine::kBarLineStyleDotted:
      result = "kBarLineStyleDotted";
      break;
    case msrBarLine::kBarLineStyleDashed:
      result = "kBarLineStyleDashed";
      break;
    case msrBarLine::kBarLineStyleHeavy:
      result = "kBarLineStyleHeavy";
      break;
    case msrBarLine::kBarLineStyleLightLight:
      result = "kBarLineStyleLightLight";
      break;
    case msrBarLine::kBarLineStyleLightHeavy:
      result = "kBarLineStyleLightHeavy";
      break;
    case msrBarLine::kBarLineStyleHeavyLight:
      result = "kBarLineStyleHeavyLight";
      break;
    case msrBarLine::kBarLineStyleHeavyHeavy:
      result = "kBarLineStyleHeavyHeavy";
      break;
    case msrBarLine::kBarLineStyleTick:
      result = "kBarLineStyleTick";
      break;
    case msrBarLine::kBarLineStyleShort:
      result = "kBarLineStyleShort";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineEndingTypeKindAsString (
  msrBarLineEndingTypeKind barLineEndingTypeKind)
{
  string result;

  switch (barLineEndingTypeKind) {
    case msrBarLine::kBarLineEndingNone:
      result = "kBarLineEndingNone";
      break;
    case msrBarLine::kBarLineEndingTypeStart:
      result = "kBarLineEndingTypeStart";
      break;
    case msrBarLine::kBarLineEndingTypeStop:
      result = "kBarLineEndingTypeStop";
      break;
    case msrBarLine::kBarLineEndingTypeDiscontinue:
      result = "kBarLineEndingTypeDiscontinue";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineRepeatDirectionKindAsString (
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind)
{
  string result;

  switch (barLineRepeatDirectionKind) {
    case msrBarLine::kBarLineRepeatDirectionNone:
      result ="barLineRepeatDirectionNone";
      break;
    case msrBarLine::kBarLineRepeatDirectionForward:
      result ="barLineRepeatDirectionForward";
      break;
    case msrBarLine::kBarLineRepeatDirectionBackward:
      result ="barLineRepeatDirectionBackward";
      break;
  } // switch

  return result;
}

string msrBarLine::barLineRepeatWingedKindAsString (
  msrBarLineRepeatWingedKind barLineRepeatWingedKind)
{
  string result;

  switch (barLineRepeatWingedKind) {
    case msrBarLine::kBarLineRepeatWingedNone:
      result = "kBarLineRepeatWingedNone";
      break;
    case msrBarLine::kBarLineRepeatWingedStraight:
      result = "kBarLineRepeatWingedStraight";
      break;
    case msrBarLine::kBarLineRepeatWingedCurved:
      result = "kBarLineRepeatWingedCurved";
      break;
    case msrBarLine::kBarLineRepeatWingedDoubleStraight:
      result = "kBarLineRepeatWingedDoubleStraight";
      break;
    case msrBarLine::kBarLineRepeatWingedDoubleCurved:
      result = "kBarLineRepeatWingedDoubleCurved";
      break;
  } // switch

  return result;
}

string msrBarLine::endingNumbersListAsString () const
{
  stringstream s;

  if (fEndingNumbersList.size ()) {
    list<int>::const_iterator
      iBegin = fEndingNumbersList.begin (),
      iEnd   = fEndingNumbersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  return s.str ();
}

string msrBarLine::asShortString () const
{
  stringstream s;

  s <<
    "[BarLine " <<
    barLineCategoryKindAsString (fBarLineCategoryKind) <<
    ", measureNumber " << fMeasureElementMeasureNumber <<
    ", positionInMeasure " << fMeasureElementPositionInMeasure <<

/* JMI
    ", " <<
    barLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    ", " <<
    barLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<

    ", " <<
    barLineLocationKindAsString (fLocationKind) <<
    ", " <<
    barLineStyleKindAsString (fStyleKind) <<
    ", " <<
    barLineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    endingNumbersListAsString () <<

    ", " <<
    barLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    barLineRepeatWingedKindAsString (fRepeatWingedKind) <<
    */
    ", " <<
    fBarLineTimes << " times" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrBarLine::asString () const
{
  stringstream s;

  s <<
    "[BarLine " <<
    barLineCategoryKindAsString (fBarLineCategoryKind) <<
    ", measureNumber " << fMeasureElementMeasureNumber <<
    ", positionInMeasure " << fMeasureElementPositionInMeasure <<

    ", " <<
    barLineLocationKindAsString (fLocationKind) <<
    ", " <<
    barLineStyleKindAsString (fStyleKind) <<

    ", " <<
    barLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<

    ", " <<
    barLineRepeatWingedKindAsString (fRepeatWingedKind) <<

    ", " <<
    barLineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    endingNumbersListAsString () <<

    ", barLineTimes: \"" <<
    fBarLineTimes << "\" times" <<

    ", " <<
    barLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    ", " <<
    barLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<

    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBarLine::print (ostream& os) const
{
  os <<
    "[BarLine" <<
    ", fBarLineCategoryKind: " <<
    barLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    ", fMeasureElementMeasureNumber: " << fMeasureElementMeasureNumber <<
    ", fMeasureElementPositionInMeasure: " << fMeasureElementPositionInMeasure <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "fLocationKind" << " : " <<
    barLineLocationKindAsString (fLocationKind) <<
    endl <<
    setw (fieldWidth) <<
    "fStyleKind" << " : " <<
    barLineStyleKindAsString (fStyleKind) <<
    endl <<

    setw (fieldWidth) <<
    "fRepeatDirectionKind" << " : " <<
    barLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    endl <<

    setw (fieldWidth) <<
    "fRepeatWingedKind" << " : " <<
    barLineRepeatWingedKindAsString (fRepeatWingedKind) <<
    endl <<

    setw (fieldWidth) <<
    "fEndingTypeKind" << " : " <<
    barLineEndingTypeKindAsString (fEndingTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "fEndingNumber" << " : " <<
    fEndingNumber <<
    endl <<

    setw (fieldWidth) <<
    "fBarLineTimes" << " : " <<
    fBarLineTimes <<
    endl <<

/* JMI
    setw (fieldWidth) <<
    "fBarLineCategoryKind" << " : " <<
    barLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    endl <<
*/

    setw (fieldWidth) <<
    "fBarLineHasSegnoKind" << " : " <<
    barLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    endl <<
    setw (fieldWidth) <<
    "fBarLineHasCodaKind" << " : " <<
    barLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<
    endl <<

    setw (fieldWidth) <<
    "endingNumbersList" << " : " <<
    endingNumbersListAsString () <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureElementMeasureNumber" << " : " <<
    fMeasureElementMeasureNumber <<
    endl;

  --gIndenter;

  os <<
    ']' <<
    endl;
}

void msrBarLine::printShort (ostream& os) const
{
  os <<
    "[BarLine" <<
    ", fBarLineCategoryKind: " << fBarLineCategoryKind <<
    barLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    ", fMeasureElementMeasureNumber: " << fMeasureElementMeasureNumber <<
    ", fMeasureElementPositionInMeasure: " << fMeasureElementPositionInMeasure <<
    ", line " << fInputLineNumber <<
    ']' <<
    endl;
}

ostream& operator << (ostream& os, const S_msrBarLine& elt)
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
