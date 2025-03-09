/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrCellsLists.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5);
  assert (obj != nullptr);
  return obj;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
{
  bsrCellsList* obj =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5, cellKind6);
  assert (obj != nullptr);
  return obj;
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber)
 //   : bsrElement (inputLineNumber)
{}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
  fCellsListElements.push_back (cellKind5);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
  fCellsListElements.push_back (cellKind5);
  fCellsListElements.push_back (cellKind6);
}

bsrCellsList::~bsrCellsList ()
{}

void bsrCellsList::appendCellsListToCellsList (
  const S_bsrCellsList& otherCellsList)
{
  if (otherCellsList) {
    if (otherCellsList->fCellsListElements.size ()) {
      std::list <bsrCellKind>::const_iterator
        iBegin = otherCellsList->fCellsListElements.begin (),
        iEnd   = otherCellsList->fCellsListElements.end (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsListElements.push_back ((*i));
        if (++i == iEnd) break;
   // JMI      os << ' ';
      } // for
    }
  }
}

void bsrCellsList::prependCellsListToCellsList (
  const S_bsrCellsList& otherCellsList)
{
  if (otherCellsList) {
    if (otherCellsList->fCellsListElements.size ()) {
      std::list <bsrCellKind>::const_reverse_iterator
        iBegin = otherCellsList->fCellsListElements.rbegin (),
        iEnd   = otherCellsList->fCellsListElements.rend (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsListElements.push_front ((*i));
        if (++i == iEnd) break;
   // JMI      os << ' ';
      } // for
    }
  }
}

/* JMI
void bsrCellsList::generateBrailleCode (std::ostream& os)
{
  if (fCellsListElements.size ()) {
    std::list <bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       os << ' ';
    } // for
  }
}
*/

void bsrCellsList::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrCellsList::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrCellsList>*
    p =
      dynamic_cast<visitor<S_bsrCellsList>*> (v)) {
        S_bsrCellsList elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrCellsList::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrCellsList::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrCellsList::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrCellsList>*
    p =
      dynamic_cast<visitor<S_bsrCellsList>*> (v)) {
        S_bsrCellsList elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrCellsList::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrCellsList::browseData (basevisitor* v)
{}


std::string bsrCellsList::asString () const
{
  std::stringstream ss;

  ss <<
    "cellsListElements [";

  if (fCellsListElements.size ()) {
    std::list <bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       ss << ' ';
    } // for
  }

  ss <<
    ']';

  return ss.str ();
}

std::string bsrCellsList::asShortString () const
{
  return asString ();
}

void bsrCellsList::print (std::ostream& os) const
{
  os <<
    "cellsListElements [";

  if (fCellsListElements.size ()) {
    std::list <bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       os << ' ';
    } // for
  }

  os <<
    ']';
}

std::ostream& operator << (std::ostream& os, const S_bsrCellsList& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

bsrCellsList& operator << (bsrCellsList& cellsList, const bsrCellKind cellKind)
{
  cellsList.appendCellKindToCellsList (cellKind);
  return cellsList;
}


}
