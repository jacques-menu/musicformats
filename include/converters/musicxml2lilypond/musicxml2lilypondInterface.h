/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2lilypondInterface___
#define ___musicxml2lilypondInterface___

#include <ostream>

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "msrOah.h"


namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFile2lilypondWithHandler (
  const char*         fileName,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlFd2lilypondWithHandler (
  FILE*               fd,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMusicxmlString2lilypondWithHandler (
  const char*         buffer,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler);


}

#endif // ___musicxml2lilypondInterface___
