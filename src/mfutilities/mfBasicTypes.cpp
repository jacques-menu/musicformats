/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfBasicTypes.h"


namespace MusicFormats
{

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
