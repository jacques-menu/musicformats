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

#include "mfStringsHandling.h"

#include "msrTablatures.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrFrameNote msrFrameNote::create (
  int              inputLineNumber,
  int              frameNoteStringNumber,
  int              frameNoteFretNumber,
  int              frameNoteFingering,
  msrBarreTypeKind frameNoteBarreTypeKind)
{
  msrFrameNote* o =
    new msrFrameNote (
      inputLineNumber,
      frameNoteStringNumber,
      frameNoteFretNumber,
      frameNoteFingering,
      frameNoteBarreTypeKind);
  assert (o != nullptr);

  return o;
}

msrFrameNote::msrFrameNote (
  int              inputLineNumber,
  int              frameNoteStringNumber,
  int              frameNoteFretNumber,
  int              frameNoteFingering,
  msrBarreTypeKind frameNoteBarreTypeKind)
    : msrElement (inputLineNumber)
{
  fFrameNoteStringNumber = frameNoteStringNumber;
  fFrameNoteFretNumber   = frameNoteFretNumber;

  fFrameNoteFingering = frameNoteFingering;

  fFrameNoteBarreTypeKind = frameNoteBarreTypeKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating frame note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFrameNote::~msrFrameNote ()
{}

string msrFrameNote::barreTypeKindAsString (
  msrBarreTypeKind barreTypeKind)
{
  string result;

  switch (barreTypeKind) {
    case msrFrameNote::kBarreTypeNone:
      result = "barreTypeNone";
      break;
    case msrFrameNote::kBarreTypeStart:
      result = "barreTypeStart";
      break;
    case msrFrameNote::kBarreTypeStop:
      result = "barreTypeStop";
      break;
  } // switch

  return result;
}

string msrFrameNote::asString () const
{
  stringstream s;

  s <<
    "FrameNote" <<
    ", frameNoteStringNumber: " << fFrameNoteStringNumber <<
    ", frameNoteFretNumber: " << fFrameNoteFretNumber <<
    ", frameNoteFingering: " << fFrameNoteFingering <<
    ", frameNoteBarreTypeKind: " <<
    barreTypeKindAsString (
      fFrameNoteBarreTypeKind) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrFrameNote::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFrameNote::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFrameNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFrameNote::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFrameNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFrameNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFrameNote::browseData (basevisitor* v)
{}

void msrFrameNote::print (ostream& os) const
{
  os <<
    "[FrameNote" <<
    ", line: " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "frameNoteStringNumber" << " : " << fFrameNoteStringNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteFretNumber" << " : " << fFrameNoteFretNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteFingering" << " : " << fFrameNoteFingering <<
    endl <<
    setw (fieldWidth) <<
    "frameNoteBarreTypeKind" << " : " <<
    barreTypeKindAsString (
      fFrameNoteBarreTypeKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrFrameNote& elt)
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
S_msrFrame msrFrame::create (
  int inputLineNumber,
  int frameStringsNumber,
  int frameFretsNumber,
  int frameFirstFretNumber)
{
  msrFrame* o =
    new msrFrame (
      inputLineNumber,
      frameStringsNumber,
      frameFretsNumber,
      frameFirstFretNumber);
  assert (o != nullptr);

  return o;
}

msrFrame::msrFrame (
  int inputLineNumber,
  int frameStringsNumber,
  int frameFretsNumber,
  int frameFirstFretNumber)
    : msrElement (inputLineNumber)
{
  fFrameStringsNumber   = frameStringsNumber;
  fFrameFretsNumber     = frameFretsNumber;
  fFrameFirstFretNumber = frameFirstFretNumber;

  fFrameContainsFingerings = false;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating frame '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFrame::~msrFrame ()
{}

void msrFrame::appendFrameNoteToFrame (
  S_msrFrameNote frameNote)
{
  int inputLineNumber =
    frameNote->getInputLineNumber ();

  fFrameFrameNotesList.push_back (
    frameNote);

  // handle nested barre start/stop
  switch (frameNote->getFrameNoteBarreTypeKind ()) {
    case msrFrameNote::kBarreTypeNone:
      break;

    case msrFrameNote::kBarreTypeStart:
      fPendingBarreStartFrameNotes.push (
        frameNote);
      break;

    case msrFrameNote::kBarreTypeStop:
      {
        if (! fPendingBarreStartFrameNotes.size ()) {
          stringstream s;

          s <<
            "frame note  with barre stop has no maching barre start" <<
            frameNote;

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        else {
          S_msrFrameNote
            pendingBarreStartFrameNotesTop =
              fPendingBarreStartFrameNotes.top ();

          int
            barreStartFretNumber =
              pendingBarreStartFrameNotesTop->
                getFrameNoteFretNumber (),
            barreStopFretNumber =
              frameNote->
                getFrameNoteFretNumber ();

          if (barreStartFretNumber != barreStopFretNumber) {
            stringstream s;

            s <<
              "frame note with barre stop has is at fret" <<
              barreStopFretNumber <<
              "while the matching barre start is at fret" <<
              barreStartFretNumber;

            msrError                                                   (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }

          fFrameBarresList.push_back (
            msrBarre (
              pendingBarreStartFrameNotesTop->
                getFrameNoteStringNumber (),
              frameNote->
                getFrameNoteStringNumber (),
              barreStartFretNumber));

          fPendingBarreStartFrameNotes.pop ();
        }
      }
      break;
  } // switch

  // are there fingerings in this frame?
  if (frameNote->getFrameNoteFingering () != -1) {
    fFrameContainsFingerings = true;
  }
}

string msrFrame::asString () const
{
  stringstream s;

  s <<
    "Frame" <<
    ", fFrameStringsNumber: " << fFrameStringsNumber <<
    ", fFrameFretsNumber: " << fFrameFretsNumber <<
    ", fFrameFirstFretNumber: " << fFrameFirstFretNumber <<
    mfSingularOrPlural (
      fFrameFrameNotesList.size (), "frame note", "frame notes") <<
    mfSingularOrPlural (
      fFrameBarresList.size (), "barre", "barres") <<
    ", fFrameContainsFingerings: " << fFrameContainsFingerings;

  return s.str ();
}

void msrFrame::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFrame::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFrame::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFrame::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFrame::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFrame::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFrame::browseData (basevisitor* v)
{
  // browse frame notes if any
  if (fFrameFrameNotesList.size ()) {
    for (
      list<S_msrFrameNote>::const_iterator i = fFrameFrameNotesList.begin ();
      i != fFrameFrameNotesList.end ();
      ++i
  ) {
      // browse the harmony degree
      msrBrowser<msrFrameNote> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

void msrFrame::print (ostream& os) const
{
  os <<
    "[Frame" <<
     ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "frameStringsNumber" << " = " << fFrameStringsNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameFretsNumber" << " = " << fFrameFretsNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameFirstFretNumber" << " = " << fFrameFirstFretNumber <<
    endl <<
    setw (fieldWidth) <<
    "frameContainsFingerings" << " = " << fFrameContainsFingerings <<
    endl;

  // print frame notes if any
  os <<
    "fFrameFrameNotesList:";
  if (fFrameFrameNotesList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrFrameNote>::const_iterator
      iBegin = fFrameFrameNotesList.begin (),
      iEnd   = fFrameFrameNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      endl;
  }

  // print the barres if any
  os <<
    "fFrameBarresList:";
  if (fFrameBarresList.size ()) {
    os << endl;

    ++gIndenter;

    list<msrBarre>::const_iterator
      iBegin = fFrameBarresList.begin (),
      iEnd   = fFrameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);

      os <<
        "Barre" <<
        endl;

      ++gIndenter;

      const int fieldWidth = 21;

      os << left <<
        setw (fieldWidth) <<
        "barreStartString" << " : " << barre.getBarreStartString () <<
        endl <<
        setw (fieldWidth) <<
        "barreStopString" << " : " << barre.getBarreStopString () <<
        endl <<
        setw (fieldWidth) <<
        "barreFretNumber" << " : " << barre.getBarreFretNumber () <<
        endl;

      --gIndenter;

      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrFrame& elt)
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
