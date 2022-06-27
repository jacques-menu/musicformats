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
#include "ischemeWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void ischemeWarning (
  const string&       message,
  const iscm::location& loc)
{
  gLogStream <<
    "### [iScheme] WARNING " <<
    loc << ": " << message <<
    endl;
}

EXP void ischemeError (
  const string&       message,
  const iscm::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [iScheme] ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeInternalError (
  const string&       message,
  const iscm::location& loc)
{
  gIndenter.resetToZero ();

  gLogStream <<
    "### [iScheme] INNTERNAL ERROR " <<
    loc << ": " << message <<
    endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeFileError (
  const string& inputSourceName,
  const string& message)
{
  gLogStream <<
    "### iScheme file error: " << message <<
    endl;

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}

EXP void ischemeOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2)
{
  stringstream s;

  s <<
    "### iScheme options incompatibility: " <<
    atom1->fetchNamesBetweenQuotes () <<
    " and " <<
    atom2->fetchNamesBetweenQuotes () <<
    " are incompatible" <<
    endl;

  string message = s.str ();

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw ischemeException (message);
  }

  throw ischemeException (message);
}


}
