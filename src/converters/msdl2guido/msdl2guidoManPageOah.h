/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2guidoManPageOah___
#define ___msdl2guidoManPageOah___

#include "mfPreprocessorSettings.h"

#include "oahVisitor.h"

#include "msdl2guidoOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdl2guidoManPageGenerateAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2guidoManPageGenerateAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2guidoManPageGenerateAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

    virtual               ~msdl2guidoManPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  displayAtomWithVariableOptionsValues (
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
typedef SMARTP<msdl2guidoManPageGenerateAtom> S_msdl2guidoManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2guidoManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP msdl2guidoManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2guidoManPageOahGroup> create (
                            const S_oahVisitor& theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2gmnManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2guidoManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

    virtual               ~msdl2guidoManPageOahGroup ();

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
                            const S_oahVisitor&
                                 theOah2manPage);

#ifdef MF_TRACE_IS_ENABLED
    void                  initializeManPageTraceOah ();
#endif // MF_TRACE_IS_ENABLED

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
typedef SMARTP<msdl2guidoManPageOahGroup> S_msdl2guidoManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2guidoManPageOahGroup& elt);

EXP extern S_msdl2guidoManPageOahGroup gGlobalMsdl2gmnManPageOahGroup;

//______________________________________________________________________________
EXP S_msdl2guidoManPageOahGroup createGlobalMsdl2gmnManPageOahGroupHandling (
  const S_oahHandler& handler,
  const S_oahVisitor& theOah2manPage);


}


#endif // ___msdl2guidoManPageOah___
