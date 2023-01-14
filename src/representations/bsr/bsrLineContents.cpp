/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "mfServices.h"
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Creating a newborn clone of line " <<
      asString () <<
      std::endl;
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
void bsrLineContents::setUpLinkToBsrLine (
S_bsrLine upLinkToBsrLine)
{
  fUpLinkToBsrLine = upLinkToBsrLine;
}

S_bsrLine bsrLineContents::getUpLinkToBsrLine () const
{
  S_msrVoice result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
  return fUpLinkToBsrLine;
  }

  return result;
}
*/

void bsrLineContents::appendLineElementToLineContents (
  const S_bsrLineContentsElement& lineElement)
{
  fLineContentsLineElementsList.push_back (lineElement);
}

void bsrLineContents::insertLineElementBeforeLastElementOfLineContents (
  const S_bsrLineContentsElement& lineElement)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting line element '" <<
      lineElement->asShortString () <<
      "' before the last element of line contents '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  size_t
    lineContentsElementsListSize =
      fLineContentsLineElementsList.size ();

  if (lineContentsElementsListSize) {
    std::list<S_bsrLineContentsElement>::iterator it =
      fLineContentsLineElementsList.begin();

    std::advance (it, lineContentsElementsListSize - 1);

    fLineContentsLineElementsList.insert (
      it, lineElement);
  }
  else {
    std::stringstream s;

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
    std::list<S_bsrLineContentsElement>::const_iterator i =
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
    std::list<S_bsrLineContentsElement>::const_iterator i =
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContents::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContents::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContents::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineContents::browseData (basevisitor* v)
{
  for (
    std::list<S_bsrLineContentsElement>::const_iterator i =
      fLineContentsLineElementsList.begin ();
    i != fLineContentsLineElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrLineContentsElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string bsrLineContentsKindAsString (
  bsrLineContentsKind lineContentsKind)
{
  std::string result;

  switch (lineContentsKind) {
    case bsrLineContentsKind::kLineContentsRegular:
      result = "kLineContentsRegular";
      break;
    case bsrLineContentsKind::kLineContentsContinuation:
      result = "kLineContentsContinuation";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrLineContentsKind& elt)
{
  os << bsrLineContentsKindAsString (elt);
  return os;
}

std::string bsrLineContents::asShortString () const
{
  /* JMI
  // this is overriden all in actual elements
  return "??? bsrLineContents::asString () ???";
  */

  std::stringstream s;

  s <<
   "LineContents" <<
    ", lineContentsKind: " <<
    bsrLineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    mfSingularOrPlural (
      fLineContentsLineElementsList.size (), "lineElement", "lineElements");

  return s.str ();
}

std::string bsrLineContents::asString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}

void bsrLineContents::print (std::ostream& os) const
{
  size_t lineElementsListSize =
    fLineContentsLineElementsList.size ();

  os <<
    "LineContents" <<
    ", lineContentsKind: " <<
    bsrLineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    mfSingularOrPlural (
      lineElementsListSize, "lineElement", "lineElements") <<
    std::endl;

  ++gIndenter;

  // print the line numbers
  const int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "cellsNumber" << ": " << fetchCellsNumber () <<
    std::endl;
  os << std::endl;

  // print the line elements if any
  if (lineElementsListSize || gGlobalBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      std::setw (fieldWidth) <<
      "LineContentsLineElementsList" <<
      ", " <<
      mfSingularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
    if (lineElementsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrLineContentsElement>::const_iterator
        iBegin = fLineContentsLineElementsList.begin (),
        iEnd   = fLineContentsLineElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::string bsrLineContents::asDebugString () const
{
  std::stringstream s;

  size_t lineElementsListSize =
    fLineContentsLineElementsList.size ();

  // collect the line elements debug strings if any
  if (lineElementsListSize) {
    std::list<S_bsrLineContentsElement>::const_iterator
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

std::ostream& operator << (std::ostream& os, const S_bsrLineContents& elt)
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
