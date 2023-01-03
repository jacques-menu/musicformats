/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWae___
#define ___mfWae___

#include "exports.h"

#include "mfExceptions.h"

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfWarning (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void mfError (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void mfInternalWarning (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);


}


#endif
