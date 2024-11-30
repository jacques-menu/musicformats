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

const int K_MF_INPUT_LINE_UNKNOWN_ = 0;

// staff numbers
//______________________________________________________________________________

const int K_STAFF_NUMBER_UNKNOWN_ = -99;

const int K_PART_HARMONIES_STAFF_NUMBER = 10;

const int K_PART_FIGURED_BASS_STAFF_NUMBER = 20;

std::string mfStaffNumberAsString (int staffNumber)
{
  std::string result;

  if (staffNumber == K_STAFF_NUMBER_UNKNOWN_) {
    result = "*Unknown*";
  }
  else {
    result = std::to_string (staffNumber);
  }

  return result;
}

// voice numbers
//______________________________________________________________________________

const int K_VOICE_NUMBER_UNKNOWN_ = -96;

const int K_PART_HARMONIES_VOICE_NUMBER = 11;
const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER = 20;

const int K_PART_FIGURED_BASS_VOICE_NUMBER = 21;
const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER = 40;

std::string mfVoiceNumberAsString (int voiceNumber)
{
  std::string result;

  if (voiceNumber == K_VOICE_NUMBER_UNKNOWN_) {
    result = "*Unknown*";
  }
  else {
    result = std::to_string (voiceNumber);
  }

  return result;
}

// measure numbers
//______________________________________________________________________________

const std::string K_MEASURE_NUMBER_UNKNOWN_ = "K_MEASURE_NUMBER_UNKNOWN_";

// lyrics
//______________________________________________________________________________

const std::string K_STANZA_NUMBER_UNKNOWN_ = "K_STANZA_NUMBER_UNKNOWN_";
const std::string K_STANZA_NAME_UNKNOWN_ = "K_STANZA_NAME_UNKNOWN_";

// names lists max length
//______________________________________________________________________________

const int K_MF_NAMES_LIST_MAX_LENGTH = 50;


}
