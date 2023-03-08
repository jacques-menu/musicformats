/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfIndentedTextOutput.h"

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
  mfOahException* obj = new
    mfOahException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

//______________________________________________________________________________
void oahWarning (const std::string& warningMessage)
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  gLog <<
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

  gLog <<
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

  gLog <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw mfOahException (errorMessage);
}

void oahErrorWithContext (
  const std::string& errorMessage,
  const std::string& context)
{
  gIndenter.resetToZero ();

  gLog <<
    "### ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw mfOahException (errorMessage);
}

//______________________________________________________________________________
void oahInternalError (const std::string& errorMessage)
{
  gIndenter.resetToZero ();

  gLog <<
    "### INTERNAL ERROR: [OAH]: " <<
    errorMessage <<
    std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw mfOahException (errorMessage);
}

void oahInternalWarning (const std::string& errorMessage)
{
  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  gLog <<
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

  gLog <<
    "### INTERNAL ERROR [OAH]: " <<
    errorMessage <<
    ", " <<
    context <<
    std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw mfOahException (errorMessage);
}


}
