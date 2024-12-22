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

#include "msrMoments.h"
#include "msrNotesDurations.h"


namespace MusicFormats
{

// input line numbers
//______________________________________________________________________________

constexpr int K_MF_INPUT_LINE_UNKNOWN_ = -1;

EXP extern const std::string
  kInputLineNumberPrefix,
  kInputLineNumberNeutralValueString;

// input locations
//______________________________________________________________________________

EXP extern const std::string
  kInputLocationRangePrefix,
  kInputLocationNeutralValueString;

// staff numbers
//______________________________________________________________________________

constexpr int K_STAFF_NUMBER_UNKNOWN_ = -99;

EXP extern const std::string
  kStaffNumberPrefix,
  kStaffNumberNeutralValueString;

constexpr int K_PART_HARMONIES_STAFF_NUMBER = 10;

constexpr int K_PART_FIGURED_BASS_STAFF_NUMBER = 20;

EXP extern std::string mfStaffNumberAsString (int staffNumber);

// voice numbers
//______________________________________________________________________________

constexpr int K_VOICE_NUMBER_UNKNOWN_ = -96;

EXP extern const std::string
  kVoiceNumberPrefix,
  kVoiceNumberNeutralValueString;

constexpr int K_PART_HARMONIES_VOICE_NUMBER = 11;
constexpr int K_VOICE_HARMONIES_VOICE_BASE_NUMBER = 20;

constexpr int K_PART_FIGURED_BASS_VOICE_NUMBER = 21;
constexpr int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER = 40;

EXP extern std::string mfVoiceNumberAsString (int voiceNumber);

// measure numbers
//______________________________________________________________________________

EXP extern const std::string K_MEASURE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMeasureNumberPrefix,
  kMeasureNumberNeutralValueString;

EXP extern std::string mfMeasureNumberAsString (std::string measureNumber);

// whole notes durations
//______________________________________________________________________________

constexpr int K_WHOLE_NOTES_NUMERATOR_UNKNOWN_ = -1;

EXP extern const msrWholeNotes K_WHOLE_NOTES_UNKNOWN_;

EXP extern const std::string
  kWholeNotePrefix,
  kWholeNoteNeutralValueString;

// moments
//______________________________________________________________________________

EXP extern const msrWholeNotes K_MEASURE_POSITION_UNKNOWN_;

EXP extern const msrMoment K_MOMENT_UNKNOWN_;

EXP extern const std::string
  kMomentPrefix,
  kMomentNeutralValueString;

// lyrics
//______________________________________________________________________________

EXP extern const std::string
  K_STANZA_NUMBER_UNKNOWN_,
  K_STANZA_NAME_UNKNOWN_;

EXP extern const std::string
  kStanzaNumberPrefix,
  kStanzaNumberNeutralValueString;

EXP extern std::string mfStanzaNumberAsString (std::string stanzaNumber);

// names lists max length
//______________________________________________________________________________

constexpr int K_MF_NAMES_LIST_MAX_LENGTH = 50;


}


#endif // ___mfConstants___
