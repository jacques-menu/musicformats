/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "oahEarlyOptions.h"

#include "wae.h"
#include "mfWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void mfWarning (
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeWarning (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}

void mfError (
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mfException (message);
    }
    else {
      throw mfException (message);
    }
  }
}

//______________________________________________________________________________
void mfInternalWarning (
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalWarning (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
