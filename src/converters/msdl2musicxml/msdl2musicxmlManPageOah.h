/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2musicxmlManPageOah___
#define ___msdl2musicxmlManPageOah___

#include "oahVisitor.h"

#include "msdl2musicxmlOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdl2musicxmlManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2musicxmlManPageGenerateAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2musicxmlManPageGenerateAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            S_oahVisitor  theOahVisitor);

    virtual               ~msdl2musicxmlManPageGenerateAtom ();

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
typedef SMARTP<msdl2musicxmlManPageGenerateAtom> S_msdl2musicxmlManPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_msdl2musicxmlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2musicxmlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2musicxmlManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2xmlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2musicxmlManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msdl2musicxmlManPageOahGroup ();

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
typedef SMARTP<msdl2musicxmlManPageOahGroup> S_msdl2musicxmlManPageOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2musicxmlManPageOahGroup& elt);

EXP extern S_msdl2musicxmlManPageOahGroup gGlobalMsdl2xmlManPageOahGroup;

//______________________________________________________________________________
EXP S_msdl2musicxmlManPageOahGroup createGlobalMsdl2xmlManPageOahGroupHandling (
  S_oahHandler handler,
  S_oahVisitor theOah2manPage);


}


#endif
