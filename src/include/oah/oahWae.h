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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfOahException: public mfException
{
  public:

    static SMARTP<mfOahException> create (
                          string const& exceptionDescription);

                          mfOahException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<mfOahException> S_mfOahException;

//______________________________________________________________________________
EXP void oahWarning (const string& warningMessage);

EXP void oahWarningWithContext (
  const string& warningMessage,
  const string& context);

//______________________________________________________________________________
EXP void oahError (const string& errorMessage);

EXP void oahErrorWithContext (
  const string& errorMessage,
  const string& context);

//______________________________________________________________________________
EXP void oahInternalWarning (const string& errorMessage);

EXP void oahInternalError (const string& errorMessage);

EXP void oahInternalErrorWithContext (
  const string& errorMessage,
  const string& context);


}


#endif
