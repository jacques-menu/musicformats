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
#include "mfslWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfslWarning (
  const std::string&       message,
  const mfsl::location& loc)
{
  gLog <<
    "### [MFSL] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void mfslError (
  const std::string&       message,
  const mfsl::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [MFSL] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslInternalError (
  const std::string&       message,
  const mfsl::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [MFSL] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### MFSL file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}

EXP void mfslOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  stringstream s;

  s <<
    "### MFSL options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = s.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mfslException (message);
  }

  throw mfslException (message);
}


}
