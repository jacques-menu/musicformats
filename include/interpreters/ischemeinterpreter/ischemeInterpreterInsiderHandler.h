/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeInterpreterInsiderHandler___
#define ___ischemeInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "ischemeInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP ischemeInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~ischemeInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheIschemeInterpreterPrefixes ();

    void                  createTheIschemeInterpreterOptionGroups (
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

    std::string           ischemeInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<ischemeInterpreterInsiderHandler> S_ischemeInterpreterInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP ischemeInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<ischemeInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeIschemeInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInterpreterInsiderOahGroup ();

    virtual               ~ischemeInterpreterInsiderOahGroup ();

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

    void                  printIschemeInterpreterInsiderOahGroupHelp ();

    void                  printIschemeInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<ischemeInterpreterInsiderOahGroup> S_ischemeInterpreterInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeInterpreterInsiderOahGroup& elt);

EXP extern S_ischemeInterpreterInsiderOahGroup gGlobalIschemeInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_ischemeInterpreterInsiderOahGroup createGlobalIschemeInterpreterInsiderOahGroup ();


}


#endif
