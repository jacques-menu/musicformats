/*

  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This file is provided as an example of the MusicXML Library use.
*/

#include <stdlib.h>
#include <iostream>

#include "mfIndentedTextOutput.h"

#include "mfcLibraryComponent.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (cout, cerr);

  // display the library components versions
  // ------------------------------------------------------
  createLibraryComponent ()->
    printVersion (gLogStream);

  return 0;
}
