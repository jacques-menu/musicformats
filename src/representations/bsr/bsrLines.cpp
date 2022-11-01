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

#include "mfStringsHandling.h"



#include "bsrLines.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "bsrBrowsers.h"


using namespace std;

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
      endl;
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
      endl;
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

void bsrLine::appendLineElementToLine (S_bsrLineContentsElement lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContents::kLineContentsRegular);

    // set lineElement's spacesAfter value if needed
    switch (lineContentsToAppendTo->getLineContentsKind ()) {
      case bsrLineContents::kLineContentsRegular:
        // leave it as 0
        break;
      case bsrLineContents::kLineContentsContinuation:
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents list
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
  S_bsrLineContentsElement lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContents::kLineContentsRegular);

    // append it to the line contents list
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
      endl;
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
      endl;
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
      endl;
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
      endl;
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
      endl;
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
      endl;
    }
#endif

  appendLineElementToLine (measure);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendLineElementToLastMeasureOfLine (
  S_bsrLineContentsElement lineElement)
{
  S_bsrLineContents
    lineContentsToAppendTo;

  if (! fLineContentsList.size ()) {
    // first insertion in this line: create the first, regular line contents
    lineContentsToAppendTo =
      bsrLineContents::create (
        lineElement->getInputLineNumber (),
        bsrLineContents::kLineContentsRegular);

    // set lineElement's spacesAfter value if needed
    switch (lineContentsToAppendTo->getLineContentsKind ()) {
      case bsrLineContents::kLineContentsRegular:
        // leave it as 0
        break;
      case bsrLineContents::kLineContentsContinuation:
    // JMI    lineElement->setSpacesBefore (2);
        break;
    } // switch

    // append it to the line contents list
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
      endl;
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
          bsrNumber::kNumberSignIsNeededYes);

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
            bsrNumber::kNumberSignIsNeededYes);

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
    list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
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
      endl;
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
            endl;
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
      endl;
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
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLine::browseData (basevisitor* v)
{
  for (
    list<S_bsrLineContents>::const_iterator i = fLineContentsList.begin ();
    i != fLineContentsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrLineContents> browser (v);
    browser.browse (*(*i));
  } // for
}

string bsrLine::asString () const
{
  stringstream s;

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

string bsrLine::asDebugString () const
{
  stringstream s;

  s <<
    "L" <<
    ", printLineNumber" << " : " << fPrintLineNumber <<
    ", brailleLineNumber" << " : " << fBrailleLineNumber <<
    ", cellsPerLine" << " : " << fCellsPerLine <<
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << " : " << fetchCellsNumber ();

  return s.str ();
}

void bsrLine::print (ostream& os) const
{
  size_t lineContentsListSize = fLineContentsList.size ();

  os <<
    "Line" <<
    ", printLineNumber: " << fPrintLineNumber <<
    ", " <<
    mfSingularOrPlural (
      lineContentsListSize, "lineContent", "lineContents") <<
    endl;

  ++gIndenter;

  // print the line numbers
  const int fieldWidth = 20;

  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "printLineNumber" << " : " << fPrintLineNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "brailleLineNumber" << " : " << fBrailleLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "cellsPerLine" << " : " << fCellsPerLine <<
    endl <<
    setw (fieldWidth) <<
    "lineNumberCellsList" << " : " << buildLineNumberCellsList () <<
    endl <<
    setw (fieldWidth) <<
    "cellsNumber" << " : " << fetchCellsNumber () <<
    endl;
  os << endl;

  // print the line contents if any
  if (lineContentsListSize || gGlobalBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      setw (fieldWidth) <<
      "LineContentsList" <<
      ", " <<
      mfSingularOrPlural (
        lineContentsListSize, "lineContent", "lineContents");
    if (lineContentsListSize) {
      os << endl;
      ++gIndenter;

      list<S_bsrLineContents>::const_iterator
        iBegin = fLineContentsList.begin (),
        iEnd   = fLineContentsList.end (),
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
        " : " << "[EMPTY]" <<
      endl;
    }
  }

  --gIndenter;
}

ostream& operator << (ostream& os, const S_bsrLine& elt)
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
