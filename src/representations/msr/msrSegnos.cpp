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

#include "oahOah.h"

#include "msrOah.h"

#include "msrSegnos.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrSegno msrSegno::create (
  int inputLineNumber,
  int staffNumber)
{
  msrSegno* o =
    new msrSegno (
      inputLineNumber,
      staffNumber);
  assert (o != nullptr);
  return o;
}

msrSegno::msrSegno (
  int inputLineNumber,
  int staffNumber)
    : msrMeasureElement (inputLineNumber)
{
  fStaffNumber = staffNumber;
}

msrSegno::~msrSegno ()
{}

void msrSegno::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegno::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegno::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegno::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegno::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

string msrSegno::asString () const
{
  stringstream s;

  s <<
    "Segno" <<
    ", staffNumber :" << fStaffNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrSegno::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrSegno& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
string dalSegnoKindAsString (
  msrDalSegnoKind dalSegnoKind)
{
  string result;

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

ostream& operator<< (ostream& os, const msrDalSegnoKind& elt)
{
  os << dalSegnoKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrDalSegno msrDalSegno::create (
  int             inputLineNumber,
  msrDalSegnoKind dalSegnoKind,
  const string&   dalSegnoString,
  int             staffNumber)
{
  msrDalSegno* o =
    new msrDalSegno (
      inputLineNumber,
      dalSegnoKind,
      dalSegnoString,
      staffNumber);
  assert (o != nullptr);
  return o;
}

msrDalSegno::msrDalSegno (
  int             inputLineNumber,
  msrDalSegnoKind dalSegnoKind,
  const string&   dalSegnoString,
  int             staffNumber)
    : msrMeasureElement (inputLineNumber)
{
  fDalSegnoKind = dalSegnoKind;

  fDalSegnoString = dalSegnoString;

  fStaffNumber = staffNumber;
}

msrDalSegno::~msrDalSegno ()
{}

void msrDalSegno::setDalSegnoPositionInMeasure (
  const rational& positionInMeasure)
{
  // set the dal segno position in measure

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting dal segno position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      endl;
  }
#endif

  string context =
    "setDalSegnoPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

/* JMI
  if (false) { // JMI
  // compute dal segno's position in voice
  rational
     positionInVoice =
      fdal segnoMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;
  positionInVoice.rationalise ();

  // set dal segno's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fdal segnoNotesVector [0]->getNoteSoundingWholeNotes ());
}
      */
}

void msrDalSegno::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDalSegno::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDalSegno::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDalSegno::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDalSegno::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDalSegno>*
    p =
      dynamic_cast<visitor<S_msrDalSegno>*> (v)) {
        S_msrDalSegno elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDalSegno::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDalSegno::browseData (basevisitor* v)
{}

string msrDalSegno::asString () const
{
  stringstream s;

  s <<
    "[DalSegno" <<
    ", dalSegnoKind: " << dalSegnoKindAsString (fDalSegnoKind) <<
    ", dalSegnoString: \"" << fDalSegnoString << "\"" <<
    ", staffNumber: " << fStaffNumber <<
    ", positionInMeasure: " << fMeasureElementPositionInMeasure <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrDalSegno::print (ostream& os) const
{
  os <<
    "[DalSegno" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "dalSegnoKind" << " : " << dalSegnoKindAsString (fDalSegnoKind) <<
    endl <<
    setw (fieldWidth) <<
    "dalSegnoString" << " : \"" << fDalSegnoString << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "staffNumber" << " : " << fStaffNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) <<
    "line" << " : " << fInputLineNumber <<
    endl;

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_msrDalSegno& elt)
{
  elt->print (os);
  return os;
}


}
