/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msr2msrComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMsr2msrComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the msr2msr pass component" <<
        endl;
    }
#endif

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "msr2msr");

    // populate it
    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          list<string> {
            "Start of sequential versions numbering"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "November 15, 2021",
          list<string> {
            "Added '-insert-page-break-after-measure, -ipbam' option",
            "Added '-avoid-msr2msr, -am2m' option (for TESTS)",
            "Added 'generate-stems-directions, ksd' option to lpsr2lilypond"
          }
      ));
  }

  return pPassComponent;
}


}
