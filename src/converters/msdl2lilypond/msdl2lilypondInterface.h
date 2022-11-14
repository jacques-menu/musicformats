/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2lilypondInterface___
#define ___msdl2lilypondInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2lilypondWithHandler (
  std::string       inputSourceName,
  std::istream&     inputStream,
  S_oahHandler handler,
  std::ostream&     out,
  std::ostream&     err);

EXP mfMusicformatsErrorKind convertMsdlStream2lilypondWithOptionsAndArguments (
  std::string         inputSourceName,
  std::istream&                      inputStream,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler (
  std::string       fileName,
  S_oahHandler handler,
  std::ostream&     out,
  std::ostream&     err);

EXP mfMusicformatsErrorKind convertMsdlFile2lilypondWithOptionsAndArguments (
  std::string         fileName,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2lilypondWithHandler (
  const char*  buffer,
  S_oahHandler handler,
  std::ostream&     out,
  std::ostream&     err);

EXP mfMusicformatsErrorKind convertMsdlString2lilypondWithOptionsAndArguments (
  const char*                   buffer,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);


}


#endif
