/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <string>

#include "mfConstants.h"


namespace MusicFormats
{

// the empty string
//______________________________________________________________________________
const std::string K_MF_EMPTY_STRING ("");

// input line numbers
//______________________________________________________________________________

const int K_MF_INPUT_LINE_UNKNOWN_ = -1;

const std::string
  kInputLineNumberPrefix             ("line_"),
  kInputLineNumberDefaultValueString ("*line_Unknown*");

// input locations
//______________________________________________________________________________

const std::string
  kInputLocationRangePrefix (".."),
  kInputLocationDefaultValueString ("*location_Unknown*");

// staff numbers
//______________________________________________________________________________

const int K_STAFF_NUMBER_UNKNOWN_ = -99;

const std::string
  kStaffNumberPrefix ("staff_"),
  kStaffNumberDefaultValueString ("*staff_Unknown*");

EXP extern const int K_PART_HARMONIES_STAFF_NUMBER = 10;

EXP extern const int K_PART_FIGURED_BASS_STAFF_NUMBER = 20;

std::string mfStaffNumberAsString (int staffNumber)
{
  std::string result;

  if (staffNumber == K_STAFF_NUMBER_UNKNOWN_) {
    result = "K_STAFF_NUMBER_UNKNOWN_";
  }
  else {
    result = std::to_string (staffNumber);
  }

  return result;
}

// voice numbers
//______________________________________________________________________________

const int K_VOICE_NUMBER_UNKNOWN_ = -96;

const std::string
  kVoiceNumberPrefix ("voice_"),
  kVoiceNumberDefaultValueString ("*voice_Unknown*");

EXP extern const int K_PART_HARMONIES_VOICE_NUMBER = 11;
EXP extern const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER = 20;

EXP extern const int K_PART_FIGURED_BASS_VOICE_NUMBER = 21;
EXP extern const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER = 40;

std::string mfVoiceNumberAsString (int voiceNumber)
{
  std::string result;

  if (voiceNumber == K_VOICE_NUMBER_UNKNOWN_) {
    result = "K_VOICE_NUMBER_UNKNOWN_";
  }
  else {
    result = std::to_string (voiceNumber);
  }

  return result;
}

// measure numbers
//______________________________________________________________________________

const std::string K_MEASURE_NUMBER_UNKNOWN_ =
  "K_MEASURE_NUMBER_UNKNOWN_";

const std::string
  kMeasureNumberPrefix ("measure_"),
  kMeasureNumberDefaultValueString ("*measure_Unknown*");

std::string mfMeasureNumberAsString (std::string measureNumber)
{
  std::string result;

  if (measureNumber == K_MEASURE_NUMBER_UNKNOWN_) {
    result = "K_MEASURE_NUMBER_UNKNOWN_";
  }
  else {
    result = measureNumber;
  }

  return result;
}

// whole notes durations
//______________________________________________________________________________

const int K_WHOLE_NOTES_NUMERATOR_UNKNOWN_ = -1;

const mfWholeNotes K_WHOLE_NOTES_UNKNOWN_ =
  mfWholeNotes (-1, 1);

const mfWholeNotes K_WHOLE_NOTES_ZERO =
  mfWholeNotes (0, 1);

const std::string
  kWholeNotePrefix ("whole_notes_"),
  kWholeNoteDefaultValueString ("K_WHOLE_NOTES_UNKNOWN_");

// position in measure
//______________________________________________________________________________

const int K_POSITION_IN_MEASURE_NUMERATOR_UNKNOWN_ = -2;

const mfPositionInMeasure K_POSITION_IN_MEASURE_UNKNOWN_ =
  mfPositionInMeasure (K_POSITION_IN_MEASURE_NUMERATOR_UNKNOWN_, 1);

const mfPositionInMeasure K_POSITION_IN_MEASURE_ZERO =
  mfPositionInMeasure (0, 1);

// moments
//______________________________________________________________________________

const mfMoment K_MOMENT_UNKNOWN_ =
  mfMoment (
    K_POSITION_IN_MEASURE_UNKNOWN_,
    K_WHOLE_NOTES_UNKNOWN_);

const std::string
  kMomentPrefix ("moment_"),
  kMomentDefaultValueString ("K_MOMENT_UNKNOWN_");

// lyrics
//______________________________________________________________________________

const std::string
  kStanzaNumberPrefix ("stanza_"),
  kStanzaNumberDefaultValueString ("*stanza_Unknown*");

const std::string
  K_STANZA_NUMBER_UNKNOWN_ = "K_STANZA_NUMBER_UNKNOWN_",
  K_STANZA_NAME_UNKNOWN_ = "K_STANZA_NAME_UNKNOWN_";

std::string mfStanzaNumberAsString (std::string stanzaNumber)
{
  std::string result;

  if (stanzaNumber == K_STANZA_NUMBER_UNKNOWN_) {
    result = "K_STANZA_NAME_UNKNOWN_";
  }
  else {
    result = stanzaNumber;
  }

  return result;
}

// names lists max length
//______________________________________________________________________________

EXP extern const int K_MF_NAMES_LIST_MAX_LENGTH = 50;


}
