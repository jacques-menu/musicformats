/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h" // for gLogStream

#include "msrComponent.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfcRepresentationComponent createMsrRepresentationComponent ()
{
  static S_mfcRepresentationComponent pRepresentationComponent;

  // protect library against multiple initializations
  if (! pRepresentationComponent) {

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
	  	std::stringstream ss;

      ss <<
        "Initializing MSR format component" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
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
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "October 14, 2021",
          std::list<std::string> {
            "Refined MSR names and summary display options"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "November 27, 2021",
          std::list<std::string> {
            "Finalized harmonies handling in MSR"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.61"),
          "November 27, 2021",
          std::list<std::string> {
            "Added msrCrescDecresc to MSR"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "November 27, 2021",
          std::list<std::string> {
            "Added measure end line numbers to MSR"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.63"),
          "May 17, 2022",
          std::list<std::string> {
            "Introduced msrSegmentComponent in MSR"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.64"),
          "July 22, 2022",
          std::list<std::string> {
            "Fixed unchechecked smart pointers in msrNotes.cpp",
            "Finalized the representation of full-bar rests"
          }
      ));

    pRepresentationComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "October 6, 2022",
          std::list<std::string> {
            "Finalized the computation of measure positions",
            "Finalized the '-display-msr-*' options"
          }
      ));
  }

  return pRepresentationComponent;
}


}
