/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mxsr2msrComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcPassComponent createMxsr2msrComponent ()
{
  static S_mfcPassComponent pPassComponent;

  // protect library against multiple initializations
  if (! pPassComponent) {

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
	  	std::stringstream ss;

      ss <<
        "Creating the mxsr2msr pass component" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
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
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "November 27, 2021",
          std::list<std::string> {
            "Made missing <divisions/> to be interpreted a 1 by default"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "January 29, 2022",
          std::list<std::string> {
            "Moved barlines to a pending list instead of being appended to the part at once",
            "Fixed a long-standing issue in barlines handling at the beginning of repeat alternatives"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.60"),
          "February 21, 2022",
          std::list<std::string> {
            "Added options for words conversions and placement"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.61"),
          "February 21, 2022",
          std::list<std::string> {
            "Finalized options -replicate-empty-lpsr-measure and add-empty-msr-measures",
            "Added conversion of MusicXML words to cresc/decresc"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "March 31, 2022",
          std::list<std::string> {
            "Fixed an issue in scordaturas handling"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.63"),
          "April 14, 2022",
          std::list<std::string> {
            "Fixed missing final barline issue",
            "Fixed an octave shifts handling issue"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.64"),
          "July 22, 2022",
          std::list<std::string> {
            "Finalized the conversion of MusicXML <credit /> markups to MSR"
          }
      ));

    pPassComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "October 3, 2022",
          std::list<std::string> {
            "Permuted the order of populateNote() and handleNoteItself() in mxsr2msrTranslator::visitEnd (S_note& elt)"
          }
      ));
  }

  return pPassComponent;
}


}
