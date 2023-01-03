/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeInterface.h"
#include "ischemeWae.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void ischemeWarning (
  const std::string&       message,
  const iscm::location& loc)
{
  gLogStream <<
    "### [iScheme] WARNING " <<
    loc << ": " << message <<
    std::endl;
}

EXP void ischemeError (
  const std::string&       message,
  const iscm::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [iScheme] ERROR " <<
    loc << ": " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeInternalError (
  const std::string&       message,
  const iscm::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [iScheme] INNTERNAL ERROR " <<
    loc << ": " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLogStream <<
    "### iScheme file error: " << message <<
    std::endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  std::stringstream s;

  s <<
    "### iScheme options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    std::endl;

  std::string message = s.str ();

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}


}
