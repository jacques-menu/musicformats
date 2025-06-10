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

// #include "mfDurationsAndPositionInMeasures.h"
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
    mfWrappedValueWithDefault <
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
    mfWrappedValueWithDefault <
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
    mfWrappedValueWithDefault <
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
  const mfVoiceNumber& staffNumber);

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

// //______________________________________________________________________________
// enum class mfDurationKind {
//   kDuration_UNKNOWN_,
//
//   // from longest to shortest for the algorithms
//   kDurationMaxima, kDurationLonga, kDurationBreve,
//   kDurationWhole, kDurationHalf,
//   kDurationQuarter,
//   kDurationEighth, kDuration16th, kDuration32nd, kDuration64th,
//   kDuration128th, kDuration256th, kDuration512th, kDuration1024th
// };
//
// std::string mfDurationKindAsString (mfDurationKind notesDurationKind);
//
// std::ostream& operator << (std::ostream& os, const mfDurationKind& elt);
//
// EXP mfDurationKind mfDurationKindFromMusicXMLGraphicNoteType (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string& durationString);
//
// EXP mfDurationKind mfDurationKindFromInteger (
//   const mfInputLineNumber& inputLineNumber,
//   int durationInteger);
//
// EXP mfDurationKind mfDurationKindFromString (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string& durationString);
//
// std::string mfDurationKindAsMusicXMLType (mfDurationKind notesDurationKind);


//______________________________________________________________________________

void testMfBasicTypes ();


}


#endif // ___mfBasicTypes___


/*
  There are still error messages on Linux and Windows at link time
  regarding some methods being undefined due to the wrapped types
  instantiations below, such as:

Linux:

/usr/bin/ld: lib/liblibmusicformats.so: undefined reference to `MusicFormats::mxsrEventsCollection::createAMultipleMeasureRestEndAndRegisterIt(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, MusicFormats::mfWrappedValueWithDefault<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, MusicFormats::kMeasureNumberPrefix[abi:cxx11], MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MEASURE_NUMBER_UNKNOWN_, MusicFormats::kMeasureNumberDefaultValueString[abi:cxx11]> const&, int, MusicFormats::mfWrappedValueWithDefault<int, MusicFormats::kInputLineNumberPrefix, MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MF_INPUT_LINE_UNKNOWN_, MusicFormats::kInputLineNumberDefaultValueString> const&)'


Windows:

C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/12.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles/musicformats.dir/objects.a(mxsr2msrSkeletonBuilder.cpp.obj):mxsr2msrSkeletonBuilder.cpp:(.text+0x21cc1): undefined reference to `MusicFormats::mxsrEventsCollection::createAMultipleMeasureRestEndAndRegisterIt(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, MusicFormats::mfWrappedValueWithDefault<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, MusicFormats::kMeasureNumberPrefix[abi:cxx11], MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MEASURE_NUMBER_UNKNOWN_, MusicFormats::kMeasureNumberDefaultValueString[abi:cxx11]> const&, int, MusicFormats::mfWrappedValueWithDefault<int, MusicFormats::kInputLineNumberPrefix, MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MF_INPUT_LINE_UNKNOWN_, MusicFormats::kInputLineNumberDefaultValueString> const&)'
2025-02-23T16:44:36.0862400Z       |           ^~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


