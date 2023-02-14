/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrWae___
#define ___msrWae___

#include "exports.h"

#include "mfExceptions.h"

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrException: public mfException
{
  public:

    static SMARTP<msrException> create (
                          std::string const& exceptionDescription);

                          msrException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<msrException> S_msrException;

class EXP msrInternalException: public mfException
{
  public:

    static SMARTP<msrInternalException> create (
                          std::string const& exceptionDescription);

                          msrInternalException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<msrInternalException> S_msrInternalException;

class EXP msrUnsupportedException: public mfException
{
  public:

    static SMARTP<msrUnsupportedException> create (
                          std::string const& exceptionDescription);

                          msrUnsupportedException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<msrUnsupportedException> S_msrUnsupportedException;

//______________________________________________________________________________
EXP void msrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void msrError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

EXP void msrUnsupported (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

EXP void msrInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void msrInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);


}


#endif // ___msrWae___
