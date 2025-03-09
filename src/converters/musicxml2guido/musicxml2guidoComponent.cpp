/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfcComponents.h"


namespace MusicFormats
{

static S_mfcConverterComponent pConverterComponent;

//______________________________________________________________________________
static void populateMultiComponent ()
{
  // populate the converter's representations list
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createMxsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createMsrRepresentationComponent ());

  // populate the converter's passes list
  pConverterComponent->
    appendPassToMultiComponent (
      createMxsr2msrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2msrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2mxsrComponent ());
  pConverterComponent->
    appendPassToMultiComponent (
      createMxsr2guidoComponent ());
}

//______________________________________________________________________________
EXP S_mfcConverterComponent createMusicxml2guidoConverterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "Creating the musicxml2guido component";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the converter component
    pConverterComponent =
      mfcConverterComponent::create (
        "musicxml2guido",
        mfcMultiComponentEntropicityKind::kComponentEntropicityYes,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the converter's own history
    pConverterComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list <std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pConverterComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.70"),
          "February 20, 2024",
          std::list <std::string> {
            "Many, many changes in the code base"
          }
      ));

    populateMultiComponent ();
  }

  return pConverterComponent;
}


}
