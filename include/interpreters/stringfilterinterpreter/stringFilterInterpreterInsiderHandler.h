/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringFilterInterpreterInsiderHandler___
#define ___stringFilterInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "stringFilterInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP stringFilterInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~stringFilterInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createThestringFilterInterpreterPrefixes ();

    void                  createThestringFilterInterpreterOptionGroups (
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

    std::string           stringFilterInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<stringFilterInterpreterInsiderHandler> S_stringFilterInterpreterInsiderHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP stringFilterInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<stringFilterInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializestringFilterInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterInterpreterInsiderOahGroup ();

    virtual               ~stringFilterInterpreterInsiderOahGroup ();

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

    void                  printstringFilterInterpreterInsiderOahGroupHelp ();

    void                  printstringFilterInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringFilterInterpreterInsiderOahGroup> S_stringFilterInterpreterInsiderOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterInsiderOahGroup& elt);

EXP extern S_stringFilterInterpreterInsiderOahGroup gGlobalstringFilterInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_stringFilterInterpreterInsiderOahGroup createGlobalstringFilterInterpreterInsiderOahGroup ();


}


#endif // ___stringFilterInterpreterInsiderHandler___
