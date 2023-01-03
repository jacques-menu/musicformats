/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2brailleInsiderHandler___
#define ___msr2brailleInsiderHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msr2brailleInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2brailleInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2brailleInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~msr2brailleInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2braillePrefixes ();

    void                  createTheXml2brailleOptionGroups (
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
typedef SMARTP<msr2brailleInsiderHandler> S_msr2brailleInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msr2brailleInsiderHandler& elt);

//______________________________________________________________________________
class EXP msr2brailleInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2brailleInsiderOahGroup> create (
                            const std::string&          serviceName,
                            const std::string&          handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brailleInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2brailleInsiderOahGroup (
                            const std::string&          serviceName,
                            const std::string&          handlerHeader);

    virtual               ~msr2brailleInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    void                  setQuit2a ()
                              { fQuitAfterPass2a = true; }
    Bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }

    void                  setQuit2b ()
                              { fQuitAfterPass2b = true; }
    Bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2a; }

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

    void                  printXml2brailleInsiderOahGroupHelp ();

    void                  printXml2brailleInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // --------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<msr2brailleInsiderOahGroup> S_msr2brailleInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2brailleInsiderOahGroup& elt);

EXP extern S_msr2brailleInsiderOahGroup gGlobalMsr2brailleInsiderOahGroup;

//______________________________________________________________________________
S_msr2brailleInsiderOahGroup createGlobalMrailleGenerationOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader);


}


#endif
