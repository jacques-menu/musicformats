/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __languageInterface___
#define __languageInterface___

#include "exports.h"

#include "mfBasicTypes.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfWarning (
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);

EXP void mfError (
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);

EXP void mfInternalWarning (
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);


}


#endif // __languageInterface___
