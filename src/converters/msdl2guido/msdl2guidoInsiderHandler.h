/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2guidoInsiderHandler___
#define ___msdl2guidoInsiderHandler___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP msdl2guidoInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2guidoInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2guidoInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~msdl2guidoInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2gmnPrefixes ();

    void                  createTheMsdl2gmnOptionGroups (
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
typedef SMARTP<msdl2guidoInsiderHandler> S_msdl2guidoInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2guidoInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2guidoInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2guidoInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2gmnInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2guidoInsiderOahGroup ();

    virtual               ~msdl2guidoInsiderOahGroup ();

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

    void                  printMsdl2gmnInsiderOahGroupHelp ();

    void                  printMsdl2gmnInsiderOahGroupValues (
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
typedef SMARTP<msdl2guidoInsiderOahGroup> S_msdl2guidoInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2guidoInsiderOahGroup& elt);

EXP extern S_msdl2guidoInsiderOahGroup gGlobalMsdl2gmnInsiderOahGroup;

//______________________________________________________________________________
S_msdl2guidoInsiderOahGroup createGlobalMsdl2gmnInsiderOahGroup ();


}


#endif // ___msdl2guidoInsiderHandler___
