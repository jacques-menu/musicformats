/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2brailleInsiderHandler___
#define ___msdl2brailleInsiderHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msdl2brailleInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brailleInsiderHandler> create (
                            const string& serviceName,
                            const string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brailleInsiderHandler (
                            const string& serviceName,
                            const string& handlerHeader);

    virtual               ~msdl2brailleInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2brlPrefixes ();

    void                  createTheMsdl2brlOptionGroups (
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
typedef SMARTP<msdl2brailleInsiderHandler> S_msdl2brailleInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_msdl2brailleInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2brailleInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2brailleInsiderOahGroup> create (
                            const string&     serviceName,
                            const string&     handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2brlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brailleInsiderOahGroup (
                            const string&     serviceName,
                            const string&     handlerHeader);

    virtual               ~msdl2brailleInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // quit after some passes
    void                  setQuitAfterPass1 ()
                              { fQuitAfterPass1 = true; }
    Bool                  getQuitAfterPass1 () const
                              { return fQuitAfterPass1; }

    void                  setQuitAfterPass2 ()
                              { fQuitAfterPass2 = true; }
    Bool                  getQuitAfterPass2 () const
                              { return fQuitAfterPass2; }

    void                  setQuit3 ()
                              { fQuitAfterPass3 = true; }
    Bool                  getQuitAfterPass3 () const
                              { return fQuitAfterPass3; }

    void                  setQuitAfterPass4 ()
                              { fQuitAfterPass4 = true; }
    Bool                  getQuitAfterPass4 () const
                              { return fQuitAfterPass4; }

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

    void                  printMsdl2brlInsiderOahGroupHelp ();

    void                  printMsdl2brlInsiderOahGroupValues (
                            int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // --------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass1;
    Bool                  fQuitAfterPass2;
    Bool                  fQuitAfterPass3;
    Bool                  fQuitAfterPass4;
};
typedef SMARTP<msdl2brailleInsiderOahGroup> S_msdl2brailleInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2brailleInsiderOahGroup& elt);

EXP extern S_msdl2brailleInsiderOahGroup gGlobalMsdl2brlInsiderOahGroup;

//______________________________________________________________________________
S_msdl2brailleInsiderOahGroup createGlobalMsdl2brlOahGroup (
  const string& serviceName,
  const string& handlerHeader);


}


#endif
