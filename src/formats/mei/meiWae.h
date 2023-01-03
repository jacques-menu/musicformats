/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___meiWae___
#define ___meiWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP meiException : public mfException
{
  public:
    meiException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<meiException> S_meiException;

// class EXP meiInternalException: public mfException
// {
//   public:
//     meiInternalException (std::string const& exceptionDescription) throw ()
//       : mfException (exceptionDescription)
//     {}
// };
// typedef SMARTP<meiInternalException> S_meiInternalException;

//______________________________________________________________________________
EXP void meiWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void meiError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

EXP void meiInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);


}


#endif
