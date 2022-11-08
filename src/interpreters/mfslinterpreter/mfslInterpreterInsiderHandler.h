/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslInterpreterInsiderHandler___
#define ___mfslInterpreterInsiderHandler___

// #include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfslInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP mfslInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInterpreterInsiderHandler> create (
                            const string& serviceName,
                            const string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInterpreterInsiderHandler (
                            const string& serviceName,
                            const string& handlerHeader);

    virtual               ~mfslInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMfslInterpreterPrefixes ();

    void                  createTheMfslInterpreterOptionGroups (
                            const string& serviceName);

  public:

    // public services
    // ------------------------------------------------------

    string                handlerServiceAboutInformation () const override;

    void                  checkOptionsAndArguments () const override;

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

    string                usageInformation ();

    string                mfslInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<mfslInterpreterInsiderHandler> S_mfslInterpreterInsiderHandler;
EXP ostream& operator << (ostream& os, const S_mfslInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP mfslInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<mfslInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMfslInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInterpreterInsiderOahGroup ();

    virtual               ~mfslInterpreterInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

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

    void                  printMfslInterpreterInsiderOahGroupHelp ();

    void                  printMfslInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfslInterpreterInsiderOahGroup> S_mfslInterpreterInsiderOahGroup;
EXP ostream& operator << (ostream& os, const S_mfslInterpreterInsiderOahGroup& elt);

EXP extern S_mfslInterpreterInsiderOahGroup gGlobalMfslInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_mfslInterpreterInsiderOahGroup createGlobalMfslInterpreterInsiderOahGroup ();


}


#endif
