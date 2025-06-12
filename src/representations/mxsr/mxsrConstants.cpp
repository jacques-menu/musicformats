/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <string>

#include "mxsrConstants.h"


namespace MusicFormats
{

// note sequential numbers
//______________________________________________________________________________

const int
  K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE = 0;

const std::string K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING =
  "note_seq_num_0 (default_value)";

const std::string
  K_MXSR_NOTE_SEQUENTIAL_NUMBER_PREFIX (
    "note_seq_num_");

std::string mxsrNoteSequentialNumberAsString (int noteSequentialNumber)
{
  std::string result;

  if (noteSequentialNumber == K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE) {
    result = K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;
  }
  else {
    result = std::to_string (noteSequentialNumber);
  }

  return result;
}

// events sequential numbers
//______________________________________________________________________________

const int
  K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE = 0;

const std::string
  K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING (
    "event_seq_num_0 (default_value)");

const std::string
  K_MXSR_EVENT_SEQUENTIAL_NUMBER_PREFIX (
    "event_seq_num_");

std::string mfEventSequentialNumberAsString (int eventSequentialNumber)
{
  std::string result;

  if (eventSequentialNumber == K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE) {
    result = K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;
  }
  else {
    result = std::to_string (eventSequentialNumber);
  }

  return result;
}


}
