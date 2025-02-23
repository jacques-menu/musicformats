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

#include "mfConstants.h"
#include "mfWrappedValueForArithmetic.h"

#include "mxsrConstants.h"


namespace MusicFormats
{

// notes sequential numbers
//______________________________________________________________________________

using mxsrNoteSequentialNumber =
  mfWrappedValueForArithmetic <
    int,
    kNoteSequentialNumberPrefix,
    K_MF_EMPTY_STRING,
    K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE,
    K_MXSR_NOTE_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING
  >;

// events sequential numbers
//______________________________________________________________________________

using mxsrEventSequentialNumber =
  mfWrappedValueForArithmetic <
    int,
    kEventSequentialNumberPrefix,
    K_MF_EMPTY_STRING,
    K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE,
    K_MXSR_EVENT_SEQUENTIAL_NUMBER_DEFAULT_VALUE_STRING
  >;

// tuplets numbers
//______________________________________________________________________________

using mxsrTupletNumber =
  mfWrappedValueForArithmetic <
    int,
    kTupletNumberPrefix,
    K_MF_EMPTY_STRING,
    K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE,
    K_MXSR_TUPLET_NUMBER_DEFAULT_VALUE_STRING
  >;


}


#endif // ___mxsrBasicTypes___
