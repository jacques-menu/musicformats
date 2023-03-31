/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfcComponents.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"

#include "Mikrokosmos3WanderingComponent.h"


namespace MusicFormats
{

static S_mfcGeneratorComponent pGeneratorComponent;

//______________________________________________________________________________
static void populateMultiComponent ()
{
  // populate the generator's representations list
  pGeneratorComponent->
    appendRepresentationToMultiComponent (
      createMsrRepresentationComponent ());
  pGeneratorComponent->
    appendRepresentationToMultiComponent (
      createLpsrRepresentationComponent ());
  pGeneratorComponent->
    appendRepresentationToMultiComponent (
      createBsrRepresentationComponent ());
  pGeneratorComponent->
    appendRepresentationToMultiComponent (
      createMxsrRepresentationComponent ());

  // populate the converter's passes list
  pGeneratorComponent->
    appendPassToMultiComponent (
      createMsr2msrComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createMsr2lpsrComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createLpsr2lilypondComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createMsr2bsrComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createBsr2bsrComponent ());
  pGeneratorComponent->
    appendPassToMultiComponent (
      createBsr2brailleComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createMsr2mxsrComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createMxsr2musicxmlComponent ());

  pGeneratorComponent->
    appendPassToMultiComponent (
      createMxsr2guidoComponent ());
}

//______________________________________________________________________________
S_mfcGeneratorComponent createMikrokosmos3WanderingGeneratorComponent ()
{
  // protect library against multiple initializations
  if (! pGeneratorComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "Crerating the Mikrokosmos3Wandering component" <<
        std::endl;

      gWaeHandler->waeTraceWithoutLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the generator component
    pGeneratorComponent =
      mfcGeneratorComponent::create (
        "Mikrokosmos3Wandering",
        mfcMultiComponentEntropicityKind::kComponentEntropicityYes,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the generators's own history
    pGeneratorComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pGeneratorComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.6"),
          "October 23, 2021",
          std::list<std::string> {
            "Fixed generated output options handling"
          }
      ));

    populateMultiComponent ();
  }

  return pGeneratorComponent;
}


}
