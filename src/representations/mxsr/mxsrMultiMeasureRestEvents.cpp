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

#include "mxsrMultiMeasureRestEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrMultiMeasureRestEventKindAsString (
  mxsrMultiMeasureRestEventKind multiMeasureRestEventKind)
{
  std::string result;

  switch (multiMeasureRestEventKind) {
    case mxsrMultiMeasureRestEventKind::kMultiMeasureRestEvent_NONE:
      result = "kMultiMeasureRestEvent_NONE";
      break;
    case mxsrMultiMeasureRestEventKind::kMultiMeasureRestEventBegin:
      result = "kMultiMeasureRestEventBegin";
      break;
    case mxsrMultiMeasureRestEventKind::kMultiMeasureRestEventEnd:
      result = "kMultiMeasureRestEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrMultiMeasureRestEventKind& elt)
{
  os << mxsrMultiMeasureRestEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrMultiMeasureRestEvent mxsrMultiMeasureRestEvent::create (
  mxsrMultiMeasureRestEventKind multiMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multiMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrMultiMeasureRestEvent* obj =
    new mxsrMultiMeasureRestEvent (
      multiMeasureRestEventKind,
      partName,
      measureNumber,
      multiMeasureRestNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

// S_mxsrMultiMeasureRestEvent mxsrMultiMeasureRestEvent::createAMultiMeasureRestEnd (
//   const std::string&       partName,
//   const mfMeasureNumber&   measureNumber,
//   int                      multiMeasureRestNumber,
//   const mfInputLineNumber& eventInputLineNumber)
// {
//   ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand
//
//   S_mxsrMultiMeasureRestEvent
//     multiMeasureRestEndEvent =
//       mxsrMultiMeasureRestEvent::create (
//         mxsrMultiMeasureRestEventKind::kMultiMeasureRestEventEnd,
//         partName,
//         measureNumber,
//         multiMeasureRestNumber,
//         fCurrentEventSequentialNumber,
//         eventInputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultiMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering multi-measure rest end event " <<
//       multiMeasureRestEndEvent->asString () <<
//       ", line " << eventInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return multiMeasureRestEndEvent;
// }

mxsrMultiMeasureRestEvent::mxsrMultiMeasureRestEvent (
  mxsrMultiMeasureRestEventKind multiMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multiMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrPartEvent (
      partName,
      measureNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  fMultiMeasureRestEventKind = multiMeasureRestEventKind;

  fMultiMeasureRestNumber = multiMeasureRestNumber;
}

mxsrMultiMeasureRestEvent::~mxsrMultiMeasureRestEvent ()
{}

std::string mxsrMultiMeasureRestEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MultiMeasureRestEvent" <<
    ", fMultiMeasureRestEventKind: " << fMultiMeasureRestEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<
    ", fMultiMeasureRestNumber: " << fMultiMeasureRestNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrMultiMeasureRestEvent::asString () const
{
  return asShortString ();
}

void mxsrMultiMeasureRestEvent::print (std::ostream& os) const
{
  os <<
    "[MultiMeasureRestEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultiMeasureRestEventKind" << ": " << fMultiMeasureRestEventKind <<
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
    "fMultiMeasureRestNumber" << ": " << fMultiMeasureRestNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrMultiMeasureRestEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrMultiMeasureRestEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

