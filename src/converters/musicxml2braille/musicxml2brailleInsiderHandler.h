/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2brailleInsiderHandler___
#define ___musicxml2brailleInsiderHandler___

#include "exports.h"

#include "oahAtomsCollection.h"

#include "oahInsiderHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP xml2brlInsiderHandler : public oahInsiderHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlInsiderHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlInsiderHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader);

    virtual               ~xml2brlInsiderHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeHandlerMultiComponent () override;

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2brlPrefixes ();

    void                  createTheXml2brlOptionGroups (
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
typedef SMARTP<xml2brlInsiderHandler> S_xml2brlInsiderHandler;
EXP std::ostream& operator << (std::ostream& os, const S_xml2brlInsiderHandler& elt);

//______________________________________________________________________________
class EXP xml2brlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2brlInsiderOahGroup> create (
                            const std::string&          serviceName,
                            const std::string&          handlerHeader);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlInsiderOahGroup (
                            const std::string&          serviceName,
                            const std::string&          handlerHeader);

    virtual               ~xml2brlInsiderOahGroup ();

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

    void                  printXml2brlInsiderOahGroupHelp ();

    void                  printXml2brlInsiderOahGroupValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // --------------------------------------

    // quit after some passes
    Bool                  fQuitAfterPass2a;
    Bool                  fQuitAfterPass2b;
    Bool                  fQuitAfterPass3;
};
typedef SMARTP<xml2brlInsiderOahGroup> S_xml2brlInsiderOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_xml2brlInsiderOahGroup& elt);

EXP extern S_xml2brlInsiderOahGroup gGlobalXml2brlInsiderOahGroup;

//______________________________________________________________________________
S_xml2brlInsiderOahGroup createGlobalXml2brlOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader);


}


#endif
