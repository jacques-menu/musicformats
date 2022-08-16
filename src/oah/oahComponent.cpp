/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "oahComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcOahComponent createOahComponent ()
{
  static S_mfcOahComponent pOahComponent;

  // protect library against multiple initializations
  if (! pOahComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the OAH component" <<
        endl;
    }
#endif

    // create the format component
    pOahComponent =
      mfcOahComponent::create (
        "OAH");

    // populate it
    pOahComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.65"),
          "August 16, 2022",
          list<string> {
            "Start of sequential versions numbering",
            "Finalized option '-find' handling"
          }
      ));
  }

  return pOahComponent;
}


}
