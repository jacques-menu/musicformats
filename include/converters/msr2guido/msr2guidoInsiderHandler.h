/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2guidoInsiderHandler___
#define ___msr2guidoInsiderHandler___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP msr2guidoInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2guidoInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2guidoInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~msr2guidoInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsr2guidoPrefixes ();

    void                  createTheMsr2guidoOptionGroups (
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
typedef SMARTP<msr2guidoInsiderHandler> S_msr2guidoInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msr2guidoInsiderHandler& elt);

//______________________________________________________________________________
class EXP msr2guidoInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msr2guidoInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2guidoInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2guidoInsiderOahGroup ();

    virtual               ~msr2guidoInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // Guido
    void                  setGenerateComments ()
                              { fGenerateComments = true; }
    Bool                  getGenerateComments () const
                              { return fGenerateComments; }

    void                  setGenerateStem ()
                              { fGenerateStem = true; }
    Bool                  getGenerateStem () const
                              { return fGenerateStem; }

    void                  setGenerateBars ()
                              { fGenerateBars = true; }
    Bool                  getGenerateBars () const
                              { return fGenerateBars; }

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

    void                  printMsr2guidoInsiderOahGroupHelp ();

    void                  printMsr2guidoInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // Guido
    Bool                  fGenerateComments;
    Bool                  fGenerateStem;
    Bool                  fGenerateBars;

    // quit after some passes
    Bool                  fQuitAfterPass2;
    Bool                  fQuitAfterPass3;
    Bool                  fQuitAfterPass4;
    Bool                  fQuitAfterPass5;
};
typedef SMARTP<msr2guidoInsiderOahGroup> S_msr2guidoInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2guidoInsiderOahGroup& elt);

EXP extern S_msr2guidoInsiderOahGroup gGlobalMsr2guidoInsiderOahGroup;

//______________________________________________________________________________
S_msr2guidoInsiderOahGroup createGlobalMsr2guidoOahGroup ();


}


#endif // ___msr2guidoInsiderHandler___
