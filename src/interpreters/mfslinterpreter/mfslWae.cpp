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


namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfslWarning (
  const std::string&       message,
  const mfsl::location& loc)
{
  gLogStream <<
    "### [MFSL] WARNING " <<
    loc << ": " << message <<
    std::endl;
}

EXP void mfslError (
  const std::string&       message,
  const mfsl::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [MFSL] ERROR " <<
    loc << ": " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslInternalError (
  const std::string&       message,
  const mfsl::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [MFSL] INNTERNAL ERROR " <<
    loc << ": " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLogStream <<
    "### MFSL file error: " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  std::stringstream s;

  s <<
    "### MFSL options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    std::endl;

  std::string message = s.str ();

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}


}
