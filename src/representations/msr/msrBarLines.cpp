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

#include "msrMeasures.h"

#include "msrBarLines.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarLine msrBarLine::create (
  int                           inputLineNumber,
  S_msrMeasure                  upLinkToMeasure,
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
  assert (o != nullptr);
  return o;
}

msrBarLine::msrBarLine (
  int                           inputLineNumber,
  S_msrMeasure                  upLinkToMeasure,
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
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
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
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  return
    msrBarLine::create (
      inputLineNumber,
      upLinkToMeasure,
      kBarLineLocationRight,
      kBarLineStyleLightHeavy,
      kBarLineRepeatDirectionNone,
      kBarLineEndingTypeNone,
      "", // endingNumber
      0,  // barLineTimes
      kBarLineCategoryStandalone,
      kBarLineHasSegnoNo,
      kBarLineHasCodaNo,
      kBarLineRepeatWingedNone);
}

S_msrBarLine msrBarLine::createDoubleBarLine (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  return
    msrBarLine::create (
      inputLineNumber,
      upLinkToMeasure,
      kBarLineLocationRight,
      kBarLineStyleLightLight,
      kBarLineRepeatDirectionNone,
      kBarLineEndingTypeNone,
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
            msrBarLineCategoryKindAsString (barLineCategoryKind) <<
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
    fEndingTypeKind == kBarLineEndingTypeNone
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
    fEndingTypeKind == kBarLineEndingTypeNone
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

string msrBarLineLocationKindAsString (
  msrBarLineLocationKind barLineLocationKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineLocationKind& elt)
{
  os << msrBarLineLocationKindAsString (elt);
  return os;
}

string msrBarLineCategoryKindAsString (
  msrBarLineCategoryKind barLineCategoryKind)
{
  string result;

  switch (barLineCategoryKind) {
    case msrBarLineCategoryKind::k_NoBarLineCategory:
      result = "k_NoBarLineCategory";
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

ostream& operator << (ostream& os, const msrBarLineCategoryKind& elt)
{
  os << msrBarLineCategoryKindAsString (elt);
  return os;
}

string msrBarLineHasSegnoKindAsString (
  msrBarLineHasSegnoKind barLineHasSegnoKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineHasSegnoKind& elt)
{
  os << msrBarLineHasSegnoKindAsString (elt);
  return os;
}

string msrBarLineHasCodaKindAsString (
  msrBarLineHasCodaKind barLineHasCodaKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineHasCodaKind& elt)
{
  os << msrBarLineHasCodaKindAsString (elt);
  return os;
}

string msrBarLineStyleKindAsString (
  msrBarLineStyleKind barLineStyleKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineStyleKind& elt)
{
  os << msrBarLineStyleKindAsString (elt);
  return os;
}

string msrBarLineEndingTypeKindAsString (
  msrBarLineEndingTypeKind barLineEndingTypeKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineEndingTypeKind& elt)
{
  os << msrBarLineEndingTypeKindAsString (elt);
  return os;
}

string msrBarLineRepeatDirectionKindAsString (
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineRepeatDirectionKind& elt)
{
  os << msrBarLineRepeatDirectionKindAsString (elt);
  return os;
}

string msrBarLineRepeatWingedKindAsString (
  msrBarLineRepeatWingedKind barLineRepeatWingedKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrBarLineRepeatWingedKind& elt)
{
  os << msrBarLineRepeatWingedKindAsString (elt);
  return os;
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
    msrBarLineCategoryKindAsString (fBarLineCategoryKind) <<
    ", measureElementMeasureNumber: " << fetchMeasureElementMeasureNumber () <<
    ", fMeasureElementMeasurePosition " << fMeasureElementMeasurePosition <<

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
    endingNumbersListAsString () <<

    ", " <<
    msrBarLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    msrBarLineRepeatWingedKindAsString (fRepeatWingedKind) <<
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
    msrBarLineCategoryKindAsString (fBarLineCategoryKind) <<
    ", measureElementMeasureNumber: " << fetchMeasureElementMeasureNumber () <<
    ", fMeasureElementMeasurePosition: " << fMeasureElementMeasurePosition <<

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
    ", " <<
    endingNumbersListAsString () <<

    ", fBarLineTimes: \"" <<
    fBarLineTimes << "\" times" <<

    ", " <<
    msrBarLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    ", " <<
    msrBarLineHasCodaKindAsString (
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
    msrBarLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    ", measureElementMeasureNumber: " << fetchMeasureElementMeasureNumber () <<
    ", fMeasureElementMeasurePosition: " << fMeasureElementMeasurePosition <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "fLocationKind" << " : " <<
    msrBarLineLocationKindAsString (fLocationKind) <<
    endl <<
    setw (fieldWidth) <<
    "fStyleKind" << " : " <<
    msrBarLineStyleKindAsString (fStyleKind) <<
    endl <<

    setw (fieldWidth) <<
    "fRepeatDirectionKind" << " : " <<
    msrBarLineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    endl <<

    setw (fieldWidth) <<
    "fRepeatWingedKind" << " : " <<
    msrBarLineRepeatWingedKindAsString (fRepeatWingedKind) <<
    endl <<

    setw (fieldWidth) <<
    "fEndingTypeKind" << " : " <<
    msrBarLineEndingTypeKindAsString (fEndingTypeKind) <<
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
    msrBarLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    endl <<
*/

    setw (fieldWidth) <<
    "fBarLineHasSegnoKind" << " : " <<
    msrBarLineHasSegnoKindAsString (
      fBarLineHasSegnoKind) <<
    endl <<
    setw (fieldWidth) <<
    "fBarLineHasCodaKind" << " : " <<
    msrBarLineHasCodaKindAsString (
      fBarLineHasCodaKind) <<
    endl <<

    setw (fieldWidth) <<
    "endingNumbersList" << " : " <<
    endingNumbersListAsString () <<
    endl <<

    setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : " <<
    fetchMeasureElementMeasureNumber () <<
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
    msrBarLineCategoryKindAsString (
      fBarLineCategoryKind) <<
    ", measureElementMeasureNumber: " << fetchMeasureElementMeasureNumber () <<
    ", fMeasureElementMeasurePosition: " << fMeasureElementMeasurePosition <<
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
