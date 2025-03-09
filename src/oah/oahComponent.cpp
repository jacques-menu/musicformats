/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "oahEarlyOptions.h"
#include "oahComponent.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcOahComponent createOahComponent ()
{
  static S_mfcOahComponent pOahComponent;

  // protect library against multiple initializations
  if (! pOahComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "Creating the OAH component";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the format component
    pOahComponent =
      mfcOahComponent::create (
        "OAH");

    // populate it
    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list <std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.70"),
          "February 20, 2024",
          std::list <std::string> {
            "Many, many changes in the code base"
          }
      ));

    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "June 25, 2024",
          std::list <std::string> {
            "Finalized the OAH introspective options names to '-help-about-option-name, -name', '-query-help, -query', '-find-string-in-help, -find' and '-match-help-with-pattern, -match'"
          }
      ));
  }

  return pOahComponent;
}


}
