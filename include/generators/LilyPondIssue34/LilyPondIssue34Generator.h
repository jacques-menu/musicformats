/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___LilyPondIssue34Generator___
#define ___LilyPondIssue34Generator___

#include "smartpointer.h"

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
                            const S_msrPart& part);

    S_msrStaff            createAndPopulateLowerStaffInPart (
                            const S_msrPart&            part,
                            msrGenerationAPIKind generationAPIKind);

    S_msrStaff            createAndPopulateUpperStaffInPart (
                            const S_msrPart&            part,
                            msrGenerationAPIKind generationAPIKind);

    // the part
    S_msrPart             createPartInScore (
                            const S_msrScore& theMsrScore);

    // the voices
    S_msrVoice            createRegularVoiceInStaff (
                            int          inputLineNumber,
                            int          voiceNumber,
                            const S_msrStaff&   staff);

    // the measures
    S_msrMeasure          createMeasureInVoice (
                            int           inputLineNumber,
                            const std::string& measureNumber,
                            const S_msrVoice&    voice);

    // populating the upper voice
    void                  populateUpperVoice1WithTheFunctionsAPI (
                            const S_msrVoice& upperVoice1);

    void                  populateUpperVoice1WithTheStringsAPI (
                            const S_msrVoice& upperVoice1);

    // populating the lower voices
    void                  populateLowerVoice1WithTheFunctionsAPI (
                            const S_msrVoice& lowerVoice1);

    void                  populateLowerVoice1WithTheStringsAPI (
                            const S_msrVoice& lowerVoice1);

    void                  populateLowerVoice2WithTheFunctionsAPI (
                            const S_msrVoice& lowerVoice2);

    void                  populateLowerVoice2WithTheStringsAPI (
                            const S_msrVoice& lowerVoice2);
};
typedef SMARTP<LilyPondIssue34Generator> S_LilyPondIssue34Generator;
// JMI EXP std::ostream& operator << (std::ostream& os, const S_LilyPondIssue34Generator& elt);


}


#endif // ___LilyPondIssue34Generator___
