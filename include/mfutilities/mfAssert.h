/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfAssert___
#define ___mfAssert___

#include "exports.h"

#include "mfBasicTypes.h"

#include "mfBool.h"


namespace MusicFormats
{

//______________________________________________________________________________
// assert

EXP void mfAssert (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  Bool               condition,
  const std::string& messageIfFalse);

EXP void mfAssertFalse (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& messageIfFalse);

//______________________________________________________________________________
// assert with input location

EXP void mfAssertWithInputLocation (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  Bool               condition,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& messageIfFalse);


}


#endif // ___mfAssert___
