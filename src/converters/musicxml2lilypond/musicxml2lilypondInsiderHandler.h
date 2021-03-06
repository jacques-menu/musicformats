/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __musicxml2lilypondInsiderHandler___
#define __musicxml2lilypondInsiderHandler___

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP xml2lyInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyInsiderHandler> create (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2lyInsiderHandler (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            oahHandlerUsedThruKind  handlerUsedThruKind);

    virtual               ~xml2lyInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;


    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2lyPrefixes ();

    void                  createTheXml2lyOptionGroups (
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
typedef SMARTP<xml2lyInsiderHandler> S_xml2lyInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyInsiderHandler& elt);

//______________________________________________________________________________
class EXP xml2lyInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2lyInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2lyInsiderOahGroup ();

    virtual               ~xml2lyInsiderOahGroup ();

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

    void                  setQuitAfterPass3 ()
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

    void                  printXml2lyInsiderOahGroupHelp ();

    void                  printXml2lyInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

    void                  createInsiderMacrosSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<xml2lyInsiderOahGroup> S_xml2lyInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2lyInsiderOahGroup& elt);

EXP extern S_xml2lyInsiderOahGroup gGlobalXml2lyInsiderOahGroup;

//______________________________________________________________________________
S_xml2lyInsiderOahGroup createGlobalXml2lyInsiderOahGroup ();


}


#endif
