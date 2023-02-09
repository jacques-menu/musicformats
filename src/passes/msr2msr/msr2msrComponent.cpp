/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msr2msrComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMsr2msrComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
	  	std::stringstream ss;

      ss <<
        "Creating the msr2msr pass component" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "msr2msr");

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

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "November 15, 2021",
          std::list<std::string> {
            "Added '-insert-page-break-after-measure, -ipbam' option",
            "Added '-avoid-msr2msr, -am2m' option (for TESTS)",
            "Added 'generate-stems-directions, ksd' option to lpsr2lilypond"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "March 5, 2022",
          std::list<std::string> {
            "Added '-ignore-msr-voice, -imv' and '-keep-msr-voice, -kmv' options"
          }
      ));
  }

  return pPassComponent;
}


}
