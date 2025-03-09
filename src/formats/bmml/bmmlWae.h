/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bmmlWae___
#define ___bmmlWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bmmlException : public mfException
{
  public:
    bmmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<bmmlException> S_bmmlException;

// class EXP bmmlInternalException: public mfException
// {
//   public:
//     bmmlInternalException (std::string const& exceptionDescription) throw ()
//       : mfException (exceptionDescription)
//     {}
// };
// typedef SMARTP<bmmlInternalException> S_bmmlInternalException;

//______________________________________________________________________________
EXP void bmmlWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void bmmlError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void bmmlInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);


}


#endif // ___bmmlWae___
