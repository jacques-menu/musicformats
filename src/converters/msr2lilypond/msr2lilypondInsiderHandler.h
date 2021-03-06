/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lilypondInsiderHandler___
#define ___msr2lilypondInsiderHandler___

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"

#include "oahRegularHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msr2lilypondInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondInsiderHandler> create (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondInsiderHandler (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

    virtual               ~msr2lilypondInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsr2lilypondPrefixes ();

    void                  createTheMsr2lilypondOptionGroups (
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
typedef SMARTP<msr2lilypondInsiderHandler> S_msr2lilypondInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondInsiderHandler& elt);

//______________________________________________________________________________
class EXP msr2lilypondInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2lilypondInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lilypondInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondInsiderOahGroup ();

    virtual               ~msr2lilypondInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
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

    void                  printMsr2lilypondInsiderOahGroupHelp ();

    void                  printMsr2lilypondInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<msr2lilypondInsiderOahGroup> S_msr2lilypondInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondInsiderOahGroup& elt);

EXP extern S_msr2lilypondInsiderOahGroup gGlobalMsr2lilypondInsiderOahGroup;

//______________________________________________________________________________
S_msr2lilypondInsiderOahGroup createGlobalMsr2lilypondInsiderOahGroup ();


}


#endif
