/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2musicxmlInterface___
#define ___musicxml2musicxmlInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind


namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFile2musicxmlWithHandler (
  const char*  fileName,
  std::ostream&     out,
  std::ostream&     err,
  const S_oahHandler& handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFd2musicxmlWithHandler (
  FILE*        fd,
  std::ostream&     out,
  std::ostream&     err,
  const S_oahHandler& handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlString2musicxmlWithHandler (
  const char*  buffer,
  std::ostream&     out,
  std::ostream&     err,
  const S_oahHandler& handler);


}


#endif
