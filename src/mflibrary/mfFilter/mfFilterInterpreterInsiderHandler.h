/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFilterInterpreterInsiderHandler___
#define ___mfFilterInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfFilterInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP mfFilterInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFilterInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~mfFilterInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createThemfFilterInterpreterPrefixes ();

    void                  createThemfFilterInterpreterOptionGroups (
                            const std::string& serviceName);

  public:

    // public services
    // ------------------------------------------------------

    std::string           handlerServiceAboutInformation () const override;

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

    void                  print (std::ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    std::string           usageInformation ();

    std::string           mfFilterInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<mfFilterInterpreterInsiderHandler> S_mfFilterInterpreterInsiderHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP mfFilterInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<mfFilterInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializemfFilterInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterInterpreterInsiderOahGroup ();

    virtual               ~mfFilterInterpreterInsiderOahGroup ();

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

    void                  printmfFilterInterpreterInsiderOahGroupHelp ();

    void                  printmfFilterInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfFilterInterpreterInsiderOahGroup> S_mfFilterInterpreterInsiderOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterInterpreterInsiderOahGroup& elt);

EXP extern S_mfFilterInterpreterInsiderOahGroup gGlobalmfFilterInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_mfFilterInterpreterInsiderOahGroup createGlobalmfFilterInterpreterInsiderOahGroup ();


}


#endif // ___mfFilterInterpreterInsiderHandler___
