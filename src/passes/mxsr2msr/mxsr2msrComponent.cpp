/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mxsr2msrComponent.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMxsr2msrComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "Creating the mxsr2msr pass component" <<
        endl;
    }
#endif

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "mxsr2msr");

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
          "November 27, 2021",
          list<string> {
            "Made missing <divisions/> to be interpreted a 1 by default"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "January 29, 2022",
          list<string> {
            "Moved barlines to a pending list instead of being appended to the part at once",
            "Fixed a long-standing issue in barlines handling at the beginning of repeat alternatives"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.60"),
          "February 21, 2022",
          list<string> {
            "Added options for words conversions and placement"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.61"),
          "February 21, 2022",
          list<string> {
            "Finalized options -replicate-empty-lpsr-measure and add-empty-msr-measures",
            "Added conversion of MusicXML words to cresc/decresc"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "March 31, 2022",
          list<string> {
            "Fixed an issue in scordaturas handling"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.63"),
          "April 14, 2022",
          list<string> {
            "Fixed missing final barline issue",
            "Fixed an octave shifts handling issue"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.64"),
          "July 22, 2022",
          list<string> {
            "Finalized the conversion of MusicXML <credit /> markups to MSR"
          }
      ));
  }

  return pPassComponent;
}


}
