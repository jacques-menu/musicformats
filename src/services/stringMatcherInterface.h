/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __stringMatcherInterface___
#define __stringMatcherInterface___

#include <ostream>

#include "exports.h"

#include "mfMusicformatsErrors.h"


namespace MusicFormats
{

//______________________________________________________________________________
mfMusicformatsErrorKind executeStringMatcher (
  const std::string& stringMatcherExpressionString,
  const std::string& stringMatcherInputString);

//_______________________________________________________________________________
EXP int stringMatcher (
  int   argc,
  char* argv[]);

//_______________________________________________________________________________
EXP int stringMatcher (
  FILE*         fd,
  std::ostream& out,
  std::ostream& err);

//_______________________________________________________________________________
EXP int stringMatcher (
  const char*   buffer,
  std::ostream& out,
  std::ostream& err);

//______________________________________________________________________________
void testStringMatcher (std::ostream& os);


}

#endif // __stringMatcherInterface___
