/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h" // for gLogStream

#include "msrComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcRepresentationComponent createMsrRepresentationComponent ()
{
  static S_mfcRepresentationComponent pRepresentationComponent;

  // protect library against multiple initializations
  if (! pRepresentationComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Initializing MSR format component" <<
        endl;
    }
#endif

    // create the format component
    pRepresentationComponent =
      mfcRepresentationComponent::create (
        "MSR");

    // populate it
    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "October 14, 2021",
          list<string> {
            "Refined MSR names and summary display options"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "November 27, 2021",
          list<string> {
            "Finalized harmonies handling in MSR"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.61"),
          "November 27, 2021",
          list<string> {
            "Added msrCrescDecresc to MSR"
          }
      ));
  }

  return pRepresentationComponent;
}


}
