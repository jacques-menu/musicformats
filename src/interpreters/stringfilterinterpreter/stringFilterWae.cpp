/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeOah.h"
#include "stringFilterWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void stringFilterWarning (
  const std::string&       message,
  const stringfilter::location& loc)
{
  gLog <<
    "### [stringFilter] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void stringFilterError (
  const std::string&       message,
  const stringfilter::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [stringFilter] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringFilterException (message);
  }

  throw stringFilterException (message);
}

EXP void stringFilterInternalError (
  const std::string&       message,
  const stringfilter::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [stringFilter] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringFilterException (message);
  }

  throw stringFilterException (message);
}

EXP void stringFilterFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### stringFilter file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringFilterException (message);
  }

  throw stringFilterException (message);
}

EXP void stringFilterOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  stringstream s;

  s <<
    "### stringFilter options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = s.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw stringFilterException (message);
  }

  throw stringFilterException (message);
}


}
