/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2musicxmlInsiderOahGroup___
#define ___msr2musicxmlInsiderOahGroup___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msr2musicxmlInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2musicxmlInsiderHandler> create (
                            const string& serviceName,
                            const string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2musicxmlInsiderHandler (
                            const string& serviceName,
                            const string& handlerHeader);

    virtual               ~msr2musicxmlInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsr2musicxmlPrefixes ();

    void                  createTheMsr2musicxmlOptionGroups (
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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msr2musicxmlInsiderHandler> S_msr2musicxmlInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msr2musicxmlInsiderHandler& elt);

//______________________________________________________________________________
class EXP msr2musicxmlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2musicxmlInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2musicxmlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2musicxmlInsiderOahGroup ();

    virtual               ~msr2musicxmlInsiderOahGroup ();

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

    void                  printMsr2musicxmlInsiderOahGroupHelp ();

    void                  printMsr2musicxmlInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<msr2musicxmlInsiderOahGroup> S_msr2musicxmlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2musicxmlInsiderOahGroup& elt);

EXP extern S_msr2musicxmlInsiderOahGroup gGlobalMsr2musicxmlInsiderOahGroup;

//______________________________________________________________________________
S_msr2musicxmlInsiderOahGroup createGlobalMsr2musicxmlOahGroup ();


}


#endif
