/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2lilypondManPageOah___
#define ___msdl2lilypondManPageOah___

#include "oahVisitor.h"

#include "msdl2lilypondOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdl2lilypondManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lilypondManPageGenerateAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            S_oahVisitor  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lilypondManPageGenerateAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            S_oahVisitor  theOahVisitor);

    virtual               ~msdl2lilypondManPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (std::ostream& os) const;

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<msdl2lilypondManPageGenerateAtom> S_msdl2lilypondManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2lilypondManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2lilypondManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lilypondManPageOahGroup> create (
                            S_oahVisitor theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2lyManPageOahGroup (
                            S_oahVisitor theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lilypondManPageOahGroup (
                            S_oahVisitor theOah2manPage);

    virtual               ~msdl2lilypondManPageOahGroup ();

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
typedef SMARTP<msdl2lilypondManPageOahGroup> S_msdl2lilypondManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2lilypondManPageOahGroup& elt);

EXP extern S_msdl2lilypondManPageOahGroup gGlobalMsdl2lyManPageOahGroup;

//______________________________________________________________________________
EXP S_msdl2lilypondManPageOahGroup createGlobalMsdl2lyManPageOahGroup (
  S_oahVisitor theOah2manPage);


}


#endif
