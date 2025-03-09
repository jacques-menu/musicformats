/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "lpsr2lilypondComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createLpsr2lilypondComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "Creating the lpsr2lilypond pass component";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "lpsr2lilypond");

    // populate it
    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list <std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.70"),
          "February 20, 2024",
          std::list <std::string> {
            "Many, many changes in the code base"
          }
      ));

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "April 15, 2024",
          std::list <std::string> {
            "Added '<<' and '>>' around staff groups contents"
          }
      ));
  }

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "April 23, 2024",
          std::list <std::string> {
            "Removed the generation of bar and barnumber checks after the last measure",
            "Disabled the generation of staff changes TEMPORARILY"
          }
      ));

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "June 9, 2024",
          std::list <std::string> {
            "Fixed an issue in LilyPond note heads generation"
          }
      ));

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "June 11, 2024",
          std::list <std::string> {
            "Fixed '\new Lyrics' generation"
          }
      ));

    pPassComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.71"),
          "June 26, 2024",
          std::list <std::string> {
            "Added missing skips in LilyPond lyrics generation"
          }
      ));

  return pPassComponent;
}


}
