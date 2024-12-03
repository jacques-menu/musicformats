/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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
  // populate the interpreter's formats list
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
S_mfcConverterComponent createstringMatcherInterpreterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    gLog <<
      "Creating the stringMatcher component" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

    // create the interpreter component
    pConverterComponent =
      mfcConverterComponent::create (
        "stringMatcher",
        mfcMultiComponentEntropicityKind::kComponentEntropicityNo,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the interpreter's own history
    // populate it
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
