/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <string>

#include "mfConstants.h"


namespace MusicFormats
{

// input line numbers
//______________________________________________________________________________

const std::string
  kInputLineNumberPrefix             ("L_"),
  kInputLineNumberNeutralValueString ("*Unknown*");

// input locations
//______________________________________________________________________________

const std::string
  kInputLocationRangePrefix (".."),
  kInputLocationNeutralValueString ("*Unknown*");

// staff numbers
//______________________________________________________________________________

const std::string
  kStaffNumberPrefix ("ST_"),
  kStaffNumberNeutralValueString ("*Unknown*");

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

const std::string
  kVoiceNumberPrefix ("V_"),
  kVoiceNumberNeutralValueString ("*Unknown*");

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
  kMeasureNumberPrefix ("ST_"),
  kMeasureNumberNeutralValueString ("*Unknown*");

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

//______________________________________________________________________________
// whole notes durations

const msrWholeNotes K_WHOLE_NOTES_UNKNOWN_ =
  msrWholeNotes (-1, 1);

const std::string
  kWholeNotePrefix ("WN_"),
  kWholeNoteNeutralValueString ("K_WHOLE_NOTES_UNKNOWN_");

// moments
//______________________________________________________________________________
const msrWholeNotes K_MEASURE_POSITION_UNKNOWN_ =
  msrWholeNotes (K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);

const msrMoment K_MOMENT_UNKNOWN_ =
  msrMoment (
    K_MEASURE_POSITION_UNKNOWN_,
    K_MEASURE_POSITION_UNKNOWN_);

const std::string
  kMomentPrefix ("WN_"),
  kMomentNeutralValueString ("K_MOMENT_UNKNOWN_");

// lyrics
//______________________________________________________________________________

const std::string
  kStanzaNumberPrefix ("ST_"),
  kStanzaNumberNeutralValueString ("*Unknown*");

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


}
