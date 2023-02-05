/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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
  // populate the interpreter's formats list
//   pConverterComponent->
//     appendRepresentationToMultiComponent (
//       createMsrRepresentationComponent ());

  // populate the interpreter's passes list
//   pConverterComponent->
//     appendPassToMultiComponent (
//       createMsr2msrComponent ());

}

//______________________________________________________________________________
S_mfcConverterComponent createIschemeInterpreterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
		std::stringstream ss;

    ss <<
      "Creating the ischeme component" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

    // create the interpreter component
    pConverterComponent =
      mfcConverterComponent::create (
        "ischeme",
        mfcMultiComponentEntropicityKind::kComponentEntropicityNo,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the interpreter's own history
    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.64"),
          "June 20, 2022",
          std::list<std::string> {
            "Created the iScheme interpreter component",
            "Cloned the MFSL interpreter into iScheme"
          }
      ));

    populateMultiComponent ();
  }

  return pConverterComponent;
}


}
