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

#include "msrMidi.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrMidiTempo msrMidiTempo::create (
  int           inputLineNumber,
  const string& midiTempoDuration,
  int           midiTempoPerSecond)
{
  msrMidiTempo* o =
    new msrMidiTempo (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
  assert (o != nullptr);
  return o;
}

msrMidiTempo::msrMidiTempo (
  int           inputLineNumber,
  const string& midiTempoDuration,
  int           midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoDuration = midiTempoDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidiTempo::msrMidiTempo ()
    : msrElement (0) // input line number
{
  fMidiTempoDuration = "2";
  fMidiTempoPerSecond = 45;
}

msrMidiTempo::~msrMidiTempo ()
{}

S_msrMidiTempo msrMidiTempo::createMsrMidiTempoNewbornClone ()
{
  S_msrMidiTempo
    newbornClone =
      msrMidiTempo::create (
        fInputLineNumber,
        fMidiTempoDuration,
        fMidiTempoPerSecond);

  return newbornClone;
}

void msrMidiTempo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMidiTempo::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMidiTempo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMidiTempo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMidiTempo::browseData (basevisitor* v)
{}

string msrMidiTempo::asString () const
{
  stringstream s;

  s <<
    "[MidiTempo" <<
    ", midiTempoDuration = \"" << fMidiTempoDuration << "\"" <<
    ", midiTempoPerSecond = " << fMidiTempoPerSecond <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrMidiTempo::print (ostream& os) const
{
  os << "[MidiTempo" << endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "midiTempoDuration" << " = " << fMidiTempoDuration <<
    endl <<
    setw (fieldWidth) <<
    "midiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
    endl << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrMidiTempo& elt)
{
  elt->print (os);
  return os;
}


}
