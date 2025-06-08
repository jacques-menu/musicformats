/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "../../MusicFormatsVersionNumber.h"

#include "../../MusicFormatsVersionDate.h"

#include "mfIndentedTextOutput.h"

#include "mfcComponents.h"


namespace MusicFormats
{

//______________________________________________________________________________
const std::string pMusicFormatsVersionNumber =
  MUSICFORMATS_VERSION_NUMBER;

const std::string pMusicFormatsVersionDate =
  MUSICFORMATS_VERSION_DATE;

EXP extern std::string getGlobalMusicFormatsVersionNumber ()
{
  return pMusicFormatsVersionNumber;
}

EXP extern std::string getGlobalMusicFormatsVersionDate ()
{
  return pMusicFormatsVersionDate;
}

EXP extern std::string getGlobalMusicFormatsVersionNumberAndDate ()
{
  std::string result;

  return
    pMusicFormatsVersionNumber +
    " (" +
    pMusicFormatsVersionDate +
    ")";
}

//______________________________________________________________________________
S_mfcLibraryComponent createLibraryComponent ()
{
  static S_mfcLibraryComponent pLibraryComponent;

  // protect library against multiple initializations
  if (! pLibraryComponent) {

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "Creating the MusicFormats library component" <<
        ", " <<
        getGlobalMusicFormatsVersionNumber <<
        ", " <<
        getGlobalMusicFormatsVersionDate;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the library's history
    pLibraryComponent =
      mfcLibraryComponent::create (
        "MusicFormats",
        mfcMultiComponentEntropicityKind::kComponentEntropicityYes,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the library's representations list
    pLibraryComponent->
      appendRepresentationToMultiComponent (
        createMsrRepresentationComponent ());
    pLibraryComponent->
      appendRepresentationToMultiComponent (
        createLpsrRepresentationComponent ());
    pLibraryComponent->
      appendRepresentationToMultiComponent (
        createBsrRepresentationComponent ());
    pLibraryComponent->
      appendRepresentationToMultiComponent (
        createMxsrRepresentationComponent ());

    // populate the library's passes list
    pLibraryComponent->
      appendPassToMultiComponent (
        createMsr2msrComponent ());

    pLibraryComponent->
      appendPassToMultiComponent (
        createMsr2lpsrComponent ());
    pLibraryComponent->
      appendPassToMultiComponent (
        createLpsr2lilypondComponent ());

    pLibraryComponent->
      appendPassToMultiComponent (
        createMsr2bsrComponent ());
    pLibraryComponent->
      appendPassToMultiComponent (
        createBsr2bsrComponent ());
    pLibraryComponent->
      appendPassToMultiComponent (
        createBsr2brailleComponent ());

    pLibraryComponent->
      appendPassToMultiComponent (
        createMsr2mxsrComponent ());

    pLibraryComponent->
      appendPassToMultiComponent (
        createMxsr2musicxmlComponent ());

    pLibraryComponent->
      appendPassToMultiComponent (
        createMxsr2guidoComponent ());

    // populate the library's converters list
    pLibraryComponent->
      appendConverterToMultiComponent (
        createMusicxml2lilypondConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMusicxml2brailleConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMusicxml2musicxmlConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMusicxml2guidoConverterComponent ());


    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsdl2lilypondConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsdl2brailleConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsdl2musicxmlConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsdl2guidoConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsdlConverterConverterComponent ());


    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsr2lilypondConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsr2brailleConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsr2musicxmlConverterComponent ());

    pLibraryComponent->
      appendConverterToMultiComponent (
        createMsr2guidoConverterComponent ());

    // populate the library's own history
    pLibraryComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list <std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pLibraryComponent->
      appendVersionToComponent (
        mfcVersion::create (
          mfcVersionNumber::createFromString ("0.9.70"),
          "February 20, 2024",
          std::list <std::string> {
            "Many, many changes in the code base"
          }
      ));
  }

  return pLibraryComponent;
}


}
