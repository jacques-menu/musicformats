/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrWae___
#define ___bsrWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrException: public mfException
{
  public:
    bsrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<bsrException> S_bsrException;

class EXP bsrInternalException: public mfException
{
  public:
    bsrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<bsrInternalException> S_bsrInternalException;

//______________________________________________________________________________
EXP void bsrWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void bsrInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);


}


#endif // ___bsrWae___
