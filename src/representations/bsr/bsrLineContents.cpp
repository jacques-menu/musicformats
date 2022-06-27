/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "bsrWae.h"

#include "bsrLineContents.h"

/* the following due to:
/Users/menu/libmusicxml-git/src/lib/smartpointer.h:75:40: error: member access into incomplete type 'bsrLine'
/Users/menu/libmusicxml-git/src/formats/bsr/bsrLineContents.cpp:51:18: note: in instantiation of member function 'SMARTP<bsrLine>::~SMARTP' requested here
1 error generated.
*/
#include "bsrLineContents.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "bsrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrLineContents bsrLineContents::create (
  int                 inputLineNumber,
  bsrLineContentsKind lineContentsKind)
{
  bsrLineContents* o =
    new bsrLineContents (
      inputLineNumber,
      lineContentsKind);
  assert (o != nullptr);
  return o;
}

bsrLineContents::bsrLineContents (
  int                 inputLineNumber,
  bsrLineContentsKind lineContentsKind)
    : bsrElement (inputLineNumber)
{
  fLineContentsKind = lineContentsKind;
}

bsrLineContents::~bsrLineContents ()
{}

S_bsrLineContents bsrLineContents::createLineNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Creating a newborn clone of line " <<
      asString () <<
      endl;
  }
#endif

  S_bsrLineContents
    newbornClone =
      bsrLineContents::create (
        fInputLineNumber,
        fLineContentsKind);

  return newbornClone;
}

/*
void bsrLineContents::setBsrLineUpLink (
S_bsrLine bsrLineUpLink)
{
  fBsrLineUpLink = bsrLineUpLink;
}

S_bsrLine bsrLineContents::getBsrLineUpLink () const
{
  S_msrVoice result;

  if (fNoteDirectMeasureUpLink) {
    result =
  return fBsrLineUpLink;
  }

  return result;
}
*/

void bsrLineContents::appendLineElementToLineContents (
  S_bsrLineContentsElement lineElement)
{
  fLineContentsLineElementsList.push_back (lineElement);
}

void bsrLineContents::insertLineElementBeforeLastElementOfLineContents (
  S_bsrLineContentsElement lineElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting line element '" <<
      lineElement->asShortString () <<
      "' before the last element of line contents '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  size_t
    lineContentsElementsListSize =
      fLineContentsLineElementsList.size ();

  if (lineContentsElementsListSize) {
    list<S_bsrLineContentsElement>::iterator it =
      fLineContentsLineElementsList.begin();

    std::advance (it, lineContentsElementsListSize - 1);

    fLineContentsLineElementsList.insert (
      it, lineElement);
  }
  else {
    stringstream s;

    s <<
      "line contents elementslist is empty, cannot insert '" <<
      lineElement->asShortString () <<
      "' before its last element";

    bsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      lineElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

S_bsrCellsList bsrLineContents::buildLineContentsElementsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  for (
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    ++i
  ) {
    S_bsrLineContentsElement lineElement = (*i);

    result->
      appendCellsListToCellsList (
        lineElement->
          fetchCellsList ());
  } // for

  return result;
}

int bsrLineContents::fetchCellsNumber () const
{
  int result = 0;

  for (
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    ++i
  ) {
    S_bsrLineContentsElement lineElement = (*i);

    result += lineElement->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLineContents::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContents::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContents::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineContents::browseData (basevisitor* v)
{
  for (
    list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrLineContentsElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string bsrLineContents::lineContentsKindAsString (
  bsrLineContentsKind lineContentsKind)
{
  string result;

  switch (lineContentsKind) {
    case bsrLineContents::kLineContentsRegular:
      result = "lineContentsRegular";
      break;
    case bsrLineContents::kLineContentsContinuation:
      result = "lineContentsContinuation";
      break;
  } // switch

  return result;
}

string bsrLineContents::asShortString () const
{
  /* JMI
  // this is overriden all in actual elements
  return "??? bsrLineContents::asString () ???";
  */

  stringstream s;

  s <<
   "LineContents" <<
    ", lineContentsKind: " <<
    lineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    mfSingularOrPlural (
      fLineContentsLineElementsList.size (), "lineElement", "lineElements");

  return s.str ();
}

string bsrLineContents::asString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}

void bsrLineContents::print (ostream& os) const
{
  size_t lineElementsListSize =
    fLineContentsLineElementsList.size ();

  os <<
    "LineContents" <<
    ", lineContentsKind: " <<
    lineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    mfSingularOrPlural (
      lineElementsListSize, "lineElement", "lineElements") <<
    endl;

  ++gIndenter;

  // print the line numbers
  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os << endl;

  // print the line elements if any
  if (lineElementsListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
//      setw (fieldWidth) <<
      "LineContentsLineElementsList" <<
      ", " <<
      mfSingularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
    if (lineElementsListSize) {
      os << endl;
      ++gIndenter;

      list<S_bsrLineContentsElement>::const_iterator
        iBegin = fLineContentsLineElementsList.begin (),
        iEnd   = fLineContentsLineElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  --gIndenter;
}

string bsrLineContents::asDebugString () const
{
  stringstream s;

  size_t lineElementsListSize =
    fLineContentsLineElementsList.size ();

  // collect the line elements debug strings if any
  if (lineElementsListSize) {
    list<S_bsrLineContentsElement>::const_iterator
      iBegin = fLineContentsLineElementsList.begin (),
      iEnd   = fLineContentsLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asDebugString ();
      if (++i == iEnd) break;
 // JMI     s << ' ';
    } // for
  }

  return s.str ();
}

ostream& operator<< (ostream& os, const S_bsrLineContents& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
