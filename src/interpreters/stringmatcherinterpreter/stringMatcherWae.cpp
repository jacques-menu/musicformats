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
#include "stringMatcherWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void stringMatcherWarning (
  const std::string&       message,
  const smi::location& loc)
{
  gLog <<
    "### [stringMatcher] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void stringMatcherError (
  const std::string&       message,
  const smi::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [stringMatcher] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringMatcherException (message);
  }

  throw stringMatcherException (message);
}

EXP void stringMatcherInternalError (
  const std::string&       message,
  const smi::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [stringMatcher] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringMatcherException (message);
  }

  throw stringMatcherException (message);
}

EXP void stringMatcherFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### stringMatcher file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringMatcherException (message);
  }

  throw stringMatcherException (message);
}

EXP void stringMatcherOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  std::stringstream ss;

  ss <<
    "### stringMatcher options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = ss.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringMatcherException (message);
  }

  throw stringMatcherException (message);
}


}
