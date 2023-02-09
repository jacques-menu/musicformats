/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languages___
#define ___languages___

#include <string>
#include <map>

#include "smartpointer.h"

#include "mfPasses.h"


namespace MusicFormats
{

// the MusicFormats languages
//______________________________________________________________________________
enum class mfLanguageKind {
  kMusicFormatsLanguage_UNKNOWN,

  kMusicFormatsLanguageEnglish, // MusicFormats default
  kMusicFormatsLanguageFrench,
  kMusicFormatsLanguageItalian,
  kMusicFormatsLanguageGerman,
  kMusicFormatsLanguageSpanish,
  kMusicFormatsLanguageDutch
};

std::string mfLanguageKindAsString (
  mfLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const mfLanguageKind& elt);

mfLanguageKind mfLanguageKindFromString (
  const std::string& theString);

extern std::map<std::string, mfLanguageKind>
  gGlobalMusicFormatsLanguageKindsMap;

std::string availableMusicFormatsLanguageKinds (size_t namesListMaxLength);

void initializeMusicFormatsLanguageKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMusicFormatsLanguages ();

//________________________________________________________________________
class EXP language : public smartable
{
/* this class is purely virtual JMI v0.9.67 misses '= 0'
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<language> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          language ();

    virtual               ~language ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // default versions are in english, sub-classes override them for other languages

    // pass names

    virtual std::string   passIDKindAsString (mfPassIDKind passIDKind) const;

    virtual std::string   passOptional () const;

    virtual std::string   fullVersion () const;
    virtual std::string   summary () const;
    virtual std::string   names () const;
    virtual std::string   slices () const;

    // quitting after passes

    virtual std::string   quittingAfterPass (mfPassIDKind passIDKind) const;

    // OAH

    virtual std::string   handleOptionsAndArgumentsFromArgcArgv () const;

    // passes

    virtual std::string   createAnMXSRFromAMusicXMLFileOrStdin () const;
    virtual std::string   createAnMXSRFromAMusicXMLDescriptor () const;
    virtual std::string   createAnMXSRFromAMusicXMLBuffer () const;

    virtual std::string   createAnMSRSqueletonFromTheMXSR () const;

    virtual std::string   populateTheMSRSqueletonFromMusicXMLData () const;

    virtual std::string   convertTheFirstMSRIntoASecondMSR () const;

    virtual std::string   convertTheSecondMSRIntoAnLPSR () const;

    virtual std::string   convertTheLPSRIntoLilyPondCode () const;

    // display

    virtual std::string   displayTheFirstMSRSkeletonAsText () const;
    virtual std::string   displayTheFirstMSRAsText () const;
    virtual std::string   displayTheSecondMSRAsText () const;
    virtual std::string   displayTheLPSRAsText () const;

    virtual std::string   displayTheNamesInTheFirstMSR () const;
    virtual std::string   displayTheNamesInTheSecondMSR () const;

    virtual std::string   displayASummaryOfTheFirstMSR () const;
    virtual std::string   displayASummaryOfTheSecondMSR () const;

    virtual std::string   displayTheBSRAsText () const;
    virtual std::string   displayTheFirstBSRAsText () const;
    virtual std::string   displayTheFinalizedBSRAsText () const;

    // timing

    virtual std::string   timingInformation () const;
    virtual std::string   activity () const;
    virtual std::string   description () const;
    virtual std::string   kind () const;
    virtual std::string   CPUSeconds () const;
    virtual std::string   mandatory () const;
    virtual std::string   optional () const;
    virtual std::string   totalSeconds () const;

    // options files

    virtual std::string   openingOptionsFileForReading (
                            const std::string& optionsFileName) const;
    virtual std::string   openingIncludeFileForReading (
                            const std::string& includeFileName) const;

    virtual std::string   cannotOpenOptionsFileForReading (
                            const std::string& optionsFileName) const;

    virtual std::string   closingOptionsFile (
                            const std::string& optionsFileName) const;

    // output files

    virtual std::string   openingLilypondFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   openingBrailleMusicFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   openingMusicXMLFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   openingGuidoFileForWriting (
                            const std::string& inputFileName) const;

    virtual std::string   cannotOpenLilypondFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   cannotOpenBrailleMusicFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   cannotOpenMusicXMLFileForWriting (
                            const std::string& inputFileName) const;
    virtual std::string   cannotOpenGuidoFileForWriting (
                            const std::string& inputFileName) const;

    virtual std::string   closingLilypondFile (
                            const std::string& outputFileName) const;
    virtual std::string   closingBrailleMusicFile (
                            const std::string& outputFileName) const;
    virtual std::string   closingMusicXMLFile (
                            const std::string& outputFileName) const;
    virtual std::string   closingGuidoFile (
                            const std::string& outputFileName) const;

    // include files

    virtual std::string   cannotOpenIncludeFileForReading (
                            const std::string& includeFileName) const;

    // scripts

    virtual std::string   cannotOpenScriptForWriting (
                            const std::string& scriptName) const;

    // MSDL

    virtual std::string   cannotOpenMSDLFileForReading (
                            const std::string& inputFileName) const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<language> S_language;
EXP std::ostream& operator << (std::ostream& os, const S_language& elt);

//________________________________________________________________________
// hidden global language variable
EXP void setGlobalLanguage (mfLanguageKind languageKind);

EXP S_language getGlobalLanguage ();

// a handy shortcut
#define gLanguage getGlobalLanguage ()

//________________________________________________________________________
void initializeLanguages (); // JMI superflous? v0.9.67


}


#endif
