/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesGerman___
#define ___languagesGerman___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageGerman : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageGerman> create ();

  public:

    // const overrideructors/destructor
    // ------------------------------------------------------

                          languageGerman ();

    virtual               ~languageGerman ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

//     // pass names
//
//     std::string          passIDKindAsString (mfPassIDKind passIDKind) const override;
//
//     std::string          passOptional () const override;
//
//     std::string          fullVersion () const override;
//     std::string          summary () const override;
//     std::string          names () const override;
//     std::string          slices () const override;
//
//     // quitting after passes
//
//     std::string          quittingAfterPass (mfPassIDKind passIDKind) const override;
//
//     // OAH
//
//     std::string          handleOptionsAndArgumentsFromArgcArgv () const override;
//
//     // passes
//
//     std::string          convertAMusicXMLStreamIntoAnMXSR () const override;
//     std::string          convertAMusicXMLDescriptorIntoAnMXSR () const override;
//     std::string          convertAMusicXMLBufferIntoAnMXSR () const override;
//
//     std::string          convertTheMXSRIntoAnMSRSkeleton () const override;
//
//     std::string          populateTheMSRSkeletonFromMusicXMLData () const override;
//
//     std::string          convertTheFirstMSRIntoASecondMSR () const override;
//
//     std::string          convertTheSecondMSRIntoAnLPSR () const override;
//     std::string          convertTheFirstMSRIntoAnLPSR () const override;
//
//     std::string          convertTheLPSRIntoLilyPondCode () const override;
//
//     // display
//
//     std::string          displayTheFirstMSRSkeletonAsText () const override;
//     std::string          displayTheFirstMSRAsText () const override;
//     std::string          displayTheSecondMSRAsText () const override;
//     std::string          displayTheMSREmbededInTheLPSRAsText () const override;
//     std::string          displayTheLPSRAsText () const override;
//
//     std::string          displayTheNamesInTheFirstMSR () const override;
//     std::string          displayTheNamesInTheSecondMSR () const override;
//
//     std::string          displayASummaryOfTheFirstMSR () const override;
//     std::string          displayASummaryOfTheSecondMSR () const override;
//
//     std::string          displayAFlatViewOfTheFirstMSR () const override;
//     std::string          displayAFlatViewOfTheSecondMSR () const override;
//     std::string          displayAFlatViewOfTheLPSR () const override;

//     std::string          displayTheSlicesOfTheFirstMSR () const override;
//     std::string          displayTheSlicesOfTheSecondMSR () const override;
//     std::string          displayTheSlicesOfTheLPSR () const override;

//     // timing
//
//     std::string          timingInformation () const override;
//     std::string          activity () const override;
//     std::string          description () const override;
//     std::string          kind () const override;
//     std::string          CPUSeconds () const override;
//     std::string          mandatory () const override;
//     std::string          optional () const override;
//     std::string          totalSeconds () const override;
//
//     // options files
//
//     std::string          openingOptionsFileForReading (
//                             const std::string& optionsFileName) const override;
//     std::string          openingIncludeFileForReading (
//                             const std::string& includeFileName) const override;
//
//     std::string          cannotOpenOptionsFileForReading (
//                             const std::string& optionsFileName) const override;
//
//     std::string          closingOptionsFile (
//                             const std::string& optionsFileName) const override;
//
//     // output files
//
//     std::string          openingLilypondFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          openingBrailleMusicFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          openingMusicXMLFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          openingGuidoFileForWriting (
//                             const std::string& inputFileName) const override;
//
//     std::string          cannotOpenLilypondFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          cannotOpenBrailleMusicFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          cannotOpenMusicXMLFileForWriting (
//                             const std::string& inputFileName) const override;
//     std::string          cannotOpenGuidoFileForWriting (
//                             const std::string& inputFileName) const override;
//
//     std::string          closingLilypondFile (
//                             const std::string& outputFileName) const override;
//     std::string          closingBrailleMusicFile (
//                             const std::string& outputFileName) const override;
//     std::string          closingMusicXMLFile (
//                             const std::string& outputFileName) const override;
//     std::string          closingGuidoFile (
//                             const std::string& outputFileName) const override;
//
//     // include files
//
//     std::string          cannotOpenIncludeFileForReading (
//                             const std::string& includeFileName) const override;
//
//     // scripts
//
//     std::string          cannotOpenScriptForWriting (
//                             const std::string& scriptName) const override;
//
//     // MSDL
//
//     std::string          cannotOpenMSDLFileForReading (
//                             const std::string& inputFileName) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<languageGerman> S_languageGerman;
EXP std::ostream& operator << (std::ostream& os, const S_languageGerman& elt);


}


#endif // ___languagesGerman___
