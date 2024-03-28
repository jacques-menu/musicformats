/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2musicxmlInsiderHandler___
#define ___msdl2musicxmlInsiderHandler___

#include "oahBasicTypes.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP msdl2musicxmlInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2musicxmlInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2musicxmlInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~msdl2musicxmlInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheMsdl2xmlPrefixes ();

    void                  createTheMsdl2xmlOptionGroups (
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
typedef SMARTP<msdl2musicxmlInsiderHandler> S_msdl2musicxmlInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2musicxmlInsiderHandler& elt);

//______________________________________________________________________________
class EXP msdl2musicxmlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<msdl2musicxmlInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2xmlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2musicxmlInsiderOahGroup ();

    virtual               ~msdl2musicxmlInsiderOahGroup ();

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

    void                  printMsdl2xmlInsiderOahGroupHelp ();

    void                  printMsdl2xmlInsiderOahGroupValues (
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
    Bool                  fQuitAfterPass2;
    Bool                  fQuitAfterPass3;
    Bool                  fQuitAfterPass4;
    Bool                  fQuitAfterPass5;
};
typedef SMARTP<msdl2musicxmlInsiderOahGroup> S_msdl2musicxmlInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2musicxmlInsiderOahGroup& elt);

EXP extern S_msdl2musicxmlInsiderOahGroup gGlobalMsdl2xmlInsiderOahGroup;

//______________________________________________________________________________
S_msdl2musicxmlInsiderOahGroup createGlobalMsdl2xmlOahGroup ();


}


#endif // ___msdl2musicxmlInsiderHandler___
