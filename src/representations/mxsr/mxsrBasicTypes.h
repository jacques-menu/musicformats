/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrBasicTypes___
#define ___mxsrBasicTypes___

#include "exports.h"

// #include "mfBasicTypes.h"
#include "mfConstants.h"
#include "mfWrappedValueForArithmetic.h"

#include "mxsrConstants.h"


namespace MusicFormats
{

// notes sequential numbers
//______________________________________________________________________________

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


// events sequential numbers
//______________________________________________________________________________

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


// tuplets numbers
//______________________________________________________________________________

// uncomment the following line for short-term tests
// with only tuplet numbers being wrapped (they're not too frequent in the code)

#undef MF_USE_WRAPPED_TYPES

#ifndef MF_USE_WRAPPED_TYPES
  using mxsrTupletNumber = int;
#else
  using mxsrTupletNumber =
    mfWrappedValueForArithmetic <
      int,
      K_MXSR_TUPLET_NUMBER_PREFIX,
      K_MF_EMPTY_STRING,
      K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE,
      K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING
    >;
#endif // MF_USE_WRAPPED_TYPES


}


#endif // ___mxsrBasicTypes___
