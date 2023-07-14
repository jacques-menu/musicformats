/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2musicxmlInsiderHandler___
#define ___msr2musicxmlInsiderHandler___

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
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2musicxmlInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

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
                            const std::string& serviceName);

  public:

    // public services
    // ------------------------------------------------------

    std::string           handlerServiceAboutInformation () const override;

    void                  checkOptionsAndArguments () const override;

    std::string           fetchOutputFileNameFromTheOptions () const override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<msr2musicxmlInsiderHandler> S_msr2musicxmlInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msr2musicxmlInsiderHandler& elt);

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
    void                  setQuitAfterPass2 ()
                              { fQuitAfterPass2 = true; }
    Bool                  getQuitAfterPass2 () const
                              { return fQuitAfterPass2; }

    void                  setQuitAfterPass3 ()
                              { fQuitAfterPass3 = true; }
    Bool                  getQuitAfterPass3 () const
                              { return fQuitAfterPass3; }

    void                  setQuitAfterPasss4 ()
                              { fQuitAfterPass4 = true; }
    Bool                  getQuitAfterPass4 () const
                              { return fQuitAfterPass4; }

    void                  setQuitAfterPasss5 ()
                              { fQuitAfterPass5 = true; }
    Bool                  getQuitAfterPass5 () const
                              { return fQuitAfterPass5; }

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
    Bool                  fQuitAfterPass2;
    Bool                  fQuitAfterPass3;
    Bool                  fQuitAfterPass4;
    Bool                  fQuitAfterPass5;
};
typedef SMARTP<msr2musicxmlInsiderOahGroup> S_msr2musicxmlInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2musicxmlInsiderOahGroup& elt);

EXP extern S_msr2musicxmlInsiderOahGroup gGlobalMsr2musicxmlInsiderOahGroup;

//______________________________________________________________________________
S_msr2musicxmlInsiderOahGroup createGlobalMsr2musicxmlOahGroup ();


}


#endif // ___msr2musicxmlInsiderHandler___
