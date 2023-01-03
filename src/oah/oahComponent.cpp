/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "oahComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcOahComponent createOahComponent ()
{
  static S_mfcOahComponent pOahComponent;

  // protect library against multiple initializations
  if (! pOahComponent) {

#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the OAH component" <<
        std::endl;
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
          std::list<std::string> {
            "Start of sequential versions numbering",
            "Finalized option '-find' handling"
          }
      ));

    pOahComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "October 4, 2022",
          std::list<std::string> {
            "Added oahElementValueKind::kElementValueUnknown to fix an initialization issue"
          }
      ));

    pOahComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "January 3, 2023",
          std::list<std::string> {
            "Added the '-language, -lang' option"
          }
      ));
  }

  return pOahComponent;
}


}
