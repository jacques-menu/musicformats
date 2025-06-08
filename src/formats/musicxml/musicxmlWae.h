/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxmlWae___
#define ___musicxmlWae___

#include "exports.h"

#include "mfBasicTypes.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP musicxmlException: public mfException
{
  public:
    musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<musicxmlException> S_musicxmlException;

//______________________________________________________________________________
EXP void musicxmlWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message);

EXP void musicxmlError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);


}


#endif // ___musicxmlWae___
