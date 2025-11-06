/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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


namespace MusicFormats
{

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//                         WRAPPED TYPES USE
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// use wrapped types?
//______________________________________________________________________________

/*
  MF_USE_WRAPPED_TYPES is used temporarily
  to switch the use of these types on and off easily

  This is placed here because only some source files use this feature, JMI 0.9.75
  which minimizes re-compilations should the setting change
*/

#define MF_USE_WRAPPED_TYPES


//______________________________________________________________________________
// input line numbers

EXP extern const int K_MF_INPUT_LINE_UNKNOWN_;

EXP extern const std::string
  kMfInputLineNumberPrefix,
  kMfInputLineNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfInputLineNumber = int;
// #else
  using mfInputLineNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfInputLineNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_INPUT_LINE_UNKNOWN_,
      kMfInputLineNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfInputLineNumberAsString (
  const mfInputLineNumber& inputLineNumber);

EXP extern int mfInputLineNumberAsInteger (
  const mfInputLineNumber& inputLineNumber);

//______________________________________________________________________________
// input location ranges

EXP extern const std::string
  kMfInputLocationRangePrefix,
  kMfInputLocationDefaultValueString;

// // #ifndef MF_USE_WRAPPED_TYPES
//   using mfInputLocationRange = int;
// #else
//   using mfInputLocationRange =
//     mfWrappedRange <
//       mfInputLineNumber,
//       kMfInputLocationRangePrefix
//     >;
// // #endif // MF_USE_WRAPPED_TYPES

//______________________________________________________________________________
// staff numbers

EXP extern const int K_MF_STAFF_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfStaffNumberPrefix,
  kMfStaffNumberDefaultValueString;

EXP extern const int K_MF_PART_HARMONIES_STAFF_NUMBER;

EXP extern const int K_MF_PART_FIGURED_BASS_STAFF_NUMBER;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfStaffNumber = int;
// #else
  using mfStaffNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfStaffNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_STAFF_NUMBER_UNKNOWN_,
      kMfInputLineNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfStaffNumberAsString (
  const mfStaffNumber& staffNumber);

EXP extern int mfStaffNumberAsInteger (
  const mfStaffNumber& staffNumber);

//______________________________________________________________________________
// voice numbers

EXP extern const int K_MF_VOICE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfVoiceNumberPrefix,
  kMfVoiceNumberDefaultValueString;

EXP extern const int K_MF_PART_HARMONIES_VOICE_NUMBER;
EXP extern const int K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER;

EXP extern const int K_MF_PART_FIGURED_BASS_VOICE_NUMBER;
EXP extern const int K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfVoiceNumber = int;
// #else
  using mfVoiceNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfVoiceNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_VOICE_NUMBER_UNKNOWN_,
      kMfVoiceNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfVoiceNumberAsString (
  const mfVoiceNumber& voiceNumber);

EXP extern int mfVoiceNumberAsInteger (
  const mfVoiceNumber& voiceNumber);

//______________________________________________________________________________
// segment numbers

EXP extern const int K_MF_SEGMENT_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfSegmentNumberPrefix,
  kMfSegmentNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfSegmentNumber = std::string;
// #else
  using mfSegmentNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfSegmentNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_SEGMENT_NUMBER_UNKNOWN_,
      kMfSegmentNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfSegmentNumberAsString (
  const mfSegmentNumber& segmentNumber);

//______________________________________________________________________________
// measure numbers

EXP extern const std::string K_MF_MEASURE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfMeasureNumberPrefix,
  kMfMeasureNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfMeasureNumber = std::string;
// #else
  using mfMeasureNumber =
    mfWrappedValueWithDefault <
      std::string,
      kMfMeasureNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_MEASURE_NUMBER_UNKNOWN_,
      kMfMeasureNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfMeasureNumberAsString (
  const mfMeasureNumber& measureNumber);


// measure debug numbers

EXP extern const int K_MF_MEASURE_DEBUG_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfMeasureDebugNumberPrefix,
  kMfMeasureDebugNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfMeasureDebugNumber = std::string;
// #else
  using mfMeasureDebugNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfMeasureDebugNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_MEASURE_DEBUG_NUMBER_UNKNOWN_,
      kMfMeasureDebugNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfMeasureDebugNumberAsString (
  const mfMeasureDebugNumber& measureDebugNumber);

//______________________________________________________________________________
// repeat elements debug numbers

EXP extern const int K_MF_REPEAT_ELEMENT_DEBUG_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfRepeatElementDebugNumberPrefix,
  kMfRepeatElementDebugNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfRepeatElementDebugNumber = std::string;
// #else
  using mfRepeatElementDebugNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfRepeatElementDebugNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_REPEAT_ELEMENT_DEBUG_NUMBER_UNKNOWN_,
      kMfRepeatElementDebugNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfRepeatElementDebugNumberAsString (
  const mfRepeatElementDebugNumber& repeatElementDebugNumber);

// repeat debug numbers

EXP extern const int K_MF_REPEAT_ELEMENT_DEBUG_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfRepeatDebugNumberPrefix,
  kMfRepeatDebugNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfRepeatDebugNumber = std::string;
// #else
  using mfRepeatDebugNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfRepeatDebugNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_REPEAT_ELEMENT_DEBUG_NUMBER_UNKNOWN_,
      kMfRepeatDebugNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfRepeatDebugNumberAsString (
  const mfRepeatDebugNumber& repeatDebugNumber);

//______________________________________________________________________________
// tuplet numbers

EXP extern const int K_MF_TUPLET_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMfTupletNumberPrefix,
  kMfTupletNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfTupletNumber = int;
// #else
  using mfTupletNumber =
    mfWrappedValueForArithmetic <
      int,
      kMfTupletNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_TUPLET_NUMBER_UNKNOWN_,
      kMfTupletNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfTupletNumberAsString (
  const mfTupletNumber& tupletNumber);

EXP extern int mfTupletNumberAsInteger (
  const mfTupletNumber& tupletNumber);

//______________________________________________________________________________
// lyrics

EXP extern const std::string
  K_MF_STANZA_NUMBER_UNKNOWN_,
  K_MF_STANZA_NAME_UNKNOWN_;

EXP extern const std::string
  kMfStanzaNumberPrefix,
  kMfStanzaNumberDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfStanzaNumber = std::string;
// #else
  using mfStanzaNumber =
    mfWrappedValueWithDefault <
      std::string,
      kMfStanzaNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_STANZA_NUMBER_UNKNOWN_,
      kMfStanzaNumberDefaultValueString
    >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfStanzaNumberAsString (
  const mfStanzaNumber& stanzaNumber);

//______________________________________________________________________________

void testMfBasicTypes ();


}


#endif // ___mfBasicTypes___
