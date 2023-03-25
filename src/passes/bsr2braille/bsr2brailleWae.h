/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2brailleWae___
#define ___bsr2brailleWae___

#include "exports.h"
#include "smartpointer.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsr2brailleException : public mfException
{
  public:

    static SMARTP<bsr2brailleException> create (
                          std::string const& exceptionDescription);

                          bsr2brailleException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2brailleException> S_bsr2brailleException;

class EXP bsr2brailleInternalException : public mfException
{
  public:

    static SMARTP<bsr2brailleInternalException> create (
                          std::string const& exceptionDescription);

                          bsr2brailleInternalException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2brailleInternalException> S_bsr2brailleInternalException;

//______________________________________________________________________________
EXP void bsr2brailleWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void bsr2brailleInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);


}


#endif // ___bsr2brailleWae___
