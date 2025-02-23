/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrConstants___
#define ___mxsrConstants___

#include <string>

#include "exports.h"


namespace MusicFormats
{

// notes sequential numbers
//______________________________________________________________________________

const int K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE = 0;

EXP extern const std::string K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  kNoteSequentialNumberPrefix,
  kNoteSequentialNumberDefaultValueString;

// events sequential numbers
//______________________________________________________________________________

const int K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE = 0;

EXP extern const std::string K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  kEventSequentialNumberPrefix,
  kEventSequentialNumberDefaultValueString;

// tuplets numbers
//______________________________________________________________________________

const int K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE = 0;

EXP extern const std::string K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  kTupletNumberPrefix,
  kTupletNumberDefaultValueString;


}


#endif // ___mxsrConstants___
