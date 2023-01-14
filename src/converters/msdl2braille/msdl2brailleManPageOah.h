/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2brailleManPageOah___
#define ___msdl2brailleManPageOah___

#include "msdl2brailleOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdl2brailleManPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brailleManPageGenerateAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brailleManPageGenerateAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

    virtual               ~msdl2brailleManPageGenerateAtom ();

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
                            int           valueFieldWidth) const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (std::ostream& os) const;

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor          fOahVisitor;
};
typedef SMARTP<msdl2brailleManPageGenerateAtom> S_msdl2brailleManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2brailleManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2brailleManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brailleManPageOahGroup> create (
                            const S_oahVisitor& theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2brlManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brailleManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

    virtual               ~msdl2brailleManPageOahGroup ();

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
                            const S_oahVisitor&
                                 theOah2manPage);

#ifdef MF_TRACING_IS_ENABLED
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
typedef SMARTP<msdl2brailleManPageOahGroup> S_msdl2brailleManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2brailleManPageOahGroup& elt);

EXP extern S_msdl2brailleManPageOahGroup gGlobalMsdl2brlManPageOahGroup;

//______________________________________________________________________________
EXP extern S_msdl2brailleManPageOahGroup createGlobalMsdl2brlManPageOahHandler (
  const S_oahVisitor& theOah2manPage);


}


#endif
