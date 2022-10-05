/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2brailleManPageOah___
#define ___musicxml2brailleManPageOah___

#include "musicxml2brailleOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP xml2brlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlManPageGenerateAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlManPageGenerateAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

    virtual               ~xml2brlManPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<xml2brlManPageGenerateAtom> S_xml2brlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2brlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~xml2brlManPageOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeManPageGenerateOptions (
                            S_oahVisitor
                                 theOah2manPage);

#ifdef TRACING_IS_ENABLED
    void                  initializeManPagetracingOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printManPageOahelp ();

    void                  printManPageOahValues (int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor
                          fOah2manPage;
};
typedef SMARTP<xml2brlManPageOahGroup> S_xml2brlManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2brlManPageOahGroup& elt);

EXP extern S_xml2brlManPageOahGroup gGlobalXml2brlManPageOahGroup;

//______________________________________________________________________________
EXP extern S_xml2brlManPageOahGroup createGlobalXml2brlManPageOahHandler (
  S_oahVisitor theOah2manPage);


}


#endif
