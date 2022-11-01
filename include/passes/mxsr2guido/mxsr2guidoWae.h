/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2guidoWae___
#define ___mxsr2guidoWae___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsr2guidoException : public mfException
{
  public:
    mxsr2guidoException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mxsr2guidoException> S_mxsr2guidoException;

//______________________________________________________________________________
EXP void mxsr2guidoWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void mxsr2guidoError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void mxsr2guidoInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
