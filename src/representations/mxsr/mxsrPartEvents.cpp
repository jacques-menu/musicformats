/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mxsrPartEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
/* this class is purely virtual
mxsr-Event mxsrPartEvent::create (
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrPartEvent* obj =
    new mxsrPartEvent (
      eventSequentialNumber,
      partName,
      measureNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrPartEvent::mxsrPartEvent (
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrEvent (
      eventSequentialNumber,
      eventInputLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;

  fPartName = partName;
  fMeasureNumber = measureNumber;
}

mxsrPartEvent::~mxsrPartEvent ()
{}


} // namespace

