/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___meiWae___
#define ___meiWae___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP meiException : public mfException
{
  public:
    meiException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<meiException> S_meiException;

// class EXP meiInternalException: public mfException
// {
//   public:
//     meiInternalException (string const& exceptionDescription) throw ()
//       : mfException (exceptionDescription)
//     {}
// };
// typedef SMARTP<meiInternalException> S_meiInternalException;

//______________________________________________________________________________
EXP void meiWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void meiError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void meiInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
