/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrOah___
#define ___bsrOah___

#include "exports.h"

#include "bsrEnumTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class bsrFacSimileKind {
  kFacSimileYes, kFacSimileNo
};

string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind);

ostream& operator << (ostream& os, const bsrFacSimileKind& elt);

//______________________________________________________________________________
class EXP bsrFacSimileKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFacSimileKindAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            bsrFacSimileKind  bsrFacSimileKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrFacSimileKindAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            bsrFacSimileKind  bsrFacSimileKindVariable);

    virtual               ~bsrFacSimileKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsFacSimileKindVariable (
                            bsrFacSimileKind value)
                              { fBsrFacSimileKindVariable = value; }

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

    bsrFacSimileKind      fBsrFacSimileKindVariable;
};
typedef SMARTP<bsrFacSimileKindAtom> S_bsrFacSimileKindAtom;
EXP ostream& operator << (ostream& os, const S_bsrFacSimileKindAtom& elt);

//______________________________________________________________________________
class EXP bsrTextsLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTextsLanguageAtom> create (
                            const string&  longName,
                            const string&  shortName,
                            const string&  description,
                            const string&  valueSpecification,
                            const string&  variableName,
                            bsrTextsLanguageKind&
                                           bsrTextsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTextsLanguageAtom (
                            const string&  longName,
                            const string&  shortName,
                            const string&  description,
                            const string&  valueSpecification,
                            const string&  variableName,
                            bsrTextsLanguageKind&
                                           bsrTextsLanguageKindVariable);

    virtual               ~bsrTextsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrTextsLanguageKindVariable (
                            bsrTextsLanguageKind value)
                              {
                                fBsrTextsLanguageKindVariable = value;
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

    bsrTextsLanguageKind& fBsrTextsLanguageKindVariable;
};
typedef SMARTP<bsrTextsLanguageAtom> S_bsrTextsLanguageAtom;
EXP ostream& operator << (ostream& os, const S_bsrTextsLanguageAtom& elt);

//______________________________________________________________________________
class EXP bsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrOahGroup ();

    virtual               ~bsrOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    Bool                  setBsrTextsLanguage (
                            const string& language);

    // display

    void                  setDisplayFirstBsrShort ()
                              { fDisplayFirstBsrShort = true; }
    Bool                  getDisplayFirstBsrShort  () const
                              { return fDisplayFirstBsrShort; }

    void                  setDisplayFirstBsrFull ()
                              { fDisplayFirstBsrFull = true; }
    Bool                  getDisplayFirstBsrFull () const
                              { return fDisplayFirstBsrFull; }

    void                  setDisplaySecondBsrShort ()
                              { fDisplaySecondBsrShort = true; }
    Bool                  getDisplaySecondBsrShort () const
                              { return fDisplaySecondBsrShort; }

    void                  setDisplaySecondBsrFull ()
                              { fDisplaySecondBsrFull = true; }
    Bool                  getDisplaySecondBsrFull () const
                              { return fDisplaySecondBsrFull; }

    Bool                  getDisplayBsrShort () const
                              {
                                return
                                  fDisplayFirstBsrShort
                                    ||
                                 fDisplaySecondBsrShort ;
                              }

    Bool                  getDisplayBsrFull () const
                              {
                                return
                                  fDisplayFirstBsrFull
                                    ||
                                  fDisplaySecondBsrFull;
                              }

    // languages

    bsrTextsLanguageKind  getBsrTextsLanguageKind () const // JMI
                              { return fBsrTextsLanguageKind; }

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    // BSR
    void                  setTraceBsr ()
                              { fTraceBsr = true; }
    Bool                  getTraceBsr () const
                              { return fTraceBsr; }

    // pages & lines
    void                  setTracePages ()
                              { fTracePages = true; }
    Bool                  getTracePages () const
                              { return fTracePages; }

    void                  setTraceLines ()
                              { fTraceLines = true; }
    Bool                  getTraceLines () const
                              { return fTraceLines; }

    void                  setTraceSpaces ()
                              { fTraceSpaces = true; }
    Bool                  getTraceSpaces () const
                              { return fTraceSpaces; }

    void                  setTraceNumbers ()
                              { fTraceNumbers = true; }
    Bool                  getTraceNumbers () const
                              { return fTraceNumbers; }

    // parallels
    void                  setTraceParallels ()
                              { fTraceParallels = true; }
    Bool                  getTraceParallels () const
                              { return fTraceParallels; }

    // visitors
    void                  setTraceBsrVisitors ()
                              { fTraceBsrVisitors = true; }
    Bool                  getTraceBsrVisitors () const
                              { return fTraceBsrVisitors; }
#endif

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

    void                  initializeBsrDisplayOptions ();

    void                  initializeBsrLanguagesOptions ();

#ifdef TRACING_IS_ENABLED
    void                  initializeBsrtracingOah ();
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

    void                  printBsrOahHelp ();

    void                  printBsrOahValues (int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    // display

    Bool                  fDisplayFirstBsrShort;
    Bool                  fDisplayFirstBsrFull;

    Bool                  fDisplaySecondBsrShort;
    Bool                  fDisplaySecondBsrFull;

    // languages

    bsrTextsLanguageKind  fBsrTextsLanguageKind;

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED

    // BSR
    Bool                  fTraceBsr;

    // pages & lines
    Bool                  fTracePages;

    Bool                  fTraceLines;

    Bool                  fTraceSpaces;
    Bool                  fTraceNumbers;

    // parallels
    Bool                  fTraceParallels;

    // visitors
    Bool                  fTraceBsrVisitors;
#endif
};
typedef SMARTP<bsrOahGroup> S_bsrOahGroup;
EXP ostream& operator << (ostream& os, const S_bsrOahGroup& elt);

EXP extern S_bsrOahGroup gGlobalBsrOahGroup;

//______________________________________________________________________________
EXP  S_bsrOahGroup createGlobalBsrOahGroup ();


}


#endif
