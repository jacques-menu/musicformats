/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesFrench___
#define ___languagesFrench___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageFrench : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageFrench> create ();

  public:

    // const overrideructors/destructor
    // ------------------------------------------------------

                          languageFrench ();

    virtual               ~languageFrench ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass names

    std::string           passIDKindAsString (mfPassIDKind passIDKind) const override;

    std::string           passOptional () const override;

    std::string           fullVersion () const override;
    std::string           summary () const override;
    std::string           names () const override;
    std::string           slices () const override;

    // quitting after passes

    std::string           quittingAfterPass (mfPassIDKind passIDKind) const override;

    // OAH

    std::string           handleOptionsAndArgumentsFromArgcArgv () const override;

    // passes

    std::string           convertAMusicXMLStreamIntoAnMXSR () const override;
    std::string           convertAMusicXMLDescriptorIntoAnMXSR () const override;
    std::string           convertAMusicXMLBufferIntoAnMXSR () const override;

    std::string           convertTheMXSRIntoAnMSRSkeleton () const override;

    std::string           populateTheMSRSkeletonFromMusicXMLData () const override;

    std::string           convertTheFirstMSRIntoASecondMSR () const override;

    std::string           convertTheSecondMSRIntoAnLPSR () const override;
    std::string           convertTheFirstMSRIntoAnLPSR () const override;

    std::string           convertTheLPSRIntoLilyPondCode () const override;

    // display

    std::string           displayTheFirstMSRSkeletonAsText () const override;
    std::string           displayTheFirstMSRAsText () const override;
    std::string           displayTheSecondMSRAsText () const override;
    std::string           displayTheMSREmbededInTheLPSRAsText () const override;
    std::string           displayTheLPSRAsText () const override;

    std::string           displayTheNamesInTheFirstMSR () const override;
    std::string           displayTheNamesInTheSecondMSR () const override;
    std::string           displayTheNamesInTheLPSR () const override;

    std::string           displayASummaryOfTheFirstMSR () const override;
    std::string           displayASummaryOfTheSecondMSR () const override;
    std::string           displayASummaryOfTheLPSR () const override;

    std::string           displayAFlatViewOfTheFirstMSR () const override;
    std::string           displayAFlatViewOfTheSecondMSR () const override;
    std::string           displayAFlatViewOfTheLPSR () const override;

    std::string           displayTheSlicesOfTheFirstMSR () const override;
    std::string           displayTheSlicesOfTheSecondMSR () const override;
    std::string           displayTheSlicesOfTheLPSR () const override;

   // timing

    std::string           timingInformation () const override;
    std::string           activity () const override;
    std::string           description () const override;
    std::string           kind () const override;
    std::string           CPUSeconds () const override;
    std::string           mandatory () const override;
    std::string           optional () const override;
    std::string           totalSeconds () const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<languageFrench> S_languageFrench;
EXP std::ostream& operator << (std::ostream& os, const S_languageFrench& elt);


}


#endif // ___languagesFrench___
