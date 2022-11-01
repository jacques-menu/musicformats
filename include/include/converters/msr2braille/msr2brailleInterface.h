/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2brailleInterface___
#define ___msr2brailleInterface___

#include "mfMusicformatsError.h" // for mfMusicformatsError

#include "msr.h"

#include "oahBasicTypes.h"


using namespace std;

namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsError msrScore2brailleWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1a,
  string       passDescription1a,
  string       passNumber1b,
  string       passDescription1b,
  string       passNumber2,
  string       passDescription2,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);


}


#endif
