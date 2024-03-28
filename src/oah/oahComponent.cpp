/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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
          mfcVersionNumber::createFromString ("0.9.65"),
          "August 16, 2022",
          std::list<std::string> {
            "Start of sequential versions numbering",
            "Finalized option '-find' handling"
          }
      ));

    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "October 4, 2022",
          std::list<std::string> {
            "Added oahElementValueKind::kElementValue_UNKNOWN_ to fix an initialization issue"
          }
      ));

    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "January 3, 2023",
          std::list<std::string> {
            "Added the '-language, -lang' option"
          }
      ));

    pOahComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.67"),
          "February 5, 2023",
          std::list<std::string> {
            "Switched 'tracing' to 'trace' everywhere"
          }
      ));
  }

  return pOahComponent;
}


}
