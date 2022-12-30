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

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "bsrMeasures.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrMeasure bsrMeasure::create (
  int           inputLineNumber,
  const std::string& printMeasureNumber)
{
  bsrMeasure* o =
    new bsrMeasure (
      inputLineNumber, printMeasureNumber);
  assert (o != nullptr);
  return o;
}

bsrMeasure::bsrMeasure (
  int           inputLineNumber,
  const std::string& printMeasureNumber)
    : bsrLineContentsElement (inputLineNumber)
{
  fPrintMeasureNumber = printMeasureNumber;

  // initially, fBrailleMeasureNumber is the same as fPrintMeasureNumber
  fBrailleMeasureNumber = fPrintMeasureNumber;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating bsrMeasure '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;
  }
#endif
}

bsrMeasure::~bsrMeasure ()
{}

S_bsrMeasure bsrMeasure::createMeasureNewbornClone ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a newborn clone of measure " <<
      asString () <<
      std::endl;
  }
#endif

  S_bsrMeasure
    newbornClone =
      bsrMeasure::create (
        fInputLineNumber,
        fPrintMeasureNumber);

  // braille measure number
  newbornClone->fBrailleMeasureNumber =
    fBrailleMeasureNumber;

  return newbornClone;
}

void bsrMeasure::appendLineElementToMeasure (
  const S_bsrLineContentsElement& lineElement)
{
  fMeasureLineElementsList.push_back (lineElement);
}

void bsrMeasure::appendClefToMeasure (S_bsrClef clef)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" <<
      clef->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (clef);
}

void bsrMeasure::appendBarLineToMeasure (S_bsrBarLine barLine)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " <<
      barLine->asShortString () <<
      " to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (barLine);
}

void bsrMeasure::appendNumberToMeasure (S_bsrNumber number)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceNumbers ()) {
    gLogStream <<
      "Appending number '" <<
      number->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (number);
}

void bsrMeasure::appendWordsToMeasure (S_bsrWords words)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceNumbers ()) {
    gLogStream <<
      "Appending words '" <<
      words->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (words);
}

void bsrMeasure::appendNoteToMeasure (S_bsrNote note)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (note);
}

void bsrMeasure::appendDynamicToMeasure (S_bsrDynamic dynamic)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "Appending dynamic '" <<
      dynamic->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  appendLineElementToMeasure (dynamic);
}

S_bsrCellsList bsrMeasure::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  for (
    std::list<S_bsrLineContentsElement>::const_iterator i = fMeasureLineElementsList.begin ();
    i != fMeasureLineElementsList.end ();
    ++i ) {
    // append the braille for the element
    result->
      appendCellsListToCellsList (
        (*i)->fetchCellsList ());
  } // for

  return result;
}

int bsrMeasure::fetchCellsNumber () const
{
  return buildCellsList ()->fetchCellsNumber ();
}

void bsrMeasure::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrMeasure::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrMeasure::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrMeasure::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrMeasure::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrMeasure::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrMeasure::browseData (basevisitor* v)
{
  for (
    std::list<S_bsrLineContentsElement>::const_iterator i = fMeasureLineElementsList.begin ();
    i != fMeasureLineElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string bsrMeasure::asString () const
{
  std::stringstream s;

  s <<
    "Measure" <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", brailleMeasureNumber: " << fBrailleMeasureNumber <<
    ", measureElementsList.size (): " << fMeasureLineElementsList.size () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrMeasure::asDebugString () const // JMI
{
  std::stringstream s;

  s << "{";

  size_t measureElementsListSize = fMeasureLineElementsList.size ();

  if (measureElementsListSize) {
    std::list<S_bsrLineContentsElement>::const_iterator
      iBegin = fMeasureLineElementsList.begin (),
      iEnd   = fMeasureLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asDebugString ();
      if (++i == iEnd) break;
      s << "|";
    } // for
  }

  s << '}';

  return s.str ();
}

void bsrMeasure::print (std::ostream& os) const
{
  os <<
    "Measure" <<
    ", printMeasureNumber " << fPrintMeasureNumber <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 21;

  // print the measure numbers
  os << std::left <<
    std::setw (fieldWidth) <<
    "printMeasureNumber" << ": " << fPrintMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "brailleMeasureNumber" << ": " << fBrailleMeasureNumber <<
    std::endl;

  // print the number of cells
  os << std::left <<
    std::setw (fieldWidth) <<
    "cellsNumber" << ": " << fetchCellsNumber () <<
    std::endl;

  // print spaces needs
  os << std::left <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " <<
    fSpacesBefore <<
    std::endl;

  os << std::endl;

  // print the measure elements if any
  size_t measureElementsListSize = fMeasureLineElementsList.size ();

  if (measureElementsListSize || gGlobalBsrOahGroup->getDisplayBsrFull ()) {
    os <<
//      std::setw (fieldWidth) <<
      "MeasureElementsList" <<
      ", " <<
      mfSingularOrPlural (
        measureElementsListSize, "element", "elements");
    if (measureElementsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrLineContentsElement>::const_iterator
        iBegin = fMeasureLineElementsList.begin (),
        iEnd   = fMeasureLineElementsList.end (),
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

std::ostream& operator << (std::ostream& os, const S_bsrMeasure& elt)
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
