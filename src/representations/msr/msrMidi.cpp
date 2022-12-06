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

#include "msrMidi.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMidiTempo msrMidiTempo::create (
  int           inputLineNumber,
  const std::string& midiTempoDuration,
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
  const std::string& midiTempoDuration,
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
      std::endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMidiTempo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMidiTempo::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMidiTempo::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMidiTempo::browseData (basevisitor* v)
{}

std::string msrMidiTempo::asString () const
{
  std::stringstream s;

  s <<
    "[MidiTempo" <<
    ", midiTempoDuration = \"" << fMidiTempoDuration << "\"" <<
    ", midiTempoPerSecond: " << fMidiTempoPerSecond <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrMidiTempo::print (std::ostream& os) const
{
  os << "[MidiTempo" << std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "midiTempoDuration" << ": " << fMidiTempoDuration <<
    std::endl <<
    std::setw (fieldWidth) <<
    "midiTempoPerSecond" << ": " << fMidiTempoPerSecond <<
    std::endl << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMidiTempo& elt)
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
