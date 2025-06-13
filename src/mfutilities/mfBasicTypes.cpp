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
// input line numbers

const int K_MF_INPUT_LINE_UNKNOWN_ = -1;

const std::string
  kMfInputLineNumberPrefix             ("line_"),
  kMfInputLineNumberDefaultValueString ("*line_Unknown*");

EXP extern std::string mfInputLineNumberAsString (
  const mfInputLineNumber& inputLineNumber)
{
  std::string result;

  if (inputLineNumber == K_MF_INPUT_LINE_UNKNOWN_) {
    result = "K_MF_INPUT_LINE_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = inputLineNumber;
// #else
//     result = inputLineNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

EXP extern int mfInputLineNumberAsInteger (
  const mfInputLineNumber& inputLineNumber);

int mfInputLineNumberAsInteger (
  const mfInputLineNumber& inputLineNumber)
{
  int result;

  if (inputLineNumber == K_MF_INPUT_LINE_UNKNOWN_) {
    result = K_MF_INPUT_LINE_UNKNOWN_;
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = inputLineNumber;
// #else
//     result = inputLineNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// input locations

const std::string
  kMfInputLocationRangePrefix (".."),
  kMfInputLocationDefaultValueString ("*location_Unknown*");

//______________________________________________________________________________
// staff numbers

const int K_MF_STAFF_NUMBER_UNKNOWN_ = -99;

const std::string
  kMfStaffNumberPrefix ("staff_"),
  kMfStaffNumberDefaultValueString ("*staff_Unknown*");

EXP extern const int K_MF_PART_HARMONIES_STAFF_NUMBER = 10;

EXP extern const int K_MF_PART_FIGURED_BASS_STAFF_NUMBER = 20;

std::string mfStaffNumberAsString (const mfStaffNumber& staffNumber)
{
  std::string result;

  if (staffNumber == K_MF_STAFF_NUMBER_UNKNOWN_) {
    result = "K_MF_STAFF_NUMBER_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = staffNumber;
// #else
//     result = staffNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

int mfStaffNumberAsInteger (const mfStaffNumber& staffNumber)
{
  int result;

  if (staffNumber == K_MF_STAFF_NUMBER_UNKNOWN_) {
    result = K_MF_STAFF_NUMBER_UNKNOWN_;
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = staffNumber;
// #else
// //     std::stringstream ss;
// //
// //     int intStaffNumber;
// //
// //     ss << staffNumber;
// //     ss >> intStaffNumber;
// //
// //     result = intStaffNumber;
//     result = staffNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// voice numbers

const int K_MF_VOICE_NUMBER_UNKNOWN_ = -96;

const std::string
  kMfVoiceNumberPrefix ("voice_"),
  kMfVoiceNumberDefaultValueString ("*voice_Unknown*");

EXP extern const int K_MF_PART_HARMONIES_VOICE_NUMBER = 11;
EXP extern const int K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER = 20;

EXP extern const int K_MF_PART_FIGURED_BASS_VOICE_NUMBER = 21;
EXP extern const int K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER = 40;

std::string mfVoiceNumberAsString (const mfVoiceNumber& voiceNumber)
{
  std::string result;

  if (voiceNumber == K_MF_VOICE_NUMBER_UNKNOWN_) {
    result = "K_MF_VOICE_NUMBER_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = voiceNumber;
// #else
//     result = std::to_string (voiceNumber.getBareValue ());
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

int mfVoiceNumberAsInteger (const mfVoiceNumber& voiceNumber)
{
  int result;

  if (voiceNumber == K_MF_VOICE_NUMBER_UNKNOWN_) {
    result = K_MF_VOICE_NUMBER_UNKNOWN_;
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = voiceNumber;
// #else
//     result = voiceNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// measure numbers

const std::string K_MF_MEASURE_NUMBER_UNKNOWN_ =
  "K_MF_MEASURE_NUMBER_UNKNOWN_";

const std::string
  kMfMeasureNumberPrefix ("measure_"),
  kMfMeasureNumberDefaultValueString ("*measure_Unknown*");

std::string mfMeasureNumberAsString (
  const mfMeasureNumber& measureNumber)
{
  std::string result;

  if (measureNumber == K_MF_MEASURE_NUMBER_UNKNOWN_) {
    result = "K_MF_MEASURE_NUMBER_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = measureNumber;
// #else
//     result = measureNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// tuplet numbers

const int K_MF_TUPLET_NUMBER_UNKNOWN_ = 0;

const std::string
  kMfTupletNumberPrefix ("tuplet_"),
  kMfTupletNumberDefaultValueString ("*tuplet_Unknown*");

std::string mfTupletNumberAsString (const mfTupletNumber& tupletNumber)
{
  std::string result;

  if (tupletNumber == K_MF_TUPLET_NUMBER_UNKNOWN_) {
    result = "K_MF_TUPLET_NUMBER_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = tupletNumber;
// #else
//     result = std::to_string (tupletNumber.getBareValue ());
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

int mfTupletNumberAsInteger (const mfTupletNumber& tupletNumber)
{
  int result;

  if (tupletNumber == K_MF_TUPLET_NUMBER_UNKNOWN_) {
    result = K_MF_TUPLET_NUMBER_UNKNOWN_;
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = tupletNumber;
// #else
//     result = tupletNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

//______________________________________________________________________________
// lyrics

const std::string
  kMfStanzaNumberPrefix ("stanza_"),
  kMfStanzaNumberDefaultValueString ("*stanza_Unknown*");

const std::string
  K_MF_STANZA_NUMBER_UNKNOWN_ = "K_MF_STANZA_NUMBER_UNKNOWN_",
  K_MF_STANZA_NAME_UNKNOWN_ = "K_MF_STANZA_NAME_UNKNOWN_";

std::string mfStanzaNumberAsString (const mfStanzaNumber& stanzaNumber)
{
  std::string result;

  if (stanzaNumber == K_MF_STANZA_NUMBER_UNKNOWN_) {
    result = "K_MF_STANZA_NAME_UNKNOWN_";
  }
  else {
// #ifndef MF_USE_WRAPPED_TYPES
    result = stanzaNumber;
// #else
//     result = stanzaNumber.getBareValue ();
// #endif // MF_USE_WRAPPED_TYPES
  }

  return result;
}

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

// #ifdef MF_USE_WRAPPED_TYPES
//       total += inputLineNumber.getBareValue ();
// #else
      total += inputLineNumber;
// #endif // MF_USE_WRAPPED_TYPES

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
