/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "wae.h"
#include "mfslWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfslWarning (
  const string&       message,
  const yy::location& loc)
{
  gLogStream <<
    "### [MFSL] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void mfslError (
  const string&       message,
  const yy::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [MFSL] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "mfsl INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw mfslInternalException (message);
}

EXP void mfslFileError (
  const string& inputSourceName,
  const string& message)
{
  gLogStream <<
    "### MFSL file error: " << message <<
    endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}


}
