/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2bsrWae___
#define ___bsr2bsrWae___

#include "exports.h"
#include "smartpointer.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsr2bsrException: public mfException
{
  public:

    static SMARTP<bsr2bsrException> create (
                          string const& exceptionDescription);

                          bsr2bsrException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2bsrException> S_bsr2bsrException;

class EXP bsr2finalizedBsrException : public mfException
{
  public:

    static SMARTP<bsr2finalizedBsrException> create (
                          string const& exceptionDescription);

                          bsr2finalizedBsrException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2finalizedBsrException> S_bsr2finalizedBsrException;

class EXP bsr2bsrInternalException : public mfException
{
  public:

    static SMARTP<bsr2bsrInternalException> create (
                          string const& exceptionDescription);

                          bsr2bsrInternalException (
                            string const& exceptionDescription) throw ();
};
typedef SMARTP<bsr2bsrInternalException> S_bsr2bsrInternalException;

//______________________________________________________________________________
EXP void bsr2bsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void bsr2bsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
