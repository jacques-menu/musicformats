/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfConstants___
#define ___mfConstants___

#include <string>

#include "exports.h"


namespace MusicFormats
{

// input line numbers
//______________________________________________________________________________

EXP extern const int K_MF_INPUT_LINE_UNKNOWN_;

// staff numbers
//______________________________________________________________________________

EXP extern const int K_STAFF_NUMBER_UNKNOWN_;

// voice numbers
//______________________________________________________________________________

EXP extern const int K_VOICE_NUMBER_UNKNOWN_;

EXP extern const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER;
EXP extern const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

// voice numbers and positions
//______________________________________________________________________________
extern const std::string K_MEASURE_NUMBER_UNKNOWN_;

// names lists max length
//______________________________________________________________________________

EXP extern const int K_MF_NAMES_LIST_MAX_LENGTH;


}


#endif // ___mfConstants___
