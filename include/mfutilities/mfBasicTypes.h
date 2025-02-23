/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfBasicTypes___
#define ___mfBasicTypes___

#include "exports.h"

#include "mfConstants.h"
#include "mfWrappedRange.h"
#include "mfWrappedValueForArithmetic.h"
#include "mfWrappedValueWithDefault.h"

// #include "msrNotesDurations.h"
// #include "msrMoments.h"


namespace MusicFormats
{

// input line numbers
//______________________________________________________________________________

using mfInputLineNumber =
  mfWrappedValueWithDefault <
    int,
    kInputLineNumberPrefix,
    K_MF_EMPTY_STRING,
    K_MF_INPUT_LINE_UNKNOWN_,
    kInputLineNumberDefaultValueString
  >;

// input location ranges
//______________________________________________________________________________

using mfInputLocationRange =
  mfWrappedRange <
    mfInputLineNumber,
    kInputLocationRangePrefix
  >;

// staff numbers
//______________________________________________________________________________

using mfStaffNumber =
  mfWrappedValueWithDefault <
    int,
    kStaffNumberPrefix,
    K_MF_EMPTY_STRING,
    K_STAFF_NUMBER_UNKNOWN_,
    kInputLineNumberDefaultValueString
  >;

// voice numbers
//______________________________________________________________________________

using mfVoiceNumber =
  mfWrappedValueWithDefault <
    int,
    kVoiceNumberPrefix,
    K_MF_EMPTY_STRING,
    K_VOICE_NUMBER_UNKNOWN_,
    kVoiceNumberDefaultValueString
  >;

// measure numbers
//______________________________________________________________________________

using mfMeasureNumber =
  mfWrappedValueWithDefault <
    std::string,
    kMeasureNumberPrefix,
    K_MF_EMPTY_STRING,
    K_MEASURE_NUMBER_UNKNOWN_,
    kMeasureNumberDefaultValueString
  >;

//______________________________________________________________________________
// whole notes durations
using mfWholeNotes =
  mfWrappedValueWithDefault <
    msrWholeNotes,
    kWholeNotePrefix,
    K_MF_EMPTY_STRING,
    K_WHOLE_NOTES_UNKNOWN_,
    kWholeNoteDefaultValueString
  >;

//______________________________________________________________________________
// moments
using mfMoment =
  mfWrappedValueWithDefault <
    msrMoment,
    kMomentPrefix,
    K_MF_EMPTY_STRING,
    K_MOMENT_UNKNOWN_,
    kMomentDefaultValueString
  >;

// lyrics
//______________________________________________________________________________

using mfStanzaNumber =
  mfWrappedValueWithDefault <
    std::string,
    kStanzaNumberPrefix,
    K_MF_EMPTY_STRING,
    K_STANZA_NUMBER_UNKNOWN_,
    kStanzaNumberDefaultValueString
  >;

//______________________________________________________________________________
void testMfBasicTypes ();


}


#endif // ___mfBasicTypes___
