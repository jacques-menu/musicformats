/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "bsrWae.h"

#include "bsrLineContents.h"

/* JMI v0.9.67
the following due to:
/Users/menu/libmusicxml-git/src/lib/smartpointer.h:75:40: error: member access into incomplete type 'bsrLine'
/Users/menu/libmusicxml-git/src/formats/bsr/bsrLineContents.cpp:51:18: note: in instantiation of member function 'SMARTP<bsrLine>::~SMARTP' requested here
1 error generated.
*/
#include "bsrLineContents.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrLineContents bsrLineContents::create (
  int                 inputLineNumber,
  bsrLineContentsKind lineContentsKind)
{
  bsrLineContents* obj =
    new bsrLineContents (
      inputLineNumber,
      lineContentsKind);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceLines ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of line " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_bsrLineContents
    newbornClone =
      bsrLineContents::create (
        fInputStartLineNumber,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting line element '" <<
      lineElement->asShortString () <<
      "' before the last element of line contents '" <<
      asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  size_t
    lineContentsElementsListSize =
      fLineContentsLineElementsList.size ();

  if (lineContentsElementsListSize) {
    std::list <S_bsrLineContentsElement>::iterator it =
      fLineContentsLineElementsList.begin();

    std::advance (it, lineContentsElementsListSize - 1);

    fLineContentsLineElementsList.insert (
      it, lineElement);
  }
  else {
    std::stringstream ss;

    ss <<
      "line contents elementslist is empty, cannot insert '" <<
      lineElement->asShortString () <<
      "' before its last element";

    bsrInternalError (
      gServiceRunData->getInputSourceName (),
      lineElement->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

S_bsrCellsList bsrLineContents::buildLineContentsElementsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputStartLineNumber);

  for (
    std::list <S_bsrLineContentsElement>::const_iterator i =
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
    std::list <S_bsrLineContentsElement>::const_iterator i =
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLineContents::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLineContents::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrLineContents::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLineContents::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLineContents>*
    p =
      dynamic_cast<visitor<S_bsrLineContents>*> (v)) {
        S_bsrLineContents elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLineContents::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrLineContents::browseData (basevisitor* v)
{
  for (
    std::list <S_bsrLineContentsElement>::const_iterator i =
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

  std::stringstream ss;

  ss <<
   "LineContents" <<
    ", lineContentsKind: " <<
    bsrLineContentsKindAsString (fLineContentsKind) <<
    ", " <<
    mfSingularOrPlural (
      fLineContentsLineElementsList.size (), "lineElement", "lineElements");

  return ss.str ();
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
  constexpr int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "cellsNumber" << ": " << fetchCellsNumber () <<
    std::endl;
  os << std::endl;

  // print the line elements if any
  if (lineElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      std::setw (fieldWidth) <<
      "LineContentsLineElementsList" <<
      ", " <<
      mfSingularOrPlural (
        lineElementsListSize, "lineElement", "lineElements");
    if (lineElementsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_bsrLineContentsElement>::const_iterator
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
  std::stringstream ss;

  size_t lineElementsListSize =
    fLineContentsLineElementsList.size ();

  // collect the line elements debug strings if any
  if (lineElementsListSize) {
    std::list <S_bsrLineContentsElement>::const_iterator
      iBegin = fLineContentsLineElementsList.begin (),
      iEnd   = fLineContentsLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i)->asDebugString ();
      if (++i == iEnd) break;
 // JMI     ss << ' ';
    } // for
  }

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const S_bsrLineContents& elt)
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
