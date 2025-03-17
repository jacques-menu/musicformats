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
#include "mfDurations.h"
// #include "mfWrappedRange.h"
#include "mfWrappedValueForArithmetic.h"
#include "mfWrappedValueWithDefault.h"

// #include "mfDurations.h"
// #include "mfMoments.h"


namespace MusicFormats
{

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//                         WRAPPED TYPES USE
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/*
  There are still error messages on Linux and Windows at link time
  regarding some methods being undefined due to the instantiations below,
  such as:

Linux:

/usr/bin/ld: lib/liblibmusicformats.so: undefined reference to `MusicFormats::mxsrEventsCollection::createAMultipleMeasureRestEndAndRegisterIt(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, MusicFormats::mfWrappedValueWithDefault<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, MusicFormats::kMeasureNumberPrefix[abi:cxx11], MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MEASURE_NUMBER_UNKNOWN_, MusicFormats::kMeasureNumberDefaultValueString[abi:cxx11]> const&, int, MusicFormats::mfWrappedValueWithDefault<int, MusicFormats::kInputLineNumberPrefix, MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MF_INPUT_LINE_UNKNOWN_, MusicFormats::kInputLineNumberDefaultValueString> const&)'


Windows:

C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/12.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles/musicformats.dir/objects.a(mxsr2msrSkeletonBuilder.cpp.obj):mxsr2msrSkeletonBuilder.cpp:(.text+0x21cc1): undefined reference to `MusicFormats::mxsrEventsCollection::createAMultipleMeasureRestEndAndRegisterIt(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, MusicFormats::mfWrappedValueWithDefault<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, MusicFormats::kMeasureNumberPrefix[abi:cxx11], MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MEASURE_NUMBER_UNKNOWN_, MusicFormats::kMeasureNumberDefaultValueString[abi:cxx11]> const&, int, MusicFormats::mfWrappedValueWithDefault<int, MusicFormats::kInputLineNumberPrefix, MusicFormats::K_MF_EMPTY_STRING, MusicFormats::K_MF_INPUT_LINE_UNKNOWN_, MusicFormats::kInputLineNumberDefaultValueString> const&)'
2025-02-23T16:44:36.0862400Z       |           ^~~~~~~~~~~~~~~~~~~~~~~~~~~


  MF_USE_WRAPPED_TYPES is used temporarily
  to switch the use of these types on and off easily

  This is placed here because only a few source files use this feature,
  which minimizes re-compilations should the setting change
*/

// #define MF_USE_WRAPPED_TYPES


// input line numbers
//______________________________________________________________________________

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


// input location ranges
//______________________________________________________________________________

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfInputLocationRange = int;
// #else
//   using mfInputLocationRange =
//     mfWrappedRange <
//       mfInputLineNumber,
//       kInputLocationRangePrefix
//     >;
// #endif // MF_USE_WRAPPED_TYPES


// staff numbers
//______________________________________________________________________________

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


// voice numbers
//______________________________________________________________________________

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


// measure numbers
//______________________________________________________________________________

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


//______________________________________________________________________________
// whole notes durations

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfWholeNotes = mfWholeNotes;
// #else
//   using mfWholeNotes =
//     mfWrappedValueWithDefault <
//       mfWholeNotes,
//       kWholeNotePrefix,
//       K_MF_EMPTY_STRING,
//       K_WHOLE_NOTES_UNKNOWN_,
//       kWholeNoteDefaultValueString
//     >;
// #endif // MF_USE_WRAPPED_TYPES


// moments
//______________________________________________________________________________

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfMoment = mfMoment;
// #else
//   using mfMoment =
//     mfWrappedValueWithDefault <
//       mfMoment,
//       kMomentPrefix,
//       K_MF_EMPTY_STRING,
//       K_MOMENT_UNKNOWN_,
//       kMomentDefaultValueString
//     >;
// #endif // MF_USE_WRAPPED_TYPES


// lyrics
//______________________________________________________________________________

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


//______________________________________________________________________________
void testMfBasicTypes ();


}


#endif // ___mfBasicTypes___
