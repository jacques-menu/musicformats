/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msr2bsrComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMsr2bsrComponent ()
{
  static S_mfcPassComponent pPassComponent;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating the msr2bsr pass component";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));
  }

  return pPassComponent;
}


}
