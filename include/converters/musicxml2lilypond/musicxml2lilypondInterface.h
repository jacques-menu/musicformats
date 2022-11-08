/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2lilypondInterface___
#define ___musicxml2lilypondInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind


using namespace std;

namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFile2lilypondWithHandler (
  const char*  fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFd2lilypondWithHandler (
  FILE*        fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlString2lilypondWithHandler (
  const char*  buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler);


}

#endif
