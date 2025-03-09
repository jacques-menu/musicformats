/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrSpaces.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrSpaces bsrSpaces::create (
  int inputLineNumber,
  int numberOfSpaces)
{
  bsrSpaces* obj =
    new bsrSpaces (
      inputLineNumber, numberOfSpaces);
  assert (obj != nullptr);
  return obj;
}

bsrSpaces::bsrSpaces (
  int inputLineNumber,
  int numberOfSpaces)
    : bsrLineContentsElement (inputLineNumber)
{
  fNumberOfSpaces = numberOfSpaces;

  fSpacesCellsList =
    bsrCellsList::create (inputLineNumber);

  for (int i = 1; i <= fNumberOfSpaces; ++i) {
    fSpacesCellsList->
      appendCellKindToCellsList (kCellSpace);
  } // for
}

bsrSpaces::~bsrSpaces ()
{}

void bsrSpaces::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrSpaces::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrSpaces::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

int bsrSpaces::fetchCellsNumber () const
{
  return fNumberOfSpaces;
}

void bsrSpaces::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrSpaces::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrSpaces::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrSpaces::browseData (basevisitor* v)
{}

std::string bsrSpaces::asString () const
{
  std::stringstream ss;

  ss <<
    "Spaces" <<
    ", numberOfSpaces: " << fNumberOfSpaces <<
    ", spacesCellsList: " << fSpacesCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrSpaces::asDebugString () const
{
  std::string result;

  for (int i = 1; i <= fNumberOfSpaces; ++i) {
    result += ' ';
  } // for

  return result;
}

void bsrSpaces::print (std::ostream& os) const
{
  os <<
    "Spaces" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 16;

  os <<
    std::setw (fieldWidth) <<
    "numberOfSpaces" << ": " << fNumberOfSpaces <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesCellsList" << ": " << fSpacesCellsList <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrSpaces& elt)
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
