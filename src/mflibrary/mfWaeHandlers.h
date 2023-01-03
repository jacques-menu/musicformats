/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrParserWaeHandlers___
#define ___msdrParserWaeHandlers___

#include "msdlTokens.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandler : public waeHandler
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandler> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWaeHandler ();

    virtual               ~mfWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass numbers

    // default versions are in english, sub-classes override them for other languages
    virtual std::string   pass1 () const;

    virtual std::string   pass2 () const;
    virtual std::string   pass2a () const;
    virtual std::string   pass2b () const;

    virtual std::string   pass3 () const;
    virtual std::string   pass3a () const;
    virtual std::string   pass3b () const;

    virtual std::string   pass4 () const;
    virtual std::string   pass4a () const;
    virtual std::string   pass4b () const;

    virtual std::string   pass5 () const;

    virtual std::string   passOptional () const;

    virtual std::string   fullVersion () const;
    virtual std::string   summary () const;
    virtual std::string   names () const;
    virtual std::string   slices () const;

    // OAH

    virtual std::string   handleOptionsAndArgumentsFromArgcArgv () const;

    // passes

    virtual std::string   createAnMXSRFromAMusicXMLFile () const;
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

    // timing

    virtual std::string   timingInformation () const;
    virtual std::string   activity () const;
    virtual std::string   description () const;
    virtual std::string   kind () const;
    virtual std::string   CPUSeconds () const;
    virtual std::string   mandatory () const;
    virtual std::string   optional () const;
    virtual std::string   totalSeconds () const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfWaeHandler> S_mfWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandler& elt);


}


#endif
