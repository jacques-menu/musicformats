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

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "msrMeasures.h"

#include "msrInstruments.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrStringTuning msrStringTuning::create (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  msrOctaveKind        stringTuningOctaveKind)
{
  msrStringTuning* o =
    new msrStringTuning (
      inputLineNumber,
      stringTuningNumber,
      stringTuningDiatonicPitchKind,
      stringTuningAlterationKind,
      stringTuningOctaveKind);
  assert (o != nullptr);
  return o;
}

msrStringTuning::msrStringTuning (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  msrOctaveKind        stringTuningOctaveKind)
    : msrElement (inputLineNumber)
{
  fStringTuningNumber = stringTuningNumber;

  fStringTuningDiatonicPitchKind = stringTuningDiatonicPitchKind;
  fStringTuningAlterationKind    = stringTuningAlterationKind;
  fStringTuningOctaveKind        = stringTuningOctaveKind;
}

msrStringTuning::~msrStringTuning ()
{}

void msrStringTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStringTuning::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStringTuning::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStringTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStringTuning::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStringTuning::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStringTuning::browseData (basevisitor* v)
{}

void msrStringTuning::print (ostream& os) const
{
  ++gIndenter;

  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "stringTuningNumber" << " : " << fStringTuningNumber <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningDiatonicPitchKind" << " : " <<
    msrDiatonicPitchKindAsString (
      fStringTuningDiatonicPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningAlterationKind" << " : " <<
    msrAlterationKindAsString (
      fStringTuningAlterationKind) <<
    endl <<
    setw (fieldWidth) <<
    "stringTuningOctaveKind" << " : " <<
    msrOctaveKindAsString (fStringTuningOctaveKind) <<
    endl << endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrStringTuning& elt)
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
S_msrScordatura msrScordatura::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrScordatura* o =
    new msrScordatura (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

msrScordatura::msrScordatura (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{}

msrScordatura::~msrScordatura ()
{}

void msrScordatura::addStringTuningToScordatura (
  S_msrStringTuning stringTuning)
{
  fScordaturaStringTuningsList.push_back (
    stringTuning);
}

void msrScordatura::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScordatura::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScordatura::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScordatura::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScordatura::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScordatura::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScordatura::browseData (basevisitor* v)
{}

void msrScordatura::print (ostream& os) const
{
  ++gIndenter;

  os <<
    "Scordatura";

  if (fScordaturaStringTuningsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrStringTuning>::const_iterator
      iBegin = fScordaturaStringTuningsList.begin (),
      iEnd   = fScordaturaStringTuningsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : no string tunings" <<
      endl;
  }

  --gIndenter;
}

string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  string result;

  switch (syllableExtendKind) {
    case msrSyllableExtendKind::kSyllableExtendNone:
      result = "kSyllableExtendNone";
      break;
    case msrSyllableExtendKind::kSyllableExtendEmpty:
      result = "kSyllableExtendEmpty";
      break;
    case msrSyllableExtendKind::kSyllableExtendSingle:
      result = "kSyllableExtendSingle";
      break;
    case msrSyllableExtendKind::kSyllableExtendStart:
      result = "kSyllableExtendStart";
      break;
    case msrSyllableExtendKind::kSyllableExtendContinue:
      result = "kSyllableExtendContinue";
      break;
    case msrSyllableExtendKind::kSyllableExtendStop:
      result = "kSyllableExtendStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSyllableExtendKind& elt)
{
  os << msrSyllableExtendKindAsString (elt);
  return os;
}

ostream& operator << (ostream& os, const S_msrScordatura& elt)
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
S_msrAccordionRegistration msrAccordionRegistration::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  int          highDotsNumber,
  int          middleDotsNumber,
  int          lowDotsNumber)
{
  msrAccordionRegistration* o =
    new msrAccordionRegistration (
      inputLineNumber,
      upLinkToMeasure,
      highDotsNumber, middleDotsNumber, lowDotsNumber);
  assert (o != nullptr);
  return o;
}

msrAccordionRegistration::msrAccordionRegistration (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  int          highDotsNumber,
  int          middleDotsNumber,
  int          lowDotsNumber)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fHighDotsNumber   = highDotsNumber;
  fMiddleDotsNumber = middleDotsNumber;
  fLowDotsNumber    = lowDotsNumber;
}

msrAccordionRegistration::~msrAccordionRegistration ()
{}

void msrAccordionRegistration::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAccordionRegistration::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAccordionRegistration::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAccordionRegistration::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAccordionRegistration::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAccordionRegistration::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAccordionRegistration::browseData (basevisitor* v)
{}

string msrAccordionRegistration::asString () const
{
  stringstream s;

  s <<
    "AccordionRegistration" <<
    ", highDotsNumber: " << fHighDotsNumber <<
    ", middleDotsNumber: " << fMiddleDotsNumber <<
    ", lowDotsNumber: " << fLowDotsNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrAccordionRegistration::print (ostream& os) const
{
  ++gIndenter;

  os << asString () << endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrAccordionRegistration& elt)
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
S_msrHarpPedalsTuning msrHarpPedalsTuning::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrHarpPedalsTuning* o =
    new msrHarpPedalsTuning (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

msrHarpPedalsTuning::msrHarpPedalsTuning (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{}

msrHarpPedalsTuning::~msrHarpPedalsTuning ()
{}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif

  S_msrHarpPedalsTuning
    newbornClone = 0; // JMI

  return newbornClone;
}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningDeepClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif

  S_msrHarpPedalsTuning
    harpPedalsTuningDeepClone = 0; // JMI

  return harpPedalsTuningDeepClone;
}

void msrHarpPedalsTuning::addPedalTuning (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  // is diatonicPitch in the part renaming map?
  map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
    it =
      fHarpPedalsAlterationKindsMap.find (diatonicPitchKind);

  if (it != fHarpPedalsAlterationKindsMap.end ()) {
    stringstream s;

    s <<
      "pedal tuning '" <<
      msrDiatonicPitchKindAsString (
        diatonicPitchKind) <<
      msrAlterationKindAsString (
        alterationKind) <<
      "' has already been specified";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fHarpPedalsAlterationKindsMap [diatonicPitchKind] = alterationKind;
}

void msrHarpPedalsTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarpPedalsTuning::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarpPedalsTuning::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarpPedalsTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarpPedalsTuning::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarpPedalsTuning::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarpPedalsTuning::browseData (basevisitor* v)
{}

string msrHarpPedalsTuning::asString () const
{
  stringstream s;

  s <<
    "HarpPedalsTuning" <<
    ", line " << fInputLineNumber <<
    ", ";

  if (fHarpPedalsAlterationKindsMap.size ()) {
    ++gIndenter;

    map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the pedal and its alteration
      s <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        ' ' <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      s << ", ";
    } // for

    --gIndenter;
  }

  else {
    s <<
      "[EMPTY]";
  }

  return s.str ();
}

void msrHarpPedalsTuning::print (ostream& os) const
{
  os <<
    "[HarpPedalsTuning" <<
    ", line " << fInputLineNumber;

  if (fHarpPedalsAlterationKindsMap.size ()) {
    ++gIndenter;

    os << endl;

    map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the pedal and its alteration
      os <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        ' ' <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      os << ", ";
    } // for

    --gIndenter;
  }

  else {
    os <<
      " empty" <<
      endl;
  }

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrHarpPedalsTuning& elt)
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
S_msrPedal msrPedal::create (
  int              inputLineNumber,
  S_msrMeasure     upLinkToMeasure,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      upLinkToMeasure,
      pedalTypeKind, pedalLineKind, pedalSignKind);
  assert (o != nullptr);
  return o;
}

msrPedal::msrPedal (
  int              inputLineNumber,
  S_msrMeasure     upLinkToMeasure,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fPedalTypeKind = pedalTypeKind;
  fPedalLineKind = pedalLineKind;
  fPedalSignKind = pedalSignKind;
}

msrPedal::~msrPedal ()
{}

void msrPedal::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPedal::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPedal::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPedal::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPedal::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPedal::browseData (basevisitor* v)
{}

string msrPedalTypeKindAsString (
  msrPedalTypeKind pedalTypeKind)
{
  string result;

  switch (pedalTypeKind) {
    case k_NoPedalType:
      result = "k_NoPedalType";
      break;
    case kPedalTypeStart:
      result = "kPedalTypeStart";
      break;
    case kPedalTypeContinue:
      result = "kPedalTypeContinue";
      break;
    case kPedalTypeChange:
      result = "kPedalTypeChange";
      break;
    case kPedalTypeStop:
      result = "kPedalTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrPedalTypeKind& elt)
{
  os << msrPedalTypeKindAsString (elt);
  return os;
}

string msrPedalLineKindAsString (
  msrPedalLineKind pedalLineKind)
{
  string result;

  switch (pedalLineKind) {
    case msrPedalTypeKind::kPedalLineYes:
      result = "kPedalLineYes";
      break;
    case msrPedalTypeKind::kPedalLineNo:
      result = "kPedalLineNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrPedalLineKind& elt)
{
  os << msrPedalLineKindAsString (elt);
  return os;
}

string msrPedalSignKindAsString (
  msrPedalSignKind pedalSignKind)
{
  string result;

  switch (pedalSignKind) {
    case msrPedalTypeKind::kPedalSignYes:
      result = "kPedalSignYes";
      break;
    case msrPedalTypeKind::kPedalSignNo:
      result = "kPedalSignNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrPedalSignKind& elt)
{
  os << msrPedalSignKindAsString (elt);
  return os;
}

void msrPedal::print (ostream& os) const
{
  os <<
    "[Pedal" <<
    ", pedalType: " <<
    pedalTypeAsString () <<
    ", pedalLine: " <<
    pedalLineAsString () <<
    ", pedalSign: " <<
    pedalSignAsString () <<
    ", line " << fInputLineNumber <<
    ']' <<
    endl;
}

ostream& operator << (ostream& os, const S_msrPedal& elt)
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
S_msrDamp msrDamp::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrDamp* o =
    new msrDamp (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

msrDamp::msrDamp (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{}

msrDamp::~msrDamp ()
{}

void msrDamp::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDamp::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDamp::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDamp::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDamp::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDamp::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDamp::browseData (basevisitor* v)
{}

void msrDamp::print (ostream& os) const
{
  os <<
    "Damp" <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator << (ostream& os, const S_msrDamp& elt)
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
S_msrDampAll msrDampAll::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrDampAll* o =
    new msrDampAll (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

msrDampAll::msrDampAll (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{}

msrDampAll::~msrDampAll ()
{}

void msrDampAll::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDampAll::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDampAll::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDampAll::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDampAll::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDampAll::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDampAll::browseData (basevisitor* v)
{}

void msrDampAll::print (ostream& os) const
{
  os <<
    "DampAll" <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator << (ostream& os, const S_msrDampAll& elt)
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
