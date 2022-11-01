/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2mxsr___
#define ___musicxml2mxsr___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP musicxml2mxsrException : public mfException
{
  public:
    musicxml2mxsrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<musicxml2mxsrException> S_musicxml2mxsrException;

//______________________________________________________________________________
EXP void musicxml2mxsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void musicxml2mxsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void musicxml2mxsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
