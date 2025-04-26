/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeOah.h"
#include "mnx2mnxsrWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mnx2mnxsrWarning (
  const std::string&       message,
  const mnx2mnxsr::location& loc)
{
  gLog <<
    "### [MNX2MNXSR] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void mnx2mnxsrError (
  const std::string&       message,
  const mnx2mnxsr::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [MNX2MNXSR] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mnx2mnxsrException (message);
  }

  throw mnx2mnxsrException (message);
}

EXP void mnx2mnxsrInternalError (
  const std::string&       message,
  const mnx2mnxsr::location& loc)
{
  gIndenter.resetToZero ();

  gLog <<
    "### [MNX2MNXSR] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mnx2mnxsrException (message);
  }

  throw mnx2mnxsrException (message);
}

EXP void mnx2mnxsrFileError (
  const std::string& inputSourceName,
  const std::string& message)
{
  gLog <<
    "### MNX2MNXSR file error: " << message <<
    endl;

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mnx2mnxsrException (message);
  }

  throw mnx2mnxsrException (message);
}

EXP void mnx2mnxsrOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  std::stringstream ss;

  ss <<
    "### MNX2MNXSR options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  std::string message = ss.str ();

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw mnx2mnxsrException (message);
  }

  throw mnx2mnxsrException (message);
}


}
