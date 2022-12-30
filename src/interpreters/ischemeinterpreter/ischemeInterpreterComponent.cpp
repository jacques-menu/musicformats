/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating the ischeme component" <<
      std::endl;
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
