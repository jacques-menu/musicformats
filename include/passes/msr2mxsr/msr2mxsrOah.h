/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2mxsrOah___
#define ___msr2mxsrOah___

#include "mfPreprocessorSettings.h"

#include "oahBasicTypes.h"

#include "lpsrEnumTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2mxsr2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2mxsr2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2mxsr2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2mxsr2msrOahGroup ();

    virtual               ~msr2mxsr2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // code generation
    Bool                  getXml2xmlInfos () const
                              { return fXml2xmlInfos; }

    Bool                  getMusicXMLComments () const
                              { return fMusicXMLComments; }

    // work and movement
    std::string           getWorkCreditTypeTitle () const
                              { return fWorkCreditTypeTitle; }
    std::string           getWorkNumber () const
                              { return fWorkNumber; }

    std::string           getMovementTitle () const
                              { return fMovementTitle; }
    std::string           getMovementNumber () const
                              { return fMovementNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    void                  initializMsr2mxsrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializeCodeGenerationOptions ();
    void                  initializeMxsrGenerationOahGroup ();

    void                  initializeWorkAndMovementOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2mxsrOahHelp ();

    void                  displayMsr2mxsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // code generation
    Bool                  fXml2xmlInfos;

    Bool                  fMusicXMLComments;

    // work and movement
    std::string           fWorkCreditTypeTitle;
    std::string           fWorkNumber;

    std::string           fMovementTitle;
    std::string           fMovementNumber;
};
typedef SMARTP<msr2mxsr2msrOahGroup> S_msr2mxsr2msrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2mxsr2msrOahGroup& elt);

EXP extern S_msr2mxsr2msrOahGroup gGlobalMsr2mxsr2msrOahGroup;

//______________________________________________________________________________
EXP S_msr2mxsr2msrOahGroup createGlobalMsr2mxsr2msrOahGroup ();


}


#endif // ___msr2mxsrOah___
