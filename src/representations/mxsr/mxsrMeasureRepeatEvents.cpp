/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mxsrMeasureRepeatEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrMeasureRepeatEventKindAsString (
  mxsrMeasureRepeatEventKind measureRepeatEventKind)
{
  std::string result;

  switch (measureRepeatEventKind) {
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEvent_NONE:
      result = "kMeasureRepeatEvent_NONE";
      break;
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEventBegin:
      result = "kMeasureRepeatEventBegin";
      break;
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEventEnd:
      result = "kMeasureRepeatEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEventKind& elt)
{
  os << mxsrMeasureRepeatEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrMeasureRepeatEvent mxsrMeasureRepeatEvent::create (
  mxsrMeasureRepeatEventKind       measureRepeatEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              measureRepeatNumber,
  int                              measureRepeatSlashes,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrMeasureRepeatEvent* obj =
    new mxsrMeasureRepeatEvent (
      measureRepeatEventKind,
      partName,
      measureNumber,
      measureRepeatNumber,
      measureRepeatSlashes,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrMeasureRepeatEvent::mxsrMeasureRepeatEvent (
  mxsrMeasureRepeatEventKind       measureRepeatEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              measureRepeatNumber,
  int                              measureRepeatSlashes,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrPartEvent (
      partName,
      measureNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  fMeasureRepeatEventKind = measureRepeatEventKind;

  fMeasureRepeatSlashes = measureRepeatSlashes;
}

mxsrMeasureRepeatEvent::~mxsrMeasureRepeatEvent ()
{}

std::string mxsrMeasureRepeatEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MeasureRepeatEvent" <<
    ", fMeasureRepeatEventKind: " << fMeasureRepeatEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<
    ", fMeasureRepeatSlashes: " << fMeasureRepeatSlashes <<

    ", fEventSequentialNumber: event_" << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrMeasureRepeatEvent::asString () const
{
  return asShortString ();
}

void mxsrMeasureRepeatEvent::print (std::ostream& os) const
{
  os <<
    "[MeasureRepeatEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatEventKind" << ": " << fMeasureRepeatEventKind <<
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
    "fMeasureRepeatSlashes" << ": " << fMeasureRepeatSlashes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrMeasureRepeatEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

