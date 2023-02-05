/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersGerman___
#define ___mfWaeHandlersGerman___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerGerman : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerGerman> create ();

  public:

    // const overrideructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerGerman ();

    virtual               ~mfWaeHandlerGerman ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass names

    virtual std::string   passIDKindAsString (mfPassIDKind passIDKind) const override;

    virtual std::string   passOptional () const override;

    virtual std::string   fullVersion () const override;
    virtual std::string   summary () const override;
    virtual std::string   names () const override;
    virtual std::string   slices () const override;

    // quitting after passes

    virtual std::string   quittingAfterPass (mfPassIDKind passIDKind) const override;

    // OAH

    virtual std::string   handleOptionsAndArgumentsFromArgcArgv () const override;

    // passes

    virtual std::string   createAnMXSRFromAMusicXMLFileOrStdin () const override;
    virtual std::string   createAnMXSRFromAMusicXMLDescriptor () const override;
    virtual std::string   createAnMXSRFromAMusicXMLBuffer () const override;

    virtual std::string   createAnMSRSqueletonFromTheMXSR () const override;

    virtual std::string   populateTheMSRSqueletonFromMusicXMLData () const override;

    virtual std::string   convertTheFirstMSRIntoASecondMSR () const override;

    virtual std::string   convertTheSecondMSRIntoAnLPSR () const override;

    virtual std::string   convertTheLPSRIntoLilyPondCode () const override;

    // display

    virtual std::string   displayTheFirstMSRSkeletonAsText () const override;
    virtual std::string   displayTheFirstMSRAsText () const override;
    virtual std::string   displayTheSecondMSRAsText () const override;
    virtual std::string   displayTheLPSRAsText () const override;

    virtual std::string   displayTheNamesInTheFirstMSR () const override;
    virtual std::string   displayTheNamesInTheSecondMSR () const override;

    virtual std::string   displayASummaryOfTheFirstMSR () const override;
    virtual std::string   displayASummaryOfTheSecondMSR () const override;

    // timing

    virtual std::string   timingInformation () const override;
    virtual std::string   activity () const override;
    virtual std::string   description () const override;
    virtual std::string   kind () const override;
    virtual std::string   CPUSeconds () const override;
    virtual std::string   mandatory () const override;
    virtual std::string   optional () const override;
    virtual std::string   totalSeconds () const override;

    // options files

    virtual std::string   openingOptionsFileForReading (
                            const std::string& optionsFileName) const override;
    virtual std::string   openingIncludeFileForReading (
                            const std::string& includeFileName) const override;

    virtual std::string   cannotOpenOptionsFileForReading (
                            const std::string& optionsFileName) const override;

    virtual std::string   closingOptionsFile (
                            const std::string& optionsFileName) const override;

    // output files

    virtual std::string   openingLilypondFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   openingBrailleMusicFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   openingMusicXMLFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   openingGuidoFileForWriting (
                            const std::string& inputFileName) const override;

    virtual std::string   cannotOpenLilypondFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   cannotOpenBrailleMusicFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   cannotOpenMusicXMLFileForWriting (
                            const std::string& inputFileName) const override;
    virtual std::string   cannotOpenGuidoFileForWriting (
                            const std::string& inputFileName) const override;

    virtual std::string   closingLilypondFile (
                            const std::string& outputFileName) const override;
    virtual std::string   closingBrailleMusicFile (
                            const std::string& outputFileName) const override;
    virtual std::string   closingMusicXMLFile (
                            const std::string& outputFileName) const override;
    virtual std::string   closingGuidoFile (
                            const std::string& outputFileName) const override;

    // include files

    virtual std::string   cannotOpenIncludeFileForReading (
                            const std::string& includeFileName) const override;

    // scripts

    virtual std::string   cannotOpenScriptForWriting (
                            const std::string& scriptName) const override;

    // MSDL

    virtual std::string   cannotOpenMSDLFileForReading (
                            const std::string& inputFileName) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfWaeHandlerGerman> S_mfWaeHandlerGerman;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerGerman& elt);


}


#endif
