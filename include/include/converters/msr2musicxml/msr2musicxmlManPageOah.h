/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2musicxmlManPageOah___
#define ___msr2musicxmlManPageOah___

#include "oahVisitor.h"

#include "msr2musicxmlOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2musicxmlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2musicxmlManPageGenerateAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2musicxmlManPageGenerateAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

    virtual               ~msr2musicxmlManPageGenerateAtom ();

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
typedef SMARTP<msr2musicxmlManPageGenerateAtom> S_msr2musicxmlManPageGenerateAtom;
EXP ostream& operator << (ostream& os, const S_msr2musicxmlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msr2musicxmlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2musicxmlManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2musicxmlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2musicxmlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msr2musicxmlManPageOahGroup ();

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
typedef SMARTP<msr2musicxmlManPageOahGroup> S_msr2musicxmlManPageOahGroup;
EXP ostream& operator << (ostream& os, const S_msr2musicxmlManPageOahGroup& elt);

EXP extern S_msr2musicxmlManPageOahGroup gGlobalMsr2musicxmlManPageOahGroup;

//______________________________________________________________________________
EXP S_msr2musicxmlManPageOahGroup createGlobalMsr2musicxmlManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPage);


}


#endif
