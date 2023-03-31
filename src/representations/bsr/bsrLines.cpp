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

#include "mfStaticSettings.h"
#include "mfStringsHandling.h"

#include "bsrLines.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrLine bsrLine::create (
  int inputLineNumber,
  int printLineNumber,
  int cellsPerLine)
{
  bsrLine* obj =
    new bsrLine (
      inputLineNumber, printLineNumber, cellsPerLine);
  assert (obj != nullptr);
  return obj;
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

#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceLines ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

bsrLine::~bsrLine ()
{}

S_bsrLine bsrLine::createLineNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceLines ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of line " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceSpaces () || gBsrOahGroup->getTraceLines ()) {
    gLog <<
      "Appending spaces '" <<
      spaces->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToLine (spaces);
}

void bsrLine::appendKeyToLine (S_bsrKey key)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Appending key " <<
      key->asShortString () <<
      "' to line '" <<
      asString () <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToLine (key);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTimeSignatureToLine (S_bsrTimeSignature timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature '" <<
      timeSignature->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToLine (timeSignature);
  fASpaceIsNeededInLine = true;
}

void bsrLine::insertTimeBeforeLastElementOfLine (S_bsrTimeSignature timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Inserting time signature '" <<
      timeSignature->asShortString () <<
      "' before the last element of line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  insertElementBeforeLastElementOfLine (timeSignature);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendTempoToLine (S_bsrTempo tempo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to BSR line " <<
      asString () <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToLine (tempo);
  fASpaceIsNeededInLine = true;
}

void bsrLine::appendMeasureToLine (S_bsrMeasure measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures () || gBsrOahGroup->getTraceLines ()) {
    gLog <<
      "Appending measure '" <<
      measure->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending note '" <<
      note->asShortString () <<
      "' to line '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLine::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLine::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrLine::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLine::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLine>*
    p =
      dynamic_cast<visitor<S_bsrLine>*> (v)) {
        S_bsrLine elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLine::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "Line" <<
    ", printLineNumber" << ": " << fPrintLineNumber <<
    ", brailleLineNumber" << ": " << fBrailleLineNumber <<
    ", cellsPerLine" << ": " << fCellsPerLine <<
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << ": " << fetchCellsNumber () <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrLine::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "L" <<
    ", printLineNumber" << ": " << fPrintLineNumber <<
    ", brailleLineNumber" << ": " << fBrailleLineNumber <<
    ", cellsPerLine" << ": " << fCellsPerLine <<
    ", lineContents: " << fLineContentsList.size () <<
    ", cellsNumber" << ": " << fetchCellsNumber ();

  return ss.str ();
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
    "printLineNumber" << ": " << fPrintLineNumber <<
    std::endl <<
    */
    std::setw (fieldWidth) <<
    "brailleLineNumber" << ": " << fBrailleLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "cellsPerLine" << ": " << fCellsPerLine <<
    std::endl <<
    std::setw (fieldWidth) <<
    "lineNumberCellsList" << ": " << buildLineNumberCellsList () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "cellsNumber" << ": " << fetchCellsNumber () <<
    std::endl;
  os << std::endl;

  // print the line contents if any
  if (lineContentsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
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
        ": " << "[EMPTY]" <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
