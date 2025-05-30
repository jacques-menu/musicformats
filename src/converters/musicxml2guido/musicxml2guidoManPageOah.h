/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2guidoManPageOah___
#define ___musicxml2guidoManPageOah___

#include "mfPreprocessorSettings.h"

#include "oahVisitor.h"

#include "musicxml2guidoOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP xml2gmnManPageGenerateAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2gmnManPageGenerateAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2gmnManPageGenerateAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

    virtual               ~xml2gmnManPageGenerateAtom ();

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
typedef SMARTP<xml2gmnManPageGenerateAtom> S_xml2gmnManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_xml2gmnManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2gmnManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2gmnManPageOahGroup> create (
                            const S_oahVisitor& theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2gmnManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2gmnManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

    virtual               ~xml2gmnManPageOahGroup ();

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
typedef SMARTP<xml2gmnManPageOahGroup> S_xml2gmnManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_xml2gmnManPageOahGroup& elt);

EXP extern S_xml2gmnManPageOahGroup gGlobalXml2gmnManPageOahGroup;

//______________________________________________________________________________
EXP S_xml2gmnManPageOahGroup createGlobalXml2gmnManPageOahGroupHandling (
  const S_oahHandler& handler,
  const S_oahVisitor& theOah2manPage);


}


#endif // ___musicxml2guidoManPageOah___
