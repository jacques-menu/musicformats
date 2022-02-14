/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfcComponents.h"


using namespace std;

namespace MusicFormats
{

static S_mfcConverterComponent pConverterComponent;

//______________________________________________________________________________
static void populateMultiComponent ()
{
  // populate the converter's formats list
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
      createMxsr2musicxmlComponent ());
}

//______________________________________________________________________________
S_mfcConverterComponent createMsdl2musicxmlConverterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the msdl2musicxml component" <<
        endl;
    }
#endif

    // create the converter component
    pConverterComponent =
      mfcConverterComponent::create (
        "msdl2musicxml",
        mfcMultiComponentEntropicityKind::kComponentEntropicityNo,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the converter's own history
    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));

    populateMultiComponent ();
  }

  return pConverterComponent;
}



}
