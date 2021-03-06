/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "msdlComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcRepresentationComponent getMsdlRepresentationComponent ()
{
  static S_mfcRepresentationComponent pRepresentationComponent;

  // protect library against multiple initializations
  if (! pRepresentationComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the MSDL format component" <<
        endl;
    }
#endif

    // create the format component
    pRepresentationComponent =
      mfcRepresentationComponent::create (
        "MSDL");

    // populate it
    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));
  }

  return pRepresentationComponent;
}


}
