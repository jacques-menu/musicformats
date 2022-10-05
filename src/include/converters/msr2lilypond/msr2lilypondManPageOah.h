/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lilypondManPageOah___
#define ___msr2lilypondManPageOah___

#include "oahVisitor.h"

#include "msr2lilypondOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2lilypondManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondManPageGenerateAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondManPageGenerateAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

    virtual               ~msr2lilypondManPageGenerateAtom ();

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
typedef SMARTP<msr2lilypondManPageGenerateAtom> S_msr2lilypondManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msr2lilypondManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lilypondManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msr2lilypondManPageOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

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

    void                  printManPageOahHelp ();

    void                  printManPageOahValues (int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<msr2lilypondManPageOahGroup> S_msr2lilypondManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2lilypondManPageOahGroup& elt);

EXP extern S_msr2lilypondManPageOahGroup gGlobalMsr2lilypondManPageOahGroup;

//______________________________________________________________________________
EXP S_msr2lilypondManPageOahGroup createGlobalMsr2lilypondManPageOahGroup (
  S_oahVisitor theOah2manPage);


}


#endif
