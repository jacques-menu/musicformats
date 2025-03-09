/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfTest.h"
#include "mfIndentedTextOutput.h"
#include "mfBasicTypes.h"

/*
  place any include you need here...
*/

// #include "mfInputLinesRange.h"
#include "mfBasicTypes.h"

#include "stringMatcherInterface.h"


namespace MusicFormats
{

// typedef explicit_t <int> mfInputLineNumber;
// typedef explicit_t <int> mfStaffNumber;

//______________________________________________________________________________
void mfTest (std::ostream& os)
{
/*
  place any code you need here...
*/

  testStringMatcher (os);

  gLog << "--------------" << std::endl;

  testMfBasicTypes ();


  mfInputLineNumber inputLineNumber = 33;
  mfStaffNumber staffNumber = 17;

  int i = 267;

//   i = inputLineNumber;

  inputLineNumber = i;

//   inputLineNumber = staffNumber;

//   staffNumber = inputLineNumber;

  gLog <<
    std::endl <<
    "inputLineNumber: " << inputLineNumber << std::endl <<
    "staffNumber: " << staffNumber << std::endl <<
    std::endl;
}


}

