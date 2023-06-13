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
#include "mfFindWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfFindWarning (
  const std::string&       message,
  const mfFind::location& loc)
{
  gLog <<
    "### [mfFind] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void mfFindError (
  const std::string&       message,
  const mfFind::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [mfFind] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFindException (message);
  }

  throw mfFindException (message);
}

EXP void mfFindInternalError (
  const std::string&       message,
  const mfFind::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [mfFind] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFindException (message);
  }

  throw mfFindException (message);
}

EXP void mfFindFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### mfFind file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFindException (message);
  }

  throw mfFindException (message);
}

EXP void mfFindOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  stringstream s;

  s <<
    "### mfFind options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = s.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfFindException (message);
  }

  throw mfFindException (message);
}


}
