/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdl2msrComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMsdl2msrComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the msdl2msr pass component" <<
        endl;
    }
#endif

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "msdl2msr");

    // populate it
    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));
  }

  return pPassComponent;
}


}
