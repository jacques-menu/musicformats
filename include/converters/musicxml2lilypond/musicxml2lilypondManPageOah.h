/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2lilypondManPageOah___
#define ___musicxml2lilypondManPageOah___

#include "mfPreprocessorSettings.h"

#include "oahVisitor.h"

#include "musicxml2lilypondManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP xml2lyManPageGenerateAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyManPageGenerateAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2lyManPageGenerateAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const S_oahVisitor&  theOahVisitor);

    virtual               ~xml2lyManPageGenerateAtom ();

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
typedef SMARTP<xml2lyManPageGenerateAtom> S_xml2lyManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_xml2lyManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2lyManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyManPageOahGroup> create (
                            const S_oahVisitor& theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2lyManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

    virtual               ~xml2lyManPageOahGroup ();

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
typedef SMARTP<xml2lyManPageOahGroup> S_xml2lyManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_xml2lyManPageOahGroup& elt);

EXP extern S_xml2lyManPageOahGroup gGlobalXml2lyManPageOahGroup;

//______________________________________________________________________________
EXP S_xml2lyManPageOahGroup createGlobalXml2lyManPageOahGroup (
  const S_oahVisitor& theOah2manPage);


}


#endif // ___musicxml2lilypondManPageOah___
