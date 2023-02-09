/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>

#include "visitor.h"

#include "bsrWords.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// brailling characters and words
S_bsrCellsList brailleCharacter (
  int  inputLineNumber,
  char ch)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (ch) {
    case 'a':
      result->
        appendCellKindToCellsList (kCellA);
      break;
    case 'b':
      result->
        appendCellKindToCellsList (kCellB);
      break;
    case 'c':
      result->
        appendCellKindToCellsList (kCellC);
      break;
    case 'd':
      result->
        appendCellKindToCellsList (kCellD);
      break;
    case 'e':
      result->
        appendCellKindToCellsList (kCellE);
      break;
    case 'f':
      result->
        appendCellKindToCellsList (kCellF);
      break;
    case 'g':
      result->
        appendCellKindToCellsList (kCellG);
      break;
    case 'h':
      result->
        appendCellKindToCellsList (kCellH);
      break;
    case 'i':
      result->
        appendCellKindToCellsList (kCellI);
      break;
    case 'j':
      result->
        appendCellKindToCellsList (kCellJ);
      break;
    case 'k':
      result->
        appendCellKindToCellsList (kCellK);
      break;
    case 'l':
      result->
        appendCellKindToCellsList (kCellL);
      break;
    case 'm':
      result->
        appendCellKindToCellsList (kCellM);
      break;
    case 'n':
      result->
        appendCellKindToCellsList (kCellN);
      break;
    case 'o':
      result->
        appendCellKindToCellsList (kCellO);
      break;
    case 'p':
      result->
        appendCellKindToCellsList (kCellP);
      break;
    case 'q':
      result->
        appendCellKindToCellsList (kCellQ);
      break;
    case 'r':
      result->
        appendCellKindToCellsList (kCellR);
      break;
    case 's':
      result->
        appendCellKindToCellsList (kCellS);
      break;
    case 't':
      result->
        appendCellKindToCellsList (kCellT);
      break;
    case 'u':
      result->
        appendCellKindToCellsList (kCellU);
      break;
    case 'v':
      result->
        appendCellKindToCellsList (kCellV);
      break;
    case 'w':
      result->
        appendCellKindToCellsList (kCellW);
      break;
    case 'x':
      result->
        appendCellKindToCellsList (kCellX);
      break;
    case 'y':
      result->
        appendCellKindToCellsList (kCellY);
      break;
    case 'z':
      result->
        appendCellKindToCellsList (kCellZ);
      break;

    case '(': // JMI literary???
      result->
        appendCellKindToCellsList (kCellParenthesis);
      break;
    case ')':
      result->
        appendCellKindToCellsList (kCellParenthesis);
      break;


    default:
      result->
        appendCellKindToCellsList (kCellQuestionMark);
  } // switch

  return result;
}

S_bsrCellsList brailleWord (
  int           inputLineNumber,
  const std::string& str)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        inputLineNumber, kCellWordSign);

  if (str.size ()) {
    std::string::const_iterator
      iBegin = str.begin (),
      iEnd   = str.end (),
      i      = iBegin;
    for ( ; ; ) {
      char ch = (*i);

      result->
        appendCellsListToCellsList (
          brailleCharacter (
            inputLineNumber, ch));

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

//______________________________________________________________________________
S_bsrWords bsrWords::create (
  int           inputLineNumber,
  const std::string& wordContents)
{
  bsrWords* o =
    new bsrWords (
      inputLineNumber, wordContents);
  assert (o != nullptr);
  return o;
}

bsrWords::bsrWords (
  int           inputLineNumber,
  const std::string& wordContents)
    : bsrLineContentsElement (inputLineNumber)
{
  fWordContents = wordContents;

  fWordCellsList =
    brailleWord (
      inputLineNumber, fWordContents);
}

bsrWords::~bsrWords ()
{}

void bsrWords::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrWords::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_bsrWords>*
    p =
      dynamic_cast<visitor<S_bsrWords>*> (v)) {
        S_bsrWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrWords::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

int bsrWords::fetchCellsNumber () const
{
  return fWordCellsList->fetchCellsNumber ();
}

void bsrWords::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrWords::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_bsrWords>*
    p =
      dynamic_cast<visitor<S_bsrWords>*> (v)) {
        S_bsrWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrWords::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrWords::browseData (basevisitor* v)
{}

std::string bsrWords::asString () const
{
  std::stringstream ss;

  ss <<
    "Word" <<
    ", wordContents: " << fWordContents <<
    ", wordCellsList: " << fWordCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrWords::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "WD" <<
    fWordContents <<
    ", spacesBefore: " << fSpacesBefore;

  return ss.str ();
}

void bsrWords::print (std::ostream& os) const
{
  os <<
    "Word" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    std::setw (fieldWidth) <<
    "wordContents" << ": " << fWordContents <<
    std::endl <<
    std::setw (fieldWidth) <<
    "wordCellsList" << ": " << fWordCellsList <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrWords& elt)
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
