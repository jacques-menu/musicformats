/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfTest.h"
#include "mfIndentedTextOutput.h"
#include "mfWrapperTypes.h"

/*
  place any include you need here...
*/


#include "stringMatcherInterface.h"


namespace MusicFormats
{

//______________________________________________________________________________
void testWrapperTypes ()
{
  mfInputLineNumber inputLineNumber1 (157);
  mfInputLineNumber inputLineNumber2 (200);

  mfInputLocationRange inputLocationRange1 (333, 333);
  mfInputLocationRange inputLocationRange2 (110, 132);

  mfVoiceNumber voiceNumber (5);

//   inputLineNumber1 = staffNumber;
//   voiceNumber = inputLineNumber2;

  inputLineNumber2 = inputLineNumber1;

  gLog <<
    "inputLineNumber1: " << inputLineNumber1 <<
    std::endl <<
    "inputLocationRange1: " << inputLocationRange1 <<
    std::endl <<
    "inputLocationRange2: " << inputLocationRange2 <<
    std::endl <<
    "voiceNumber: " << voiceNumber <<
    std::endl <<
    "inputLocationRange2.asString (): " << inputLocationRange2.asString () <<
    std::endl;
}

void mfTest (std::ostream& os)
{
/*
  place any code you need here...
*/

  testStringMatcher (os);

  gLog << std::endl << std::endl;

  testWrapperTypes ();
}


}

