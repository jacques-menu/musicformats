/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "oahWae.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
mfOahException::mfOahException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_mfOahException mfOahException::create (
  string const& exceptionDescription)
{
  mfOahException* o = new
    mfOahException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void oahWarning (const string& warningMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "*** WARNING [OAH]: " <<
    warningMessage <<
    endl;

  gIndenter.setIndent (saveIndent);
}

void oahWarningWithContext (
  const string& warningMessage,
  const string& context)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "*** WARNING [OAH]: " <<
    warningMessage <<
    ", " <<
    context <<
    endl;

  gIndenter.setIndent (saveIndent);
}

//______________________________________________________________________________
void oahError (const string& errorMessage)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

void oahErrorWithContext (
  const string& errorMessage,
  const string& context)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

//______________________________________________________________________________
void oahInternalError (const string& errorMessage)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL ERROR: [OAH]: " <<
    errorMessage <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

void oahInternalWarning (const string& errorMessage)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL WARNING: [OAH]: " <<
    errorMessage <<
    endl;

  gIndenter.setIndent (saveIndent);
}

void oahInternalErrorWithContext (
  const string& errorMessage,
  const string& context)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}


}
