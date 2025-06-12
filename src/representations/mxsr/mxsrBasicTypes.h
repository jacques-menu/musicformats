/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrBasicTypes___
#define ___mxsrBasicTypes___

#include "exports.h"

#include "mfWrappedValueForArithmetic.h"

#include "mxsrConstants.h"


namespace MusicFormats
{

//______________________________________________________________________________
// notes sequential numbers

constexpr int K_MXSR_NOTE_SEQUENTIAL_NUMBER_UNKNOWN_ = -1;

#ifndef MF_USE_WRAPPED_TYPES
  using mxsrNoteSequentialNumber = int;
#else
  using mxsrNoteSequentialNumber =
    mfWrappedValueForArithmetic <
      int,
      K_MXSR_NOTE_SEQUENTIAL_NUMBER_PREFIX,
      K_MF_EMPTY_STRING,
      K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE,
      K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mxsrNoteSequentialNumberAsString (
  const mxsrNoteSequentialNumber& noteSequentialNumber);

EXP extern int mxsrNoteSequentialNumberAsInteger (
  const mxsrNoteSequentialNumber& noteSequentialNumber);

//______________________________________________________________________________
// MXSR events sequential numbers

constexpr int K_MXSR_EVENTS_SEQUENTIAL_NUMBER_UNKNOWN_ = -1;

#ifndef MF_USE_WRAPPED_TYPES
  using mxsrEventSequentialNumber = int;
#else
  using mxsrEventSequentialNumber =
    mfWrappedValueForArithmetic <
      int,
      K_MXSR_EVENT_SEQUENTIAL_NUMBER_PREFIX,
      K_MF_EMPTY_STRING,
      K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE,
      K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mxsrEventSequentialNumberAsString (
  const mxsrEventSequentialNumber& eventSequentialNumber);

EXP extern int mxsrEventSequentialNumberAsInteger (
  const mxsrEventSequentialNumber& eventSequentialNumber);


}


#endif // ___mxsrBasicTypes___
