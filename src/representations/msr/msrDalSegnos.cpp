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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"

#include "msrDalSegnos.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrDalSegnoKindAsString (
  msrDalSegnoKind dalSegnoKind)
{
  std::string result;

  switch (dalSegnoKind) {
    case msrDalSegnoKind::kDalSegnoNone:
      result = "kDalSegnoNone";
      break;
    case msrDalSegnoKind::kDalSegno:
      result = "kDalSegno";
      break;
    case msrDalSegnoKind::kDalSegnoAlFine:
      result = "kDalSegnoAlFine";
      break;
    case msrDalSegnoKind::kDalSegnoAlCoda:
      result = "kDalSegnoAlCoda";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDalSegnoKind& elt)
{
  os << msrDalSegnoKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrDalSegno msrDalSegno::create (
  int             inputLineNumber,
  S_msrMeasure    upLinkToMeasure,
  msrDalSegnoKind dalSegnoKind,
  const std::string&   dalSegnoString,
  int             staffNumber)
{
  msrDalSegno* o =
    new msrDalSegno (
      inputLineNumber,
      upLinkToMeasure,
      dalSegnoKind,
      dalSegnoString,
      staffNumber);
  assert (o != nullptr);
  return o;
}

msrDalSegno::msrDalSegno (
  int             inputLineNumber,
  S_msrMeasure    upLinkToMeasure,
  msrDalSegnoKind dalSegnoKind,
  const std::string&   dalSegnoString,
  int             staffNumber)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{
  fDalSegnoKind = dalSegnoKind;

  fDalSegnoString = dalSegnoString;

  fStaffNumber = staffNumber;
}

msrDalSegno::~msrDalSegno ()
{}

void msrDalSegno::setDalSegnoMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const std::string& context)
{
  // set the dal segno position in measure

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting dal segno's position in measure of " << asString () <<
      " to '" <<
      measurePosition <<
      std::endl;
  }
#endif

  // set harmony's position in measure
  fMeasureElementMeasurePosition = measurePosition;

/* JMI
  if (false) { // JMI v0.9.66
  // compute dal segno's position in voice
  Rational
     voicePosition =
       segnoUpLinkToMeasure->getMeasureVoicePosition ()
        +
      measurePosition;

  // set dal segno's position in voice
  setMeasureElementVoicePosition (
    voicePosition,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fdal segnoNotesVector [0]->getMeasureElementSoundingWholeNotes ());
}
      */
}

void msrDalSegno::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDalSegno::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDalSegno::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrDalSegno::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDalSegno::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDalSegno::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrDalSegno::browseData (basevisitor* v)
{}

std::string msrDalSegno::asString () const
{
  std::stringstream s;

  s <<
    "[DalSegno" <<
    ", dalSegnoKind: " << msrDalSegnoKindAsString (fDalSegnoKind) <<
    ", dalSegnoString: \"" << fDalSegnoString << "\"" <<
    ", staffNumber: " << fStaffNumber <<
    ", measurePosition: " << fMeasureElementMeasurePosition <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrDalSegno::print (std::ostream& os) const
{
  os <<
    "[DalSegno" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "dalSegnoKind" << " : " << msrDalSegnoKindAsString (fDalSegnoKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "dalSegnoString" << " : \"" << fDalSegnoString << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "staffNumber" << " : " << fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "measurePosition" << " : " << fMeasureElementMeasurePosition <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "voicePosition" << " : " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "line" << " : " << fInputLineNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDalSegno& elt)
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
