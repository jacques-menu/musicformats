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

#include "msrWae.h"

#include "mfStaticSettings.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrTablatures.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Creating frame note '" <<
      asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrFrameNote::~msrFrameNote ()
{}

std::string msrBarreTypeKindAsString (
  msrBarreTypeKind barreTypeKind)
{
  std::string result;

  switch (barreTypeKind) {
    case msrBarreTypeKind::kBarreTypeNone:
      result = "barreTypeNone";
      break;
    case msrBarreTypeKind::kBarreTypeStart:
      result = "barreTypeStart";
      break;
    case msrBarreTypeKind::kBarreTypeStop:
      result = "barreTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBarreTypeKind& elt)
{
  os << msrBarreTypeKindAsString (elt);
  return os;
}

std::string msrFrameNote::asString () const
{
  std::stringstream ss;

  ss <<
    "[FrameNote" <<
    ", fFrameNoteStringNumber: " << fFrameNoteStringNumber <<
    ", fFrameNoteFretNumber: " << fFrameNoteFretNumber <<
    ", fFrameNoteFingering: " << fFrameNoteFingering <<
    ", fFrameNoteBarreTypeKind: " << fFrameNoteBarreTypeKind <<
    ", line: " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrFrameNote::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFrameNote::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFrameNote::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrFrameNote::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFrameNote::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFrameNote>*
    p =
      dynamic_cast<visitor<S_msrFrameNote>*> (v)) {
        S_msrFrameNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFrameNote::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrFrameNote::browseData (basevisitor* v)
{}

void msrFrameNote::print (std::ostream& os) const
{
  os <<
    "[FrameNote" <<
    ", line: " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os <<
    std::setw (fieldWidth) <<
    "fFrameNoteStringNumber" << ": " << fFrameNoteStringNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameNoteFretNumber" << ": " << fFrameNoteFretNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameNoteFingering" << ": " << fFrameNoteFingering <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameNoteBarreTypeKind" << ": " << fFrameNoteBarreTypeKind <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrFrameNote& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Creating frame '" <<
      asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrFrame::~msrFrame ()
{}

void msrFrame::appendFrameNoteToFrame (
  const S_msrFrameNote& frameNote)
{
  int inputLineNumber =
    frameNote->getInputLineNumber ();

  fFrameFrameNotesList.push_back (
    frameNote);

  // handle nested barre start/stop
  switch (frameNote->getFrameNoteBarreTypeKind ()) {
    case msrBarreTypeKind::kBarreTypeNone:
      break;

    case msrBarreTypeKind::kBarreTypeStart:
      fPendingBarreStartFrameNotes.push (
        frameNote);
      break;

    case msrBarreTypeKind::kBarreTypeStop:
      {
        if (! fPendingBarreStartFrameNotes.size ()) {
          std::stringstream ss;

          ss <<
            "frame note  with barre stop has no maching barre start" <<
            frameNote;

          msrError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str ());
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
            std::stringstream ss;

            ss <<
              "frame note with barre stop has is at fret" <<
              barreStopFretNumber <<
              "while the matching barre start is at fret" <<
              barreStartFretNumber;

            msrError                                                   (
              gServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              ss.str ());
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

std::string msrFrame::asString () const
{
  std::stringstream ss;

  ss <<
    "Frame" <<
    ", fFrameStringsNumber: " << fFrameStringsNumber <<
    ", fFrameFretsNumber: " << fFrameFretsNumber <<
    ", fFrameFirstFretNumber: " << fFrameFirstFretNumber <<
    mfSingularOrPlural (
      fFrameFrameNotesList.size (), "frame note", "frame notes") <<
    mfSingularOrPlural (
      fFrameBarresList.size (), "barre", "barres") <<
    ", fFrameContainsFingerings: " << fFrameContainsFingerings;

  return ss.str ();
}

void msrFrame::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFrame::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFrame::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrFrame::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFrame::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFrame>*
    p =
      dynamic_cast<visitor<S_msrFrame>*> (v)) {
        S_msrFrame elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFrame::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrFrame::browseData (basevisitor* v)
{
  // browse frame notes if any
  if (fFrameFrameNotesList.size ()) {
    for (
      std::list<S_msrFrameNote>::const_iterator i = fFrameFrameNotesList.begin ();
      i != fFrameFrameNotesList.end ();
      ++i
  ) {
      // browse the harmony degree
      msrBrowser<msrFrameNote> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

void msrFrame::print (std::ostream& os) const
{
  os <<
    "[Frame" <<
     ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 15;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fFrameStringsNumber" << ": " << fFrameStringsNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameFretsNumber" << ": " << fFrameFretsNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameFirstFretNumber" << ": " << fFrameFirstFretNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fFrameContainsFingerings" << ": " << fFrameContainsFingerings <<
    std::endl;

  // print frame notes if any
  os <<
    "fFrameFrameNotesList:";
  if (fFrameFrameNotesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrFrameNote>::const_iterator
      iBegin = fFrameFrameNotesList.begin (),
      iEnd   = fFrameFrameNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the barres if any
  os <<
    "fFrameBarresList:";
  if (fFrameBarresList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<msrBarre>::const_iterator
      iBegin = fFrameBarresList.begin (),
      iEnd   = fFrameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);

      os <<
        "Barre" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 21;

      os << std::left <<
        std::setw (fieldWidth) <<
        "barreStartString" << ": " << barre.getBarreStartString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "barreStopString" << ": " << barre.getBarreStopString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "barreFretNumber" << ": " << barre.getBarreFretNumber () <<
        std::endl;

      --gIndenter;

      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrFrame& elt)
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
