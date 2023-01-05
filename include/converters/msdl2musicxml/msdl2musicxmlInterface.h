 /*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2musicxmlInterface___
#define ___msdl2musicxmlInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahBasicTypes.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2musicxmlWithOptionsAndArguments (
  std::string                   inputSourceName,
  std::istream&                 inputStream,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlStream2musicxmlWithHandler (
  std::string         inputSourceName,
  std::istream&       inputStream,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2musicxmlWithOptionsAndArguments (
  std::string                   inputFileName,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlFile2musicxmlWithHandler (
  std::string         inputFileName,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2musicxmlWithOptionsAndArguments (
  const char*                   buffer,
  const oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&                 out,
  std::ostream&                 err);

EXP mfMusicformatsErrorKind convertMsdlString2musicxmlWithHandler (
  const char*         buffer,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err);


}


#endif
