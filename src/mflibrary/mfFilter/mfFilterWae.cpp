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
#include "mfFilterWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfFilterWarning (
  const std::string&       message,
  const mfFilter::location& loc)
{
  gLog <<
    "### [mfFilter] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void mfFilterError (
  const std::string&       message,
  const mfFilter::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [mfFilter] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFilterException (message);
  }

  throw mfFilterException (message);
}

EXP void mfFilterInternalError (
  const std::string&       message,
  const mfFilter::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [mfFilter] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFilterException (message);
  }

  throw mfFilterException (message);
}

EXP void mfFilterFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### mfFilter file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFilterException (message);
  }

  throw mfFilterException (message);
}

EXP void mfFilterOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  stringstream s;

  s <<
    "### mfFilter options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = s.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFilterException (message);
  }

  throw mfFilterException (message);
}


}
