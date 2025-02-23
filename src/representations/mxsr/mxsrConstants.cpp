/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

const std::string K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING =
  "note 0 (default_value)";

const std::string
  kNoteSequentialNumberPrefix             ("note_seq_num_"),
  kNoteSequentialNumberDefaultValueString ("note 0 *default_value*");

std::string mfNoteSequentialNumberAsString (int noteSequentialNumber)
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

const std::string K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING =
  "event 0 (default_value)";

const std::string
  kEventSequentialNumberPrefix             ("event_seq_num_"),
  kEventSequentialNumberDefaultValueString ("event 0 *default_value*");

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

// tuplets numbers
//______________________________________________________________________________

const std::string K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING =
  "tuplet 0 (default_value)";

const std::string
  kTupletNumberPrefix             ("tuplet__seq_num_"),
  kTupletNumberDefaultValueString ("tuplet 0 *default_value*");

std::string mfTupletSequentialNumberAsString (int tupletSequentialNumber)
{
  std::string result;

  if (tupletSequentialNumber == K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE) {
    result = K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING;
  }
  else {
    result = std::to_string (tupletSequentialNumber);
  }

  return result;
}


}
