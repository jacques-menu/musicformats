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

#include "msrMidi.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMidiTempo msrMidiTempo::create (
  int           inputLineNumber,
  const std::string& midiTempoNotesDuration,
  int           midiTempoPerSecond)
{
  msrMidiTempo* obj =
    new msrMidiTempo (
      inputLineNumber,
      midiTempoNotesDuration,
      midiTempoPerSecond);
  assert (obj != nullptr);
  return obj;
}

msrMidiTempo::msrMidiTempo (
  int           inputLineNumber,
  const std::string& midiTempoNotesDuration,
  int           midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoNotesDuration = midiTempoNotesDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidiTempo::msrMidiTempo ()
    : msrElement (0) // input line number
{
  fMidiTempoNotesDuration = "2";
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
        fMidiTempoNotesDuration,
        fMidiTempoPerSecond);

  return newbornClone;
}

void msrMidiTempo::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMidiTempo::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMidiTempo::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMidiTempo::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMidiTempo::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMidiTempo>*
    p =
      dynamic_cast<visitor<S_msrMidiTempo>*> (v)) {
        S_msrMidiTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMidiTempo::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrMidiTempo::browseData (basevisitor* v)
{}

std::string msrMidiTempo::asString () const
{
  std::stringstream ss;

  ss <<
    "[MidiTempo" <<
    ", midiTempoNotesDuration = \"" << fMidiTempoNotesDuration << "\"" <<
    ", midiTempoPerSecond: " << fMidiTempoPerSecond <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMidiTempo::print (std::ostream& os) const
{
  os << "[MidiTempo" << std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "midiTempoNotesDuration" << ": " << fMidiTempoNotesDuration <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
