/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"



#include "bsrLines.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "bsrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrLine bsrLine::create (
  int inputLineNumber,
  int printLineNumber,
  int cellsPerLine)
{
  bsrLine* o =
    new bsrLine (
      inputLineNumber, printLineNumber, cellsPerLine);
  assert (o != nullptr);
  return o;
}

bsrLine::bsrLine (
  int inputLineNumber,
  int printLineNumber,
  int cellsPerLine)
    : bsrPageElement (inputLineNumber)
{
  fPrintLineNumber   = printLineNumber;
  fBrailleLineNumber = fPrintLineNumber; // will be set by BSR finalizer

  fCellsPerLine = cellsPerLine;

  fLineNumberCellsList = buildLineNumberCellsList ();

  fASpaceIsNeededInLine = true;

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Creating bsrLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;
  }
#endif
}

bsrLine::~bsrLine ()
{}

S_bsrLine bsrLine::createLineNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Creating a newborn clone of line " <<
      asString () <<
      std::endl;
  }
#endif

  S_bsrLine
    newbornClone =
      bsrLine::create (
        fInputLineNumber,
        fPrintLineNumber,
        fCellsPerLine);

  // braille line number
  newbornClone->fBrailleLineNumber =
    fBrailleLineNumber;

  return newbornClone;
}

void bsrLine::appendLineElementToLine (
  const S_bsrLineContentsElement& lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContentsKind::kLineContentsRegular);

    // set lineElement's spacesAfter value if needed
    switch (lineContentsToAppendTo->getLineContentsKind ()) {
      case bsrLineContentsKind::kLineContentsRegular:
        // leave it as 0
        break;
      case bsrLineContentsKind::kLineContentsContinuation:
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents std::list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();

    // set lineElement's spacesAfter value
 // JMI   lineElement->setSpacesBefore (1);
  }

  if (fASpaceIsNeededInLine) {
    // append a space to the line elements list // JMI appendSpacesToLine ???
    lineContentsToAppendTo->
      appendLineElementToLineContents (
        bsrSpaces::create (
          fInputLineNumber, 1));

    fASpaceIsNeededInLine = false;
  }

  lineContentsToAppendTo->
    appendLineElementToLineContents (lineElement);
}

void bsrLine::insertElementBeforeLastElementOfLine (
  const S_bsrLineContentsElement& lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContentsKind::kLineContentsRegular);

    // append it to the line contents std::list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();
  }

  lineContentsToAppendTo->
    insertLineElementBeforeLastElementOfLineContents (lineElement);
}

void bsrLine::appendSpacesToLine (S_bsrSpaces spaces)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceSpaces () || gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Appending spaces '" <<
      spaces->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToLine (spaces);
}

void bsrLine::appendKeyToLine (S_bsrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " <<
      key->asShortString () <<
      "' to line '" <<
      asString () <<
      std::endl;
    }
#endif

  appendLineElementToLine (key);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTimeSignatureToLine (S_bsrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      timeSignature->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToLine (timeSignature);
  fASpaceIsNeededInLine = true;
}

void bsrLine::insertTimeBeforeLastElementOfLine (S_bsrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Inserting time signature '" <<
      timeSignature->asShortString () <<
      "' before the last element of line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  insertElementBeforeLastElementOfLine (timeSignature);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTempoToLine (S_bsrTempo tempo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to BSR line " <<
      asString () <<
      std::endl;
    }
#endif

  appendLineElementToLine (tempo);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendMeasureToLine (S_bsrMeasure measure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures () || gGlobalBsrOahGroup->getTraceLines ()) {
    gLogStream <<
      "Appending measure '" <<
      measure->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToLine (measure);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendLineElementToLastMeasureOfLine (
  const S_bsrLineContentsElement& lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContentsKind::kLineContentsRegular);

    // set lineElement's spacesAfter value if needed
    switch (lineContentsToAppendTo->getLineContentsKind ()) {
      case bsrLineContentsKind::kLineContentsRegular:
        // leave it as 0
        break;
      case bsrLineContentsKind::kLineContentsContinuation:
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents std::list
    fLineContentsList.push_back (lineContentsToAppendTo);
  }
  else {
    lineContentsToAppendTo = fLineContentsList.back ();

    // set lineElement's spacesAfter value
 // JMI   lineElement->setSpacesBefore (1);
  }

  if (fASpaceIsNeededInLine) {
    // append a space to the line elements list // JMI appendSpacesToLine ???
    lineContentsToAppendTo->
      appendLineElementToLineContents (
        bsrSpaces::create (
          fInputLineNumber, 1));

    fASpaceIsNeededInLine = false;
  }

  lineContentsToAppendTo->
    appendLineElementToLineContents (lineElement);
}

void bsrLine::appendNoteToLine (S_bsrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToLastMeasureOfLine (note); // last measure? JMI
}

S_bsrCellsList bsrLine::buildLineNumberCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  if (! gGlobalMsr2bsrOahGroup->getNoLineNumbers ()) {
    // create the print line number
    S_bsrNumber
      printLineNumber =
        bsrNumber::create (
          fInputLineNumber,
          fPrintLineNumber,
          bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

    // append it to result
    result->
      appendCellsListToCellsList (
        printLineNumber->
          fetchCellsList ());

    if (fBrailleLineNumber != fPrintLineNumber) { // JMI
      // create the braille line number
      S_bsrNumber
        brailleLineNumber =
          bsrNumber::create (
            fInputLineNumber,
            fBrailleLineNumber,
            bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

      // append it to result
      result->
        appendCellsListToCellsList (
          brailleLineNumber->
            fetchCellsList ());
    }
  }

  return result;
}

int bsrLine::fetchCellsNumber () const
{
  int result = 0;

  for (
    std::list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
    i != fLineContentsList.end ();
    ++i ) {
    result += (*i)->fetchCellsNumber ();
  } // for

  return result;
}

void bsrLine::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLine::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLine::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLine::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLine::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLine::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLine::browseData (basevisitor* v)
{
  for (
    std::list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
    i != fLineContentsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrLineContents> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string bsrLine::asString () const
{
  std::stringstream s;

  s <<
    "Line" <<
    ", printLineNumber" << " : " << fPrintLineNumber <<
    ", brailleLineNumber" << " : " << fBrailleLineNumber <<
    ", cellsPerLine" << " : " << fCellsPerLine <<
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << " : " << fetchCellsNumber () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrLine::asDebugString () const
{
  std::stringstream s;

  s <<
    "L" <<
    ", printLineNumber" << " : " << fPrintLineNumber <<
    ", brailleLineNumber" << " : " << fBrailleLineNumber <<
    ", cellsPerLine" << " : " << fCellsPerLine <<
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << " : " << fetchCellsNumber ();

  return s.str ();
}

void bsrLine::print (std::ostream& os) const
{
  size_t lineContentsListSize = fLineContentsList.size ();

  os <<
    "Line" <<
    ", printLineNumber: " << fPrintLineNumber <<
    ", " <<
    mfSingularOrPlural (
      lineContentsListSize, "lineContent", "lineContents") <<
    std::endl;

  ++gIndenter;

  // print the line numbers
  const int fieldWidth = 20;

  os << std::left <<
  /* JMI
    std::setw (fieldWidth) <<
    "printLineNumber" << " : " << fPrintLineNumber <<
    std::endl <<
    */
    std::setw (fieldWidth) <<
    "brailleLineNumber" << " : " << fBrailleLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "cellsPerLine" << " : " << fCellsPerLine <<
    std::endl <<
    std::setw (fieldWidth) <<
    "lineNumberCellsList" << " : " << buildLineNumberCellsList () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    std::endl;
  os << std::endl;

  // print the line contents if any
  if (lineContentsListSize || gGlobalBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      std::setw (fieldWidth) <<
      "LineContentsList" <<
      ", " <<
      mfSingularOrPlural (
        lineContentsListSize, "lineContent", "lineContents");
    if (lineContentsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrLineContents>::const_iterator
        iBegin = fLineContentsList.begin (),
        iEnd   = fLineContentsList.end (),
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
        " : " << "[EMPTY]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrLine& elt)
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
