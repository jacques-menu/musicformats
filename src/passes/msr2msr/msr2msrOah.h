/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2msrOah___
#define ___msr2msrOah___


#include "oahBasicTypes.h"

#include "lpsrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrIgnorePartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrIgnorePartAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrIgnorePartAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

    virtual               ~msrIgnorePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const string&  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

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

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrIgnorePartAtom> S_msrIgnorePartAtom;
EXP ostream& operator<< (ostream& os, const S_msrIgnorePartAtom& elt);

//______________________________________________________________________________
class EXP msrKeepPartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrKeepPartAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrKeepPartAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

    virtual               ~msrKeepPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const string&  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

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

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrKeepPartAtom> S_msrKeepPartAtom;
EXP ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt);

//______________________________________________________________________________
class EXP msr2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2msrOahGroup ();

    virtual               ~msr2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // avoid msr2msr? (for TESTS)
    Bool                  getAvoidMsr2msr () const
                              { return fAvoidMsr2msr; }

    // book
    Bool                  getExpandToHarmonyBandBook () const
                              { return fExpandToHarmonyBandBook; }

    // page breaks
    const set<string>&    getInserPageBreakAfterMeasureSet () const
                              { return fInserPageBreakAfterMeasureSet; }
    S_oahStringSetAtom    getInserPageBreakAfterMeasureAtom () const
                              { return fInserPageBreakAfterMeasureAtom; }

    // compress
    void                  setCoalesceEmptyMeasures ()
                              { fCoalesceEmptyMeasures = true; }
    Bool                  getCoalesceEmptyMeasures () const
                              { return fCoalesceEmptyMeasures; }

    Bool                  getMergeRests () const
                              { return fMergeRests; }

    // barLines
    Bool                  getCreateImplicitInitialRepeatBarLine () const
                              { return fCreateImplicitInitialRepeatBarLine; }

    // convert words to tempo
    Bool                  getConvertMsrWordsToTempo () const
                              { return fConvertMsrWordsToTempo; }
    // add words from the lyrics
    Bool                  getAddMsrWordsFromTheMsrLyrics () const
                              { return fAddMsrWordsFromTheMsrLyrics; }

    // convert MSR words to rehearsal marks
    Bool                  getConvertMsrWordsToRehearsalMarks () const
                              { return fConvertMsrWordsToRehearsalMarks; }

    // convert tempos to rehearsal marks
    Bool                  getConvertTemposToRehearsalMarks () const
                              { return fConvertTemposToRehearsalMarks; }
  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializMsr2msrTraceOptions ();
#endif

    void                  initializeBookOptions ();

    void                  initializePageBreakOptions ();

    void                  initializeCompressOptions ();

    void                  initializeRepeatsOptions ();

    void                  initializeNotesOptions ();

    void                  initializeLyricsOptions ();

    void                  initializeTemposOptions ();

    void                  initializeWordsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2msrOahHelp ();

    void                  printMsr2msrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // avoid msr2msr? (for TESTS)
    Bool                  fAvoidMsr2msr;

    // book
    Bool                  fExpandToHarmonyBandBook;

    // page breaks
    set<string>           fInserPageBreakAfterMeasureSet;
    S_oahStringSetAtom    fInserPageBreakAfterMeasureAtom;

    // compress
    Bool                  fCoalesceEmptyMeasures;

    Bool                  fMergeRests;

    // repeats
    Bool                  fCreateImplicitInitialRepeatBarLine;

    // rehearsal marks
    Bool                  fConvertTemposToRehearsalMarks;

    // convert MSR words to tempo
    Bool                  fConvertMsrWordsToTempo;
    // add MSR words from the lyrics
    Bool                  fAddMsrWordsFromTheMsrLyrics;

    // convert MSR words to rehearsal marks
    Bool                  fConvertMsrWordsToRehearsalMarks;
};
typedef SMARTP<msr2msrOahGroup> S_msr2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2msrOahGroup& elt);

EXP extern S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

//______________________________________________________________________________
EXP S_msr2msrOahGroup createGlobalMsr2msrOahGroup ();


}


#endif
