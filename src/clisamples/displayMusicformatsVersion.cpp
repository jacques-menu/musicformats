/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

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
