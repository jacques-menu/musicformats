/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___Mikrokosmos3WanderingInsiderHandler___
#define ___Mikrokosmos3WanderingInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfMultiGenerationOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP Mikrokosmos3WanderingInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            mfMultiGenerationOutputKind
                                                    multiGenerationOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            mfMultiGenerationOutputKind
                                                    multiGenerationOutputKind);

    virtual               ~Mikrokosmos3WanderingInsiderHandler ();

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

    void                  createTheMikrokosmos3WanderingPrefixes ();

    void                  createTheMikrokosmos3WanderingOptionGroups (
                            const std::string& serviceName,
                            mfMultiGenerationOutputKind
                                          multiGenerationOutputKind);

  public:

    // public services
    // ------------------------------------------------------

    std::string           handlerServiceAboutInformation () const override;

    void                  checkOptionsAndArguments () const override;

    std::string           fetchOutputFileNameFromTheOptions () const override;

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

    void                  print (std::ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    std::string           usageInformation (
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

    std::string           Mikrokosmos3WanderingAboutInformation (
                            mfMultiGenerationOutputKind multiGenerationOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;

};
typedef SMARTP<Mikrokosmos3WanderingInsiderHandler> S_Mikrokosmos3WanderingInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_Mikrokosmos3WanderingInsiderHandler& elt);

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<Mikrokosmos3WanderingInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMikrokosmos3WanderingInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingInsiderOahGroup ();

    virtual               ~Mikrokosmos3WanderingInsiderOahGroup ();

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

    void                  printMikrokosmos3WanderingInsiderOahGroupHelp ();

    void                  printMikrokosmos3WanderingInsiderOahGroupValues (
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
typedef SMARTP<Mikrokosmos3WanderingInsiderOahGroup> S_Mikrokosmos3WanderingInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_Mikrokosmos3WanderingInsiderOahGroup& elt);

EXP extern S_Mikrokosmos3WanderingInsiderOahGroup gGlobalMikrokosmos3WanderingInsiderOahGroup;

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup createGlobalMikrokosmos3WanderingInsiderOahGroup ();


}


#endif // ___Mikrokosmos3WanderingInsiderHandler___
