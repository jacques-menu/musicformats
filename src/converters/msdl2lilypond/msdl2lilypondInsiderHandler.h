/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2lilypondInsiderHandler___
#define ___msdl2lilypondInsiderHandler___

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msdl2lilypondInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lilypondInsiderHandler> create (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lilypondInsiderHandler (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

    virtual               ~msdl2lilypondInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2lyPrefixes ();

    void                  createTheMsdl2lyOptionGroups (
                            const string& serviceName);

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

  private:

    // private methods
    // ------------------------------------------------------

    string               usageFromUsedThruKind (
                           oahHandlerUsedThruKind handlerUsedThruKind) const;
  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msdl2lilypondInsiderHandler> S_msdl2lilypondInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2lilypondInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2lilypondInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2lilypondInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2lyInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lilypondInsiderOahGroup ();

    virtual               ~msdl2lilypondInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    void                  setQuitAfterPass1 ()
                              { fQuitAfterPass1 = true; }
    Bool                  getQuitAfterPass1 () const
                              { return fQuitAfterPass1; }

    void                  setQuitAfterPass2a ()
                              { fQuitAfterPass2a = true; }
    Bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }

    void                  setQuitAfterPass2b ()
                              { fQuitAfterPass2b = true; }
    Bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2b; }

    void                  setQuit3 ()
                              { fQuitAfterPass3 = true; }
    Bool                  getQuitAfterPass3 () const
                              { return fQuitAfterPass3; }

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

    void                  printMsdl2lyInsiderOahGroupHelp ();

    void                  printMsdl2lyInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass1;
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<msdl2lilypondInsiderOahGroup> S_msdl2lilypondInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2lilypondInsiderOahGroup& elt);

EXP extern S_msdl2lilypondInsiderOahGroup gGlobalMsdl2lyInsiderOahGroup;

//______________________________________________________________________________
S_msdl2lilypondInsiderOahGroup createGlobalMsdl2lyInsiderOahGroup ();


}


#endif
