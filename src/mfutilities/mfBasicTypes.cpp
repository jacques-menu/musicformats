/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfBasicTypes.h"

#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

//______________________________________________________________________________
// the empty string
const std::string K_MF_EMPTY_STRING ("");

//______________________________________________________________________________
// input line numbers

const int K_MF_INPUT_LINE_UNKNOWN_ = -1;

const std::string
  kInputLineNumberPrefix             ("line_"),
  kInputLineNumberDefaultValueString ("*line_Unknown*");

//______________________________________________________________________________
// input locations

const std::string
  kInputLocationRangePrefix (".."),
  kInputLocationDefaultValueString ("*location_Unknown*");

//______________________________________________________________________________
// staff numbers

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

//______________________________________________________________________________
// voice numbers

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

//______________________________________________________________________________
// measure numbers

const std::string K_MEASURE_NUMBER_UNKNOWN_ =
  "K_MEASURE_NUMBER_UNKNOWN_";

const std::string
  kMeasureNumberPrefix ("measure_"),
  kMeasureNumberDefaultValueString ("*measure_Unknown*");

std::string mfMeasureNumberAsString (
  const mfMeasureNumber& measureNumber)
{
  std::string result;

  if (measureNumber == K_MEASURE_NUMBER_UNKNOWN_) {
    result = "K_MEASURE_NUMBER_UNKNOWN_";
  }
  else {
#ifndef MF_USE_WRAPPED_TYPES
    result = measureNumber;
#else
    result = measureNumber.getBareValue ();
#endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// lyrics

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

//______________________________________________________________________________
// names lists max length

EXP extern const int K_MF_NAMES_LIST_MAX_LENGTH = 50;

//______________________________________________________________________________
void testMfBasicTypes ()
{
  {
    mfInputLineNumber inputLineNumber1 (157);
    mfInputLineNumber inputLineNumber2 (200);

//     mfInputLocationRange inputLocationRange1 (333, 333);
//     mfInputLocationRange inputLocationRange2 (110, 132);

    mfVoiceNumber voiceNumber (5);

  //   inputLineNumber1 = staffNumber;
  //   voiceNumber = inputLineNumber2;

    inputLineNumber2 = inputLineNumber1;

    gLog <<
      "inputLineNumber1: " << inputLineNumber1 <<
      std::endl <<
      "inputLineNumber2: " << inputLineNumber2 <<
      std::endl <<
//       "inputLocationRange1: " << inputLocationRange1 <<
//       std::endl <<
//       "inputLocationRange2: " << inputLocationRange2 <<
//       std::endl <<
      "voiceNumber: " << voiceNumber <<
//       std::endl <<
//       "inputLocationRange2.asString (): " << inputLocationRange2.asString () <<
      std::endl;
  }

  gLog << "--------------" << std::endl;

  {
    // start the clock
    clock_t startClock = clock ();

    constexpr int iterationsNumber = 10000000;
    int total = 0;
    for (int i = 1; i < iterationsNumber; ++i ) {
      total += i;
    } // for

    clock_t endClock = clock ();

    // print the time spent
    clock_t
      timeSpentInClocks =
        endClock - startClock;

    gLog <<
      "Total: " << total << std::endl <<
      "Time spent with integers: " << timeSpentInClocks << " clocks" <<
      std::endl;
  }

  gLog << "--------------" << std::endl;

  {
    // start the clock
    clock_t startClock = clock ();

    constexpr int iterationsNumber = 10000000;
    int total = 0;
    for (int i = 1; i < iterationsNumber; ++i ) {
      mfInputLineNumber inputLineNumber (i);

#ifdef MF_USE_WRAPPED_TYPES
      total += inputLineNumber.getBareValue ();
#else
      total += inputLineNumber;
#endif // MF_USE_WRAPPED_TYPES

    } // for

    clock_t endClock = clock ();

    // print the time spent
    clock_t
      timeSpentInClocks =
        endClock - startClock;

    gLog <<
      "Total: " << total << std::endl <<
      "Time spent with mfInputLineNumber's: " << timeSpentInClocks << " clocks" <<
      std::endl;
  }
}


}
