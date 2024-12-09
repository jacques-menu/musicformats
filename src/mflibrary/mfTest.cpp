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
void mfTest (std::ostream& os)
{
/*
  place any code you need here...
*/

  testStringMatcher (os);

  gLog << "--------------" << std::endl;

  testWrapperTypes ();
}


}

