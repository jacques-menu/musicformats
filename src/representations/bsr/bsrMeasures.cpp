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

#include "bsrMeasures.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrMeasure bsrMeasure::create (
  int           inputLineNumber,
  const std::string& printMeasureNumber)
{
  bsrMeasure* obj =
    new bsrMeasure (
      inputLineNumber, printMeasureNumber);
  assert (obj != nullptr);
  return obj;
}

bsrMeasure::bsrMeasure (
  int           inputLineNumber,
  const std::string& printMeasureNumber)
    : bsrLineContentsElement (inputLineNumber)
{
  fPrintMeasureNumber = printMeasureNumber;

  // initially, fBrailleMeasureNumber is the same as fPrintMeasureNumber
  fBrailleMeasureNumber = fPrintMeasureNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrMeasure '" <<
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

bsrMeasure::~bsrMeasure ()
{}

S_bsrMeasure bsrMeasure::createMeasureNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of measure " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Appending clef " <<
      clef->asShortString () <<
      " to measure " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToMeasure (clef);
}

void bsrMeasure::appendBarLineToMeasure (S_bsrBarLine barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " <<
      barLine->asShortString () <<
      " to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToMeasure (barLine);
}

void bsrMeasure::appendNumberToMeasure (S_bsrNumber number)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceNumbers ()) {
    gLog <<
      "Appending number '" <<
      number->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToMeasure (number);
}

void bsrMeasure::appendWordsToMeasure (S_bsrWords words)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceNumbers ()) {
    gLog <<
      "Appending words '" <<
      words->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToMeasure (words);
}

void bsrMeasure::appendNoteToMeasure (S_bsrNote note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending note '" <<
      note->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  appendLineElementToMeasure (note);
}

void bsrMeasure::appendDynamicToMeasure (S_bsrDynamic dynamic)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Appending dynamic '" <<
      dynamic->asShortString () <<
      "' to measure '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrMeasure::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrMeasure::visitStart ()" <<
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

void bsrMeasure::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrMeasure::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrMeasure>*
    p =
      dynamic_cast<visitor<S_bsrMeasure>*> (v)) {
        S_bsrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrMeasure::visitEnd ()" <<
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
  std::stringstream ss;

  ss <<
    "Measure" <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", printMeasureNumber: " << fPrintMeasureNumber <<
    ", brailleMeasureNumber: " << fBrailleMeasureNumber <<
    ", measureElementsList.size (): " << fMeasureLineElementsList.size () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrMeasure::asDebugString () const // JMI
{
  std::stringstream ss;

  ss << "{";

  size_t measureElementsListSize = fMeasureLineElementsList.size ();

  if (measureElementsListSize) {
    std::list<S_bsrLineContentsElement>::const_iterator
      iBegin = fMeasureLineElementsList.begin (),
      iEnd   = fMeasureLineElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i)->asDebugString ();
      if (++i == iEnd) break;
      ss << "|";
    } // for
  }

  ss << '}';

  return ss.str ();
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

  if (measureElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
