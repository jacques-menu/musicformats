/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfConstants___
#define ___mfConstants___

#include <string>

#include "exports.h"

#include "mfMoments.h"
#include "mfDurations.h"


namespace MusicFormats
{

// the empty string
//______________________________________________________________________________
EXP extern const std::string K_MF_EMPTY_STRING;

// input line numbers
//______________________________________________________________________________

EXP extern const int K_MF_INPUT_LINE_UNKNOWN_;

EXP extern const std::string
  kInputLineNumberPrefix,
  kInputLineNumberDefaultValueString;

// input locations
//______________________________________________________________________________

EXP extern const std::string
  kInputLocationRangePrefix,
  kInputLocationDefaultValueString;

// staff numbers
//______________________________________________________________________________

EXP extern const int K_STAFF_NUMBER_UNKNOWN_;

EXP extern const std::string
  kStaffNumberPrefix,
  kStaffNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_STAFF_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_STAFF_NUMBER;

EXP extern std::string mfStaffNumberAsString (int staffNumber);

// voice numbers
//______________________________________________________________________________

EXP extern const int K_VOICE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kVoiceNumberPrefix,
  kVoiceNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_VOICE_NUMBER;
EXP extern const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_VOICE_NUMBER;
EXP extern const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

EXP extern std::string mfVoiceNumberAsString (int voiceNumber);

// measure numbers
//______________________________________________________________________________

EXP extern const std::string K_MEASURE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMeasureNumberPrefix,
  kMeasureNumberDefaultValueString;

EXP extern std::string mfMeasureNumberAsString (std::string measureNumber);

// whole notes durations
//______________________________________________________________________________

EXP extern const int K_WHOLE_NOTES_NUMERATOR_UNKNOWN_;

EXP extern const mfWholeNotes K_WHOLE_NOTES_UNKNOWN_;

EXP extern const std::string
  kWholeNotePrefix,
  kWholeNoteDefaultValueString;

// moments
//______________________________________________________________________________

EXP extern const mfWholeNotes K_POSITION_IN_MEASURE_UNKNOWN_;

EXP extern const mfMoment K_MOMENT_UNKNOWN_;

EXP extern const std::string
  kMomentPrefix,
  kMomentDefaultValueString;

// lyrics
//______________________________________________________________________________

EXP extern const std::string
  K_STANZA_NUMBER_UNKNOWN_,
  K_STANZA_NAME_UNKNOWN_;

EXP extern const std::string
  kStanzaNumberPrefix,
  kStanzaNumberDefaultValueString;

EXP extern std::string mfStanzaNumberAsString (std::string stanzaNumber);

// names lists max length
//______________________________________________________________________________

EXP extern const int K_MF_NAMES_LIST_MAX_LENGTH;


}


#endif // ___mfConstants___
