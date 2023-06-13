/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFindInterpreterInsiderHandler___
#define ___mfFindInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mfFindInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP mfFindInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~mfFindInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createThemfFindInterpreterPrefixes ();

    void                  createThemfFindInterpreterOptionGroups (
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

    std::string           mfFindInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<mfFindInterpreterInsiderHandler> S_mfFindInterpreterInsiderHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP mfFindInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<mfFindInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializemfFindInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindInterpreterInsiderOahGroup ();

    virtual               ~mfFindInterpreterInsiderOahGroup ();

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

    void                  printmfFindInterpreterInsiderOahGroupHelp ();

    void                  printmfFindInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfFindInterpreterInsiderOahGroup> S_mfFindInterpreterInsiderOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindInterpreterInsiderOahGroup& elt);

EXP extern S_mfFindInterpreterInsiderOahGroup gGlobalmfFindInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_mfFindInterpreterInsiderOahGroup createGlobalmfFindInterpreterInsiderOahGroup ();


}


#endif // ___mfFindInterpreterInsiderHandler___
