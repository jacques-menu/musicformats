/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlConverterInterface___
#define ___msdlConverterInterface___

#include "mfMusicformatsError.h" // for mfMusicformatsError


using namespace std;

namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFile2lilypondWithHandler (
  const char*  fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFd2lilypondWithHandler (
  FILE*        fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlString2lilypondWithHandler (
  const char*  buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);


}

#endif
