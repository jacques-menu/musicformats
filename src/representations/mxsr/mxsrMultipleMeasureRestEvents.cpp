/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mfIndentedTextOutput.h"

#include "mxsrMultipleMeasureRestEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrMultipleMeasureRestEventKindAsString (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind)
{
  std::string result;

  switch (multipleMeasureRestEventKind) {
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEvent_NONE:
      result = "kMultipleMeasureRestEvent_NONE";
      break;
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventBegin:
      result = "kMultipleMeasureRestEventBegin";
      break;
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd:
      result = "kMultipleMeasureRestEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEventKind& elt)
{
  os << mxsrMultipleMeasureRestEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrMultipleMeasureRestEvent mxsrMultipleMeasureRestEvent::create (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multipleMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrMultipleMeasureRestEvent* obj =
    new mxsrMultipleMeasureRestEvent (
      multipleMeasureRestEventKind,
      partName,
      measureNumber,
      multipleMeasureRestNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

// S_mxsrMultipleMeasureRestEvent mxsrMultipleMeasureRestEvent::createAMultipleMeasureRestEnd (
//   const std::string&       partName,
//   const mfMeasureNumber&   measureNumber,
//   int                      multipleMeasureRestNumber,
//   const mfInputLineNumber& eventInputLineNumber)
// {
//   ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand
//
//   S_mxsrMultipleMeasureRestEvent
//     multipleMeasureRestEndEvent =
//       mxsrMultipleMeasureRestEvent::create (
//         mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd,
//         partName,
//         measureNumber,
//         multipleMeasureRestNumber,
//         fCurrentEventSequentialNumber,
//         eventInputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering multiple measure rest end event " <<
//       multipleMeasureRestEndEvent->asString () <<
//       ", line " << eventInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return multipleMeasureRestEndEvent;
// }

mxsrMultipleMeasureRestEvent::mxsrMultipleMeasureRestEvent (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multipleMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrPartEvent (
      partName,
      measureNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  fMultipleMeasureRestEventKind = multipleMeasureRestEventKind;

  fMultipleMeasureRestNumber = multipleMeasureRestNumber;
}

mxsrMultipleMeasureRestEvent::~mxsrMultipleMeasureRestEvent ()
{}

std::string mxsrMultipleMeasureRestEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MultipleMeasureRestEvent" <<
    ", fMultipleMeasureRestEventKind: " << fMultipleMeasureRestEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<
    ", fMultipleMeasureRestNumber: " << fMultipleMeasureRestNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrMultipleMeasureRestEvent::asString () const
{
  return asShortString ();
}

void mxsrMultipleMeasureRestEvent::print (std::ostream& os) const
{
  os <<
    "[MultipleMeasureRestEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestEventKind" << ": " << fMultipleMeasureRestEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<

    std::endl <<
    std::setw (fieldWidth) <<
    "fPartName" << ": " << fPartName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureNumber" << ": " << fMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestNumber" << ": " << fMultipleMeasureRestNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrMultipleMeasureRestEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

