/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___LilyPondIssue34Generator___
#define ___LilyPondIssue34Generator___

#include "smartpointer.h"

// #include "msr.h"

#include "mfMultiGenerationOah.h"

#include "msrGenerationBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP LilyPondIssue34Generator : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<LilyPondIssue34Generator> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          LilyPondIssue34Generator ();

    virtual               ~LilyPondIssue34Generator ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilyPondIssue34Generator ();

  public:

    // public services
    // ------------------------------------------------------

    S_msrScore            createAndPopulateTheScore (
                            msrGenerationAPIKind generationAPIKind);

  private:

    // private services
    // ------------------------------------------------------

    // the score
    S_msrScore            createTheScore (
                            msrGenerationAPIKind generationAPIKind);

    // the staves
    S_msrStaff            createStaffInPart (
                            int staffNumber,
                            S_msrPart part);

    S_msrStaff            createAndPopulateLowerStaffInPart (
                            S_msrPart            part,
                            msrGenerationAPIKind generationAPIKind);

    S_msrStaff            createAndPopulateUpperStaffInPart (
                            S_msrPart            part,
                            msrGenerationAPIKind generationAPIKind);

    // the part
    S_msrPart             createPartInScore (S_msrScore theMsrScore);

    // the voices
    S_msrVoice            createRegularVoiceInStaff (
                            int          inputLineNumber,
                            int          voiceNumber,
                            S_msrStaff   staff);

    // the measures
    S_msrMeasure          createMeasureInVoice (
                            int           inputLineNumber,
                            const string& measureNumber,
                            S_msrVoice    voice);

    // populating the upper voice
    void                  populateUpperVoice1WithTheFunctionsAPI (
                            S_msrVoice upperVoice1);

    void                  populateUpperVoice1WithTheStringsAPI (
                            S_msrVoice upperVoice1);

    // populating the lower voices
    void                  populateLowerVoice1WithTheFunctionsAPI (
                            S_msrVoice lowerVoice1);

    void                  populateLowerVoice1WithTheStringsAPI (
                            S_msrVoice lowerVoice1);

    void                  populateLowerVoice2WithTheFunctionsAPI (
                            S_msrVoice lowerVoice2);

    void                  populateLowerVoice2WithTheStringsAPI (
                            S_msrVoice lowerVoice2);
};
typedef SMARTP<LilyPondIssue34Generator> S_LilyPondIssue34Generator;
// JMI EXP ostream& operator << (ostream& os, const S_LilyPondIssue34Generator& elt);


}


#endif
