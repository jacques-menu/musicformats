/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __musicxml2lilypondInsiderHandler___
#define __musicxml2lilypondInsiderHandler___

#include "mfPreprocessorSettings.h"

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
                            const std::string&     serviceName,
                            const std::string&     handlerHeader,
                            oahHandlerUsedThruKind handlerUsedThruKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2lyInsiderHandler (
                            const std::string&     serviceName,
                            const std::string&     handlerHeader,
                            oahHandlerUsedThruKind handlerUsedThruKind);

    virtual               ~xml2lyInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;


    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2lyPrefixes ();

    void                  createTheXml2lyOptionGroups (
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

    // private methods
    // ------------------------------------------------------

    std::string           usageFromUsedThruKind (
                            oahHandlerUsedThruKind handlerUsedThruKind) const;
  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;
};
typedef SMARTP<xml2lyInsiderHandler> S_xml2lyInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_xml2lyInsiderHandler& elt);

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
    Bool                  fQuitAfterPass2;
    Bool                  fQuitAfterPass3;
    Bool                  fQuitAfterPass4;
    Bool                  fQuitAfterPass5;
};
typedef SMARTP<xml2lyInsiderOahGroup> S_xml2lyInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_xml2lyInsiderOahGroup& elt);

EXP extern S_xml2lyInsiderOahGroup gGlobalXml2lyInsiderOahGroup;

//______________________________________________________________________________
S_xml2lyInsiderOahGroup createGlobalXml2lyInsiderOahGroup ();


}


#endif // __musicxml2lilypondInsiderHandler___
