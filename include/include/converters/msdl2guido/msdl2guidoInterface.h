/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2guidoInterface___
#define ___msdl2guidoInterface___

#include "mfMusicformatsError.h" // for mfMusicformatsError


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlStream2guidoWithOptionsAndArguments (
  string                        inputSourceName,
  istream&                      inputStream,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsError convertMsdlStream2guidoWithHandler (
  string       inputSourceName,
  istream&     inputStream,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFile2guidoWithOptionsAndArguments (
  string                        fileName,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsError msdlFile2guidoWithHandler (
  string       fileName,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlString2guidoWithOptionsAndArguments (
  const char*                   buffer,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsError msdlString2guidoWithHandler (
  const char*  buffer,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);


}


#endif
