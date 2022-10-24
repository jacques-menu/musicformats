/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlInputOah___
#define ___msdlInputOah___

#include "msrPitchesNames.h"

#include "msdlBasicTypes.h"
#include "msdlKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdlKeywordsLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlKeywordsLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlKeywordsLanguageKind&
                                              msdlKeywordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlKeywordsLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlKeywordsLanguageKind&
                                              msdlKeywordsLanguageKindVariable);

    virtual               ~msdlKeywordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsdlKeywordsLanguageKindVariable (
                            msdlKeywordsLanguageKind value)
                              {
                                fMsdlKeywordsLanguageKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msdlKeywordsLanguageKind&
                          fMsdlKeywordsLanguageKindVariable;
};
typedef SMARTP<msdlKeywordsLanguageAtom> S_msdlKeywordsLanguageAtom;
EXP ostream& operator << (ostream& os, const S_msdlKeywordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP oahDisplayMsdlKeywordsInLanguageAtom : public oahPureHelpAtomExpectingAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplayMsdlKeywordsInLanguageAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplayMsdlKeywordsInLanguageAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahDisplayMsdlKeywordsInLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahDisplayMsdlKeywordsInLanguageAtom> S_oahDisplayMsdlKeywordsInLanguageAtom;
EXP ostream& operator << (ostream& os, const S_oahDisplayMsdlKeywordsInLanguageAtom& elt);

//______________________________________________________________________________
class EXP oahDisplayMsdlTokensInLanguageAtom : public oahPureHelpAtomExpectingAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplayMsdlTokensInLanguageAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplayMsdlTokensInLanguageAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahDisplayMsdlTokensInLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahDisplayMsdlTokensInLanguageAtom> S_oahDisplayMsdlTokensInLanguageAtom;
EXP ostream& operator << (ostream& os, const S_oahDisplayMsdlTokensInLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdlCommentsTypeAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCommentsTypeAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlCommentsTypeKind&
                                              msdlCommentsTypeKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCommentsTypeAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlCommentsTypeKind&
                                              msdlCommentsTypeKindVariable);

    virtual               ~msdlCommentsTypeAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsdlCommentsTypeKindVariable (
                            msdlCommentsTypeKind value)
                              {
                                fMsdlCommentsTypeKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msdlCommentsTypeKind& fMsdlCommentsTypeKindVariable;
};
typedef SMARTP<msdlCommentsTypeAtom> S_msdlCommentsTypeAtom;
EXP ostream& operator << (ostream& os, const S_msdlCommentsTypeAtom& elt);

//______________________________________________________________________________
class EXP msdlUserLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlUserLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlUserLanguageKind&
                                              msdlUserLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlUserLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msdlUserLanguageKind&
                                              msdlUserLanguageKindVariable);

    virtual               ~msdlUserLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsdlUserLanguageKindVariable (
                            msdlUserLanguageKind value)
                              {
                                fMsdlUserLanguageVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msdlUserLanguageKind&
                          fMsdlUserLanguageVariable;
};
typedef SMARTP<msdlUserLanguageAtom> S_msdlUserLanguageAtom;
EXP ostream& operator << (ostream& os, const S_msdlUserLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdlPitchesLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlPitchesLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              msdlPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlPitchesLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              msdlPitchesLanguageKindVariable);

    virtual               ~msdlPitchesLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrQuarterTonesPitchesLanguageKindVariable (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fMsrQuarterTonesPitchesLanguageKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msdlPitchesLanguageAtom> S_msdlPitchesLanguageAtom;
EXP ostream& operator << (ostream& os, const S_msdlPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msdlInputOahGroup : public oahGroup
{
  public:

    static SMARTP<msdlInputOahGroup> create (
                            const string& serviceName);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlInputOahGroup (
                            const string& serviceName);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlInputOahGroup (
                            const string& serviceName);

    virtual               ~msdlInputOahGroup ();

    // set and get
    // ------------------------------------------------------

    // display
    // --------------------------------------

    void                  setDisplayMsdl ()
                              { fDisplayMsdl = true; }

    Bool                  getDisplayMsdl () const
                              { return fDisplayMsdl; }

    void                  setDisplayMsdlShort ()
                              { fDisplayMsdlShort = true; }

    Bool                  getDisplayMsdlShort () const
                              { return fDisplayMsdlShort; }

    msrQuarterTonesPitchesLanguageKind
                          getMsdlQuarterTonesPitchesLanguageKind () const
                              { return fMsdlQuarterTonesPitchesLanguageKind; }

  public:

    // public services
    // ------------------------------------------------------

    // languages
    Bool                  setMsdlQuarterTonesPitchesLanguage (string language);

    Bool                  setMsdlKeywordsLanguage (string language);

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

    void                  enforceQuietness (); // JMI

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdlInputOahHelp ();

    void                  printMsdlInputOahValues (int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

    void                  initializeMsdlDisplayOptions ();

    void                  initializeMsdlLanguagesOptions (
                            const string& serviceName);

  private:

    // private fields
    // --------------------------------------

    // display
    // --------------------------------------

    Bool                  fDisplayMsdl;
    Bool                  fDisplayMsdlShort;

    // keywords languages
    // --------------------------------------

    msdlKeywordsLanguageKind
                          fMsdlKeywordsLanguageKind;

    // pitches language
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fMsdlQuarterTonesPitchesLanguageKind;
};
typedef SMARTP<msdlInputOahGroup> S_msdlInputOahGroup;
EXP ostream& operator << (ostream& os, const S_msdlInputOahGroup& elt);

EXP extern S_msdlInputOahGroup gGlobalMsdlInputOahGroup;

//______________________________________________________________________________
EXP S_msdlInputOahGroup createGlobalMsdlInputOahGroup (
  const string& serviceName);


}


#endif
