/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "lpsr2lilypondComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createLpsr2lilypondComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "Creating the lpsr2lilypond pass component" <<
        std::endl;

      gWaeHandler->waeTraceWithoutLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the pass component
    pPassComponent =
      mfcPassComponent::create (
        "lpsr2lilypond");

    // populate it
    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "November 23, 2021",
          std::list<std::string> {
            "Added '-lilypond-dynamics-text-spanners-style, -dtss' option"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "December 16, 2021",
          std::list<std::string> {
            "Finalized harmonies duration generation",
            "Added kVoiceKindDynamics to enum class msrVoiceKind"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.60"),
          "February 21, 2022",
          std::list<std::string> {
            "Finalized paper options handling"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.63"),
          "May 31, 2022",
          std::list<std::string> {
            "Fixed an issue in octave shift handling",
            "Replace \\fermataMarkup by \\fermata in R* multiple full-bar rests",
            "Fixed bug in absolute and relative octaves generation"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.64"),
          "May 31, 2022",
          std::list<std::string> {
            "Finalized the generation of LilyPond code for full-bar rests"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.65"),
          "August 2, 2022",
          std::list<std::string> {
            "Added support for '\"print-object=\"no\"' in notes",
            "Removed extraneous '(' in tempo beat units equivalence code generation"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "December 18, 2022",
          std::list<std::string> {
            "Fixed octave shift LilyPond code generation"
          }
      ));
  }

  return pPassComponent;
}


}
