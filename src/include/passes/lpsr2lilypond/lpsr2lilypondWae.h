/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondWae___
#define ___lpsr2lilypondWae___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsr2lilypondException: public mfException
{
  public:
    lpsr2lilypondException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<lpsr2lilypondException> S_lpsr2lilypondException;

//______________________________________________________________________________
EXP void lpsr2lilypondWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void lpsr2lilypondInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void lpsr2lilypondError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void lpsr2lilypondInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
