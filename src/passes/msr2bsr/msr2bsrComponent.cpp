/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msr2bsrComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMsr2bsrComponent ()
{
  static S_mfcPassComponent pPassComponent;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating the msr2bsr pass component" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! pPassComponent) {

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "msr2bsr");

    // populate it
    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.5"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));
  }

  return pPassComponent;
}


}
