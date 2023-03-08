/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "mfStaticSettings.h"

#include "bsrBarLines.h"

#include "oahEarlyOptions.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrBarLine bsrBarLine::create (
  int            inputLineNumber,
  bsrBarLineKind barLineKind)
{
  bsrBarLine* obj =
    new bsrBarLine (
      inputLineNumber, barLineKind);
  assert (obj != nullptr);
  return obj;
}

bsrBarLine::bsrBarLine (
  int            inputLineNumber,
  bsrBarLineKind barLineKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fBarLineKind = barLineKind;

  fBarLineCellsList = buildCellsList ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrBarLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

bsrBarLine::~bsrBarLine ()
{}

S_bsrCellsList bsrBarLine::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fBarLineKind) {
    case bsrBarLineKind::kBarLineKindNone:
      break;
    case bsrBarLineKind::kBarLineKindSpecial:
      result->
        appendCellKindToCellsList (
          bsrCellKind::kDots13);
      break;
    case bsrBarLineKind::kBarLineKindUnusual:
      result->
        appendCellKindToCellsList (
          bsrCellKind::kDots123);
      break;
    case bsrBarLineKind::kBarLineKindFinalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            bsrCellKind::kDots126, bsrCellKind::kDots13));
      break;
    case bsrBarLineKind::kBarLineKindSectionalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            bsrCellKind::kDots126, bsrCellKind::kDots13, bsrCellKind::kDots3));
      break;
  } // switch

  return result;
}

int bsrBarLine::fetchCellsNumber() const
{
  return fBarLineCellsList->fetchCellsNumber();
}

void bsrBarLine::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrBarLine::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrBarLine>*
    p =
      dynamic_cast<visitor<S_bsrBarLine>*> (v)) {
        S_bsrBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrBarLine::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrBarLine::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrBarLine::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrBarLine>*
    p =
      dynamic_cast<visitor<S_bsrBarLine>*> (v)) {
        S_bsrBarLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrBarLine::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrBarLine::browseData (basevisitor* v)
{}

std::string bsrBarLineKindAsString (
  bsrBarLineKind barLineKind)
{
  std::string result;

  switch (barLineKind) {
    case bsrBarLineKind::kBarLineKindNone:
      result = "kBarLineKindNone";
      break;
    case bsrBarLineKind::kBarLineKindSpecial:
      result = "kBarLineKindSpecial";
      break;
    case bsrBarLineKind::kBarLineKindUnusual:
      result = "kBarLineKindUnusual";
      break;
    case bsrBarLineKind::kBarLineKindFinalDouble:
      result = "kBarLineKindFinalDouble";
      break;
    case bsrBarLineKind::kBarLineKindSectionalDouble:
      result = "kBarLineKindSectionalDouble";
      break;
  } // switch

  return result;
}

std::string bsrBarLineKindAsDebugString ( // JMI of little use??? v0.9.66
  bsrBarLineKind barLineKind)
{
  std::string result;

  switch (barLineKind) {
    case bsrBarLineKind::kBarLineKindNone:
      result = "kBarLineKindNone***";
      break;
    case bsrBarLineKind::kBarLineKindSpecial:
      result = "kBarLineKindSpecial";
      break;
    case bsrBarLineKind::kBarLineKindUnusual:
      result = "kBarLineKindUnusual";
      break;
    case bsrBarLineKind::kBarLineKindFinalDouble:
      result = "kBarLineKindFinalDouble";
      break;
    case bsrBarLineKind::kBarLineKindSectionalDouble:
      result = "||"; // L'\u2016' ??? JMI v0.9.66
//      result = L'\u1D101';
      break;
  } // switch

  return result;
}

std::string bsrBarLine::asString () const
{
  std::stringstream ss;

  ss <<
    "BarLine" <<
    ", " << bsrBarLineKindAsString (fBarLineKind) <<
    ", barLineCellsList: " << fBarLineCellsList->asShortString () <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrBarLine::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "[BARLINE " <<
    bsrBarLineKindAsDebugString (fBarLineKind) <<
    ']';

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const bsrBarLineKind& elt)
{
  os << bsrBarLineKindAsDebugString (elt);
  return os;
}

void bsrBarLine::print (std::ostream& os) const
{
  os <<
    "BarLine" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os <<
    std::setw (fieldWidth) <<
    "barLineKind" << ": " << bsrBarLineKindAsString (fBarLineKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "barLineCellsList" << ": " << fBarLineCellsList->asShortString () <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrBarLine& elt)
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
