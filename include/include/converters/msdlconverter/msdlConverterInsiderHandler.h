/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlConverterInsiderHandler___
#define ___msdlConverterInsiderHandler___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfMultiGenerationOah.h"

#include "msdlConverterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP msdlConverterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlConverterInsiderHandler> create (
                            const string&               serviceName,
                            const string&               handlerHeader,
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlConverterInsiderHandler (
                            const string&               serviceName,
                            const string&               handlerHeader,
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

    virtual               ~msdlConverterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdlConverterPrefixes ();

    void                  createTheMsdlConverterOptionGroups (
                            const string&              serviceName,
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

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

    void                  print (ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                usageInformation (
                            mfMultiGenerationOutputKind
                              multiGenerationOutputKind);

    string                msdlConverterAboutInformation (
                            mfMultiGenerationOutputKind
                              multiGenerationOutputKind) const;

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    mfMultiGenerationOutputKind   fMultiGenerationOutputKind;

};
typedef SMARTP<msdlConverterInsiderHandler> S_msdlConverterInsiderHandler;
EXP ostream& operator << (ostream& os, const S_msdlConverterInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdlConverterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdlConverterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializemsdlConverterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlConverterInsiderOahGroup ();

    virtual               ~msdlConverterInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated output kind
    void                  setMultiGenerationOutputKind (
                            mfMultiGenerationOutputKind value)
                              { fMultiGenerationOutputKind = value; }
    mfMultiGenerationOutputKind
                          getMultiGenerationOutputKind () const
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

    void                  printmsdlConverterInsiderOahGroupHelp ();

    void                  printmsdlConverterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;
};
typedef SMARTP<msdlConverterInsiderOahGroup> S_msdlConverterInsiderOahGroup;
EXP ostream& operator << (ostream& os, const S_msdlConverterInsiderOahGroup& elt);

EXP extern S_msdlConverterInsiderOahGroup gGlobalmsdlConverterInsiderOahGroup;

//______________________________________________________________________________
S_msdlConverterInsiderOahGroup createGlobalmsdlConverterInsiderOahGroup ();


}


#endif
