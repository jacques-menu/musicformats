/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeOah.h"
#include "ischemeWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void ischemeWarning (
  const std::string&       message,
  const stringMatcher::location& loc)
{
  gLog <<
    "### [iScheme] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void ischemeError (
  const std::string&       message,
  const stringMatcher::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [iScheme] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeInternalError (
  const std::string&       message,
  const stringMatcher::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [iScheme] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### iScheme file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  std::stringstream ss;

  ss <<
    "### iScheme options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = ss.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}


}
