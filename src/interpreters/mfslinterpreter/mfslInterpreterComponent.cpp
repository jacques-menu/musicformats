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
  // populate the interpreter's representations list
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createMsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createLpsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createBsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createMxsrRepresentationComponent ());

  // populate the interpreter's passes list
  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2msrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2lpsrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createLpsr2lilypondComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2bsrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createBsr2bsrComponent ());
  pConverterComponent->
    appendPassToMultiComponent (
      createBsr2brailleComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2mxsrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMxsr2musicxmlComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMxsr2guidoComponent ());
}

//______________________________________________________________________________
S_mfcConverterComponent createMfslInterpreterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
		std::stringstream ss;

    ss <<
      "Creating the mfsl component" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

    // create the interpreter component
    pConverterComponent =
      mfcConverterComponent::create (
        "mfsl",
        mfcMultiComponentEntropicityKind::kComponentEntropicityNo,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the interpreter's own history
    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "March 9, 2022",
          std::list<std::string> {
            "Created the MFSL component"
          }
      ));

    // populate the interpreter's own history
    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.63"),
          "March 9, 2022",
          std::list<std::string> {
            "Finalized the syntax and semantics of MFSL, added '-input' option"
          }
      ));

    populateMultiComponent ();
  }

  return pConverterComponent;
}


}
