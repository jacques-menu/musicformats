/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondWae___
#define ___lpsr2lilypondWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsr2lilypondException: public mfException
{
  public:
    lpsr2lilypondException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<lpsr2lilypondException> S_lpsr2lilypondException;

//______________________________________________________________________________
EXP void lpsr2lilypondWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void lpsr2lilypondInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void lpsr2lilypondError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

EXP void lpsr2lilypondInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);


}


#endif
