/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "mfEnableAbortToDebugErrors.h"

#include "oahWae.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
mfOahException::mfOahException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_mfOahException mfOahException::create (
  std::string const& exceptionDescription)
{
  mfOahException* o = new
    mfOahException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void oahWarning (const std::string& warningMessage)
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  gLogStream <<
    "*** WARNING [OAH]: " <<
    warningMessage <<
    std::endl;

  gIndenter.setIndentation (saveIndent);
}

void oahWarningWithContext (
  const std::string& warningMessage,
  const std::string& context)
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  gLogStream <<
    "*** WARNING [OAH]: " <<
    warningMessage <<
    ", " <<
    context <<
    std::endl;

  gIndenter.setIndentation (saveIndent);
}

//______________________________________________________________________________
void oahError (const std::string& errorMessage)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    std::endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

void oahErrorWithContext (
  const std::string& errorMessage,
  const std::string& context)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    std::endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

//______________________________________________________________________________
void oahInternalError (const std::string& errorMessage)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL ERROR: [OAH]: " <<
    errorMessage <<
    std::endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}

void oahInternalWarning (const std::string& errorMessage)
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL WARNING: [OAH]: " <<
    errorMessage <<
    std::endl;

  gIndenter.setIndentation (saveIndent);
}

void oahInternalErrorWithContext (
  const std::string& errorMessage,
  const std::string& context)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### INTERNAL ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    std::endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw mfOahException (errorMessage);
}


}
