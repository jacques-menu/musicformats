/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2brailleWae___
#define ___bsr2brailleWae___

#include "exports.h"
#include "smartpointer.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsr2brailleException : public mfException
{
  public:

    static SMARTP<bsr2brailleException> create (
                          string const& exceptionDescription);

                          bsr2brailleException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2brailleException> S_bsr2brailleException;

class EXP bsr2brailleInternalException : public mfException
{
  public:

    static SMARTP<bsr2brailleInternalException> create (
                          string const& exceptionDescription);

                          bsr2brailleInternalException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2brailleInternalException> S_bsr2brailleInternalException;

//______________________________________________________________________________
EXP void bsr2brailleWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void bsr2brailleInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
