/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

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
      createMxsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createMsrRepresentationComponent ());
  pConverterComponent->
    appendRepresentationToMultiComponent (
      createLpsrRepresentationComponent ());

  // populate the converter's passes list
  pConverterComponent->
    appendPassToMultiComponent (
      createMxsr2msrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2msrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createMsr2lpsrComponent ());

  pConverterComponent->
    appendPassToMultiComponent (
      createLpsr2lilypondComponent ());
}

//______________________________________________________________________________
EXP S_mfcConverterComponent createMusicxml2lilypondConverterComponent ()
{
  // protect library against multiple initializations
  if (! pConverterComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the musicxml2lilypond component" <<
        endl;
    }
#endif

    // create the converter component
    pConverterComponent =
      mfcConverterComponent::create (
        "musicxml2lilypond",
        mfcMultiComponentEntropicityKind::kComponentEntropicityYes,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the converter's own history
    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "October 12, 2021",
          list<string> {
            "Fixed trace OAH issue"
          }
      ));

    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "November 29, 2021",
          list<string> {
            "Finalized MusicXML data encoding check against UTF-8"
          }
      ));

    pConverterComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.60"),
          "February 21, 2022",
          list<string> {
            "Added options for LilyPond generation of words"
          }
      ));

    populateMultiComponent ();
}

  return pConverterComponent;
}


}
