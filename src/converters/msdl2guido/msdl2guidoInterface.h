/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2guidoInterface___
#define ___msdl2guidoInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2guidoWithOptionsAndArguments (
  std::string                   inputSourceName,
  std::istream&                 inputStream,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlStream2guidoWithHandler (
  std::string         inputSourceName,
  std::istream&       inputStream,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2guidoWithOptionsAndArguments (
  std::string                   inputFileName,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind msdlFile2guidoWithHandler (
  std::string         inputFileName,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2guidoWithOptionsAndArguments (
  const char*                   buffer,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind msdlString2guidoWithHandler (
  const char*         buffer,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);


}


#endif
