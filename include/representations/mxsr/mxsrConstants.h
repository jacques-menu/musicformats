/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

EXP extern const int K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE;

EXP extern const std::string K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  K_MXSR_NOTE_SEQUENTIAL_NUMBER_PREFIX,
  K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

// events sequential numbers
//______________________________________________________________________________

EXP extern const int K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE;

EXP extern const std::string K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  K_MXSR_EVENT_SEQUENTIAL_NUMBER_PREFIX,
  K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING;

// tuplets numbers
//______________________________________________________________________________

EXP extern const int K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE;

EXP extern const std::string K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING;

EXP extern const std::string
  K_MXSR_TUPLET_NUMBER_PREFIX,
  K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING;


}


#endif // ___mxsrConstants___
