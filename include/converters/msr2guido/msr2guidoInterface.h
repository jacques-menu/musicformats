/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2guidoInterface___
#define ___msr2guidoInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahBasicTypes.h"


namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2guidoWithHandler (
  S_msrScore   theMsrScore,
  std::string       passNumber1,
  std::string       passDescription1,
  std::string       passNumber2,
  std::string       passDescription2,
  std::string       passNumber3,
  std::string       passDescription3,
  std::ostream&     out,
  std::ostream&     err,
  S_oahHandler handler);


}


#endif
