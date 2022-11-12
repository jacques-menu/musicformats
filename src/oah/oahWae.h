/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahWae___
#define ___oahWae___

#include "exports.h"
#include "smartpointer.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfOahException: public mfException
{
  public:

    static SMARTP<mfOahException> create (
                          std::string const& exceptionDescription);

                          mfOahException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<mfOahException> S_mfOahException;

//______________________________________________________________________________
EXP void oahWarning (const std::string& warningMessage);

EXP void oahWarningWithContext (
  const std::string& warningMessage,
  const std::string& context);

//______________________________________________________________________________
EXP void oahError (const std::string& errorMessage);

EXP void oahErrorWithContext (
  const std::string& errorMessage,
  const std::string& context);

//______________________________________________________________________________
EXP void oahInternalWarning (const std::string& errorMessage);

EXP void oahInternalError (const std::string& errorMessage);

EXP void oahInternalErrorWithContext (
  const std::string& errorMessage,
  const std::string& context);


}


#endif
