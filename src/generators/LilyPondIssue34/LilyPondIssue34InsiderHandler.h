/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___LilyPondIssue34InsiderHandler___
#define ___LilyPondIssue34InsiderHandler___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfMultiGenerationOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP LilyPondIssue34InsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<LilyPondIssue34InsiderHandler> create (
                            const string& serviceName,
                            const string& handlerHeader,
                            mfMultiGenerationOutputKind
                                                    multiGenerationOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          LilyPondIssue34InsiderHandler (
                            const string& serviceName,
                            const string& handlerHeader,
                            mfMultiGenerationOutputKind
                                                    multiGenerationOutputKind);

    virtual               ~LilyPondIssue34InsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheLilyPondIssue34Prefixes ();

    void                  createTheLilyPondIssue34OptionGroups (
                            const string&             serviceName,
                            mfMultiGenerationOutputKind mfMultiGenerationOutputKind);

  public:

    // public services
    // ------------------------------------------------------

    string                handlerServiceAboutInformation () const override;

    void                  checkOptionsAndArguments () const override;

    string                fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    void                  checkHandlerOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                usageInformation (
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

    string                LilyPondIssue34AboutInformation (
                            mfMultiGenerationOutputKind multiGenerationOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;

};
typedef SMARTP<LilyPondIssue34InsiderHandler> S_LilyPondIssue34InsiderHandler;
EXP ostream& operator<< (ostream& os, const S_LilyPondIssue34InsiderHandler& elt);

//______________________________________________________________________________
class EXP LilyPondIssue34InsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<LilyPondIssue34InsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilyPondIssue34InsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          LilyPondIssue34InsiderOahGroup ();

    virtual               ~LilyPondIssue34InsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generation API kind
    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }
    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind; }

    // generated output kind
    void                  setMultiGenerationOutputKind (mfMultiGenerationOutputKind value)
                              { fMultiGenerationOutputKind = value; }
    mfMultiGenerationOutputKind   getMultiGenerationOutputKind () const
                              { return fMultiGenerationOutputKind; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLilyPondIssue34InsiderOahGroupHelp ();

    void                  printLilyPondIssue34InsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeGenerateCodeOptions ();

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind
    msrGenerationAPIKind  fGenerationAPIKind;

    // generator output kind
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;
    S_mfMultiGenerationOutputKindAtom
                          fMultiGenerationOutputKindAtom;
};
typedef SMARTP<LilyPondIssue34InsiderOahGroup> S_LilyPondIssue34InsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_LilyPondIssue34InsiderOahGroup& elt);

EXP extern S_LilyPondIssue34InsiderOahGroup gGlobalLilyPondIssue34InsiderOahGroup;

//______________________________________________________________________________
S_LilyPondIssue34InsiderOahGroup createGlobalLilyPondIssue34InsiderOahGroup ();


}


#endif
