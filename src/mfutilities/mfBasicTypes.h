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
  kInputLineNumberPrefix,
  kInputLineNumberDefaultValueString;

#ifndef MF_USE_WRAPPED_TYPES
  using mfInputLineNumber = int;
#else
  using mfInputLineNumber =
    mfWrappedValueForArithmetic <
      int,
      kInputLineNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MF_INPUT_LINE_UNKNOWN_,
      kInputLineNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfInputLineNumberAsString (
  const mfInputLineNumber& inputLineNumber);

EXP extern int mfInputLineNumberAsInteger (
  const mfInputLineNumber& inputLineNumber);

//______________________________________________________________________________
// input location ranges

EXP extern const std::string
  kInputLocationRangePrefix,
  kInputLocationDefaultValueString;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfInputLocationRange = int;
// #else
//   using mfInputLocationRange =
//     mfWrappedRange <
//       mfInputLineNumber,
//       kInputLocationRangePrefix
//     >;
// #endif // MF_USE_WRAPPED_TYPES

//______________________________________________________________________________
// staff numbers

EXP extern const int K_STAFF_NUMBER_UNKNOWN_;

EXP extern const std::string
  kStaffNumberPrefix,
  kStaffNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_STAFF_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_STAFF_NUMBER;

#ifndef MF_USE_WRAPPED_TYPES
  using mfStaffNumber = int;
#else
  using mfStaffNumber =
    mfWrappedValueForArithmetic <
      int,
      kStaffNumberPrefix,
      K_MF_EMPTY_STRING,
      K_STAFF_NUMBER_UNKNOWN_,
      kInputLineNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfStaffNumberAsString (
  const mfStaffNumber& staffNumber);

EXP extern int mfStaffNumberAsInteger (
  const mfStaffNumber& staffNumber);

//______________________________________________________________________________
// voice numbers

EXP extern const int K_VOICE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kVoiceNumberPrefix,
  kVoiceNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_VOICE_NUMBER;
EXP extern const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_VOICE_NUMBER;
EXP extern const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

#ifndef MF_USE_WRAPPED_TYPES
  using mfVoiceNumber = int;
#else
  using mfVoiceNumber =
    mfWrappedValueForArithmetic <
      int,
      kVoiceNumberPrefix,
      K_MF_EMPTY_STRING,
      K_VOICE_NUMBER_UNKNOWN_,
      kVoiceNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfVoiceNumberAsString (
  const mfVoiceNumber& voiceNumber);

EXP extern int mfVoiceNumberAsInteger (
  const mfVoiceNumber& voiceNumber);

//______________________________________________________________________________
// measure numbers

EXP extern const std::string K_MEASURE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMeasureNumberPrefix,
  kMeasureNumberDefaultValueString;

#ifndef MF_USE_WRAPPED_TYPES
  using mfMeasureNumber = std::string;
#else
  using mfMeasureNumber =
    mfWrappedValueWithDefault <
      std::string,
      kMeasureNumberPrefix,
      K_MF_EMPTY_STRING,
      K_MEASURE_NUMBER_UNKNOWN_,
      kMeasureNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfMeasureNumberAsString (
  const mfMeasureNumber& measureNumber);

//______________________________________________________________________________
// tuplet numbers

EXP extern const int K_TUPLET_NUMBER_UNKNOWN_;

EXP extern const std::string
  kTupletNumberPrefix,
  kTupletNumberDefaultValueString;

#ifndef MF_USE_WRAPPED_TYPES
  using mfTupletNumber = int;
#else
  using mfTupletNumber =
    mfWrappedValueForArithmetic <
      int,
      kTupletNumberPrefix,
      K_MF_EMPTY_STRING,
      K_TUPLET_NUMBER_UNKNOWN_,
      kTupletNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfTupletNumberAsString (
  const mfTupletNumber& tupletNumber);

EXP extern int mfTupletNumberAsInteger (
  const mfTupletNumber& tupletNumber);

//______________________________________________________________________________
// lyrics

EXP extern const std::string
  K_STANZA_NUMBER_UNKNOWN_,
  K_STANZA_NAME_UNKNOWN_;

EXP extern const std::string
  kStanzaNumberPrefix,
  kStanzaNumberDefaultValueString;

#ifndef MF_USE_WRAPPED_TYPES
  using mfStanzaNumber = std::string;
#else
  using mfStanzaNumber =
    mfWrappedValueWithDefault <
      std::string,
      kStanzaNumberPrefix,
      K_MF_EMPTY_STRING,
      K_STANZA_NUMBER_UNKNOWN_,
      kStanzaNumberDefaultValueString
    >;
#endif // MF_USE_WRAPPED_TYPES

EXP extern std::string mfStanzaNumberAsString (
  const mfStanzaNumber& stanzaNumber);

//______________________________________________________________________________

void testMfBasicTypes ();


}


#endif // ___mfBasicTypes___


