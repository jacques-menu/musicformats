/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2brailleInterface___
#define ___msdl2brailleInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2brailleWithOptionsAndArguments (
  string                        inputSourceName,
  istream&                      inputStream,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlStream2brailleWithHandler (
  string       inputSourceName,
  istream&     inputStream,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2brailleWithOptionsAndArguments (
  string                        fileName,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlFile2brailleWithHandler (
  string       fileName,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2brailleWithOptionsAndArguments (
  const char*                   buffer,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlString2brailleWithHandler (
  const char*  buffer,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err);


}


#endif
