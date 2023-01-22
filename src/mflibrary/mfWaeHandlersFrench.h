/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersFrench___
#define ___mfWaeHandlersFrench___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerFrench : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerFrench> create ();

  public:

    // const overrideructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerFrench ();

    virtual               ~mfWaeHandlerFrench ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass names

    virtual std::string   passIDKindAsString (mfPassIDKind passIDKind) const override;

//     std::string           passName (mfPassIDKind passIDKind) const override;

    std::string           passOptional () const override;

    std::string           fullVersion () const override;
    std::string           summary () const override;
    std::string           names () const override;
    std::string           slices () const override;

    // quitting after passes

    virtual std::string   quittingAfterPass (mfPassIDKind passIDKind) const override;

    // OAH

    std::string           handleOptionsAndArgumentsFromArgcArgv () const override;

    // passes

    std::string           createAnMXSRFromAMusicXMLFile () const override;
    std::string           createAnMXSRFromAMusicXMLDescriptor () const override;
    std::string           createAnMXSRFromAMusicXMLBuffer () const override;

    std::string           createAnMSRSqueletonFromTheMXSR () const override;

    std::string           populateTheMSRSqueletonFromMusicXMLData () const override;

    std::string           convertTheFirstMSRIntoASecondMSR () const override;

    std::string           convertTheSecondMSRIntoAnLPSR () const override;

    std::string           convertTheLPSRIntoLilyPondCode () const override;

    // display

    std::string           displayTheFirstMSRSkeletonAsText () const override;
    std::string           displayTheFirstMSRAsText () const override;
    std::string           displayTheSecondMSRAsText () const override;
    std::string           displayTheLPSRAsText () const override;

    std::string           displayTheNamesInTheFirstMSR () const override;
    std::string           displayTheNamesInTheSecondMSR () const override;

    std::string           displayASummaryOfTheFirstMSR () const override;
    std::string           displayASummaryOfTheSecondMSR () const override;

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
typedef SMARTP<mfWaeHandlerFrench> S_mfWaeHandlerFrench;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerFrench& elt);


}


#endif
