/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <stdlib.h>
#include <iostream>

#include "mfIndentedTextOutput.h"

#include "mfcLibraryComponent.h"


using namespace MusicFormats;

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

  // display the library components history
  // ------------------------------------------------------
  createLibraryComponent ()->
    printHistory (gLogStream);

  return 0;
}
