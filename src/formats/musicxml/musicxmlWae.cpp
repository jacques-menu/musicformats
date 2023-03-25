/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"


#include "waeInterface.h"
#include "musicxmlWae.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void musicxmlWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "MusicXML",
    inputSourceName,
    inputLineNumber,
    message);
}

void musicxmlError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "MusicXML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw musicxmlException (message);
    }
    else {
      throw musicxmlException (message);
    }
  }
}


}
