/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherInterpreterInsiderHandler___
#define ___stringMatcherInterpreterInsiderHandler___

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "stringMatcherInterpreterOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP stringMatcherInterpreterInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherInterpreterInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherInterpreterInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~stringMatcherInterpreterInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createThestringMatcherInterpreterPrefixes ();

    void                  createThestringMatcherInterpreterOptionGroups (
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

    std::string           stringMatcherInterpreterAboutInformation () const;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<stringMatcherInterpreterInsiderHandler> S_stringMatcherInterpreterInsiderHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherInterpreterInsiderHandler& elt);

//______________________________________________________________________________
class EXP stringMatcherInterpreterInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<stringMatcherInterpreterInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializestringMatcherInterpreterInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherInterpreterInsiderOahGroup ();

    virtual               ~stringMatcherInterpreterInsiderOahGroup ();

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

    void                  printstringMatcherInterpreterInsiderOahGroupHelp ();

    void                  printstringMatcherInterpreterInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherInterpreterInsiderOahGroup> S_stringMatcherInterpreterInsiderOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherInterpreterInsiderOahGroup& elt);

EXP extern S_stringMatcherInterpreterInsiderOahGroup gGlobalstringMatcherInterpreterInsiderOahGroup;

//______________________________________________________________________________
S_stringMatcherInterpreterInsiderOahGroup createGlobalstringMatcherInterpreterInsiderOahGroup ();


}


#endif // ___stringMatcherInterpreterInsiderHandler___
