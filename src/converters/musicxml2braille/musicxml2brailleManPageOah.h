/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2brailleManPageOah___
#define ___musicxml2brailleManPageOah___

#include "mfStaticSettings.h"

#include "musicxml2brailleOah2manPage.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP xml2brlManPageGenerateAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlManPageGenerateAtom> create (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            const S_oahVisitor& theOahVisitor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlManPageGenerateAtom (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            const S_oahVisitor& theOahVisitor);

    virtual               ~xml2brlManPageGenerateAtom ();

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
typedef SMARTP<xml2brlManPageGenerateAtom> S_xml2brlManPageGenerateAtom;
EXP std::ostream& operator << (std::ostream& os, const S_xml2brlManPageGenerateAtom& elt);

//______________________________________________________________________________
class EXP xml2brlManPageOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlManPageOahGroup> create (
                            const S_oahVisitor& theOah2manPage);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlManPageOahGroup (
                            const S_oahVisitor& theOah2manPage);

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

    void                  printManPageOahelp ();

    void                  printManPageOahValues (int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    S_oahVisitor
                          fOah2manPage;
};
typedef SMARTP<xml2brlManPageOahGroup> S_xml2brlManPageOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_xml2brlManPageOahGroup& elt);

EXP extern S_xml2brlManPageOahGroup gGlobalXml2brlManPageOahGroup;

//______________________________________________________________________________
EXP extern S_xml2brlManPageOahGroup createGlobalXml2brlManPageOahHandler (
  const S_oahVisitor& theOah2manPage);


}


#endif // ___musicxml2brailleManPageOah___
