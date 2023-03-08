/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "oahEarlyOptions.h"

#include "waeInterface.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void mfWarning (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeWarning (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}

void mfError (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mfException (message);
    }
    else {
      throw mfException (message);
    }
  }
}

//______________________________________________________________________________
void mfInternalWarning (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "MF",
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
