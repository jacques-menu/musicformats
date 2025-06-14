/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "msdrComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcRepresentationComponent getMsdrRepresentationComponent ()
{
  static S_mfcRepresentationComponent pRepresentationComponent;

  // protect library against multiple initializations
  if (! pRepresentationComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "Create the MSDR format component";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the format component
    pRepresentationComponent =
      mfcRepresentationComponent::create (
        "MSDR");

    // populate it
    pRepresentationComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list <std::string> {
            "Start of sequential versions numbering"
          }
      ));
  }

  return pRepresentationComponent;
}


}
