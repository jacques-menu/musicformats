/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mnx2mnxsrInterpreterInsiderHandler___
#define ___mnx2mnxsrInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "mnx2mnxsrInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP mnx2mnxsrInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~mnx2mnxsrInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMnx2mnxsrInterpreterPrefixes ();

    void                  createTheMnx2mnxsrInterpreterOptionGroups (
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

    std::string           mnx2mnxsrInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<mnx2mnxsrInterpreterInsiderHandler> S_mnx2mnxsrInterpreterInsiderHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<mnx2mnxsrInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMnx2mnxsrInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrInterpreterInsiderOahGroup ();

    virtual               ~mnx2mnxsrInterpreterInsiderOahGroup ();

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

    void                  printMnx2mnxsrInterpreterInsiderOahGroupHelp ();

    void                  printMnx2mnxsrInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mnx2mnxsrInterpreterInsiderOahGroup> S_mnx2mnxsrInterpreterInsiderOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInterpreterInsiderOahGroup& elt);

EXP extern S_mnx2mnxsrInterpreterInsiderOahGroup gGlobalMnx2mnxsrInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_mnx2mnxsrInterpreterInsiderOahGroup createGlobalMnx2mnxsrInterpreterInsiderOahGroup ();


}


#endif // ___mnx2mnxsrInterpreterInsiderHandler___
