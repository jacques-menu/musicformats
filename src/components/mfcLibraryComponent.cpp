/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
    "v" +
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

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Creating the MusicFormats library component" <<
        ", " <<
        getGlobalMusicFormatsVersionNumber <<
        ", " <<
        getGlobalMusicFormatsVersionDate <<
        std::endl;
    }
#endif

    // create the library's history
    pLibraryComponent =
      mfcLibraryComponent::create (
        "MusicFormats",
        mfcMultiComponentEntropicityKind::kComponentEntropicityYes,
        mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes); // JMI ???

    // populate the library's representations std::list
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

    // populate the library's passes std::list
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

    // populate the library's converters std::list
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
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.51"),
          "October 12, 2021",
          std::list<std::string> {
            "Adding a version number to the MusicFormats library",
            "Fixed trace OAH issue in the musicxml2* converters)"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.52"),
          "October 12, 2021",
          std::list<std::string> {
            "Added MusicFormats library versions history to '-hist, -history'"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.53"),
          "October 22, 2021",
          std::list<std::string> {
            "Replaced bool by class   Bool in variables and fields",
            "Created MFC (MusicFormats components)"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.54"),
          "Novermber 6, 2021",
          std::list<std::string> {
            "Replaced std::cout and std::cerr by gOutputStream and gLogStream respectively in the CLI samples",
            "Finalized components numbering (MFC)"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.55"),
          "Novermber 6, 2021",
          std::list<std::string> {
            "Added options to lpsr2lilypond"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.56"),
          "Novermber 6, 2021",
          std::list<std::string> {
            "Fixed UTF-16 encoding detection in mxsr2msr"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.57"),
          "December 12, 2021",
          std::list<std::string> {
            "Finalized MusicXML data encoding check against UTF-8",
            "Added a std::list of the converters to the library component",
            "Redistributed WAE files in the code base",
            "Added the ' -mf-version, -mfv' and '-mf-history, -mfhist' options",
            "Finalized early options handling"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.58"),
          "January 2, 2022",
          std::list<std::string> {
            "Finalized the OAH documentation and macros handling",
            "Fixed an issue in options and argumentss handling",
            "Fixed generated output options handling in Mikrokosmos3Wandering and LilyPondIssue34",
            "Finalized the class   names in oahAtomsCollection.h/.cpp",
            "Moved from compiler writing terminology to more musicians-friendly terms"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.59"),
          "January 4, 2022",
          std::list<std::string> {
            "Fixed an issue in strings quoting",
            "Finalized the components terminology",
            "Finalized oahAtomImplicitlyStoringAValue"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.60"),
          "February 14, 2022",
          std::list<std::string> {
            "Finalized the handling of option '-include'",
            "Finalized the options values display options",
            "Added distributions for MacOS, Ubuntu and Windows",
            "Finalized multi-generation output handling",
            "Complements to the user guide"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.61"),
          "February 18, 2022",
          std::list<std::string> {
            "Complements to the user guide"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.62"),
          "March 28, 2022",
          std::list<std::string> {
            "Finalized ragged output handling"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.65"),
          "Augus 24, 2022",
          std::list<std::string> {
            "Upgraded MusicXML from version 3.1 to 4.0",
            "Switched to GitHub's release management",
            "Ready-to-use versions installation and use documentation complements"
          }
      ));

    pLibraryComponent->
      appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.66"),
          "September 29, 2022",
          std::list<std::string> {
            "Finalized the 'std::string existing*()' functions"
          }
      ));
 }

  return pLibraryComponent;
}


}
