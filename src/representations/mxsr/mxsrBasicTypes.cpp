/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mxsrBasicTypes.h"

#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

//______________________________________________________________________________
// notes sequential numbers

std::string mxsrNoteSequentialNumberAsString (
  const mxsrNoteSequentialNumber& noteSequentialNumber)
{
  std::string result;

  if (noteSequentialNumber == K_MXSR_NOTE_SEQUENTIAL_NUMBER_UNKNOWN_) {
    result = "K_MXSR_NOTE_SEQUENTIAL_NUMBER_UNKNOWN_";
  }
  else {
#ifndef MF_USE_WRAPPED_TYPES
    result = noteSequentialNumber;
#else
    result = std::to_string (noteSequentialNumber.getBareValue ());
#endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

int mxsrNoteSequentialNumberAsInteger (
  const mxsrNoteSequentialNumber& noteSequentialNumber)
{
  int result;

  if (noteSequentialNumber == K_MXSR_NOTE_SEQUENTIAL_NUMBER_UNKNOWN_) {
    result = K_MXSR_NOTE_SEQUENTIAL_NUMBER_UNKNOWN_;
  }
  else {
#ifndef MF_USE_WRAPPED_TYPES
    result = noteSequentialNumber;
#else
    result = noteSequentialNumber.getBareValue ();
#endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// MXSR events sequential numbers

std::string mxsrEventSequentialNumberAsString (
  const mxsrNoteSequentialNumber& noteSequentialNumber)
{
  std::string result;

  if (noteSequentialNumber == K_MXSR_EVENTS_SEQUENTIAL_NUMBER_UNKNOWN_) {
    result = "K_MXSR_EVENTS_SEQUENTIAL_NUMBER_UNKNOWN_";
  }
  else {
#ifndef MF_USE_WRAPPED_TYPES
    result = noteSequentialNumber;
#else
    result = std::to_string (noteSequentialNumber.getBareValue ());
#endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

int mxsrEventSequentialNumberAsInteger (
  const mxsrNoteSequentialNumber& noteSequentialNumber)
{
  int result;

  if (noteSequentialNumber == K_MXSR_EVENTS_SEQUENTIAL_NUMBER_UNKNOWN_) {
    result = K_MXSR_EVENTS_SEQUENTIAL_NUMBER_UNKNOWN_;
  }
  else {
#ifndef MF_USE_WRAPPED_TYPES
    result = noteSequentialNumber;
#else
    result = noteSequentialNumber.getBareValue ();
#endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}



}
