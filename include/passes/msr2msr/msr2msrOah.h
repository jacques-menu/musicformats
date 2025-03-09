/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2msrOah___
#define ___msr2msrOah___

#include "mfPreprocessorSettings.h"

#include "oahBasicTypes.h"

#include "lpsrEnumTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrIgnorePartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrIgnorePartAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set <std::string>&  stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrIgnorePartAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set <std::string>&  stringSetVariable);

    virtual               ~msrIgnorePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const std::string&  partName)
                              { fStringSetVariable.insert (partName); }

    const std::set <std::string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set <std::string>&          fStringSetVariable;
};
typedef SMARTP<msrIgnorePartAtom> S_msrIgnorePartAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrIgnorePartAtom& elt);

//______________________________________________________________________________
class EXP msrKeepPartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrKeepPartAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set <std::string>&  stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrKeepPartAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set <std::string>&  stringSetVariable);

    virtual               ~msrKeepPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const std::string&  partName)
                              { fStringSetVariable.insert (partName); }

    const std::set <std::string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set <std::string>&          fStringSetVariable;
};
typedef SMARTP<msrKeepPartAtom> S_msrKeepPartAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrKeepPartAtom& elt);

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

    // voices
    const std::set <std::string>&    getIgnoreMsrVoicesSet () const
                              { return fIgnoreMsrVoicesSet; }
    S_oahStringSetElementAtom
                          getIgnoreMsrVoicesSetAtom () const
                              { return fIgnoreMsrVoicesSetAtom; }

    const std::set <std::string>&    getKeepMsrVoicesSet () const
                              { return fKeepMsrVoicesSet; }
    S_oahStringSetElementAtom
                          getKeepMsrVoicesSetAtom () const
                              { return fKeepMsrVoicesSetAtom; }

    // page and line breaks
    const std::set <std::string>&    getInserPageBreakAfterMeasureSet () const
                              { return fInserPageBreakAfterMeasureSet; }
    S_oahStringSetElementAtom
                          getInserPageBreakAfterMeasureAtom () const
                              { return fInserPageBreakAfterMeasureAtom; }

    // compress
    void                  setCoalesceEmptyMeasures ()
                              { fCoalesceEmptyMeasures = true; }
    Bool                  getCoalesceEmptyMeasures () const
                              { return fCoalesceEmptyMeasures; }

    Bool                  getMergeStaffCommonRests () const
                              { return fMergeStaffCommonRests; }

    // bar lines
    Bool                  getCreateImplicitInitialRepeatBarLine () const
                              { return fCreateImplicitInitialRepeatBarLine; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    void                  initializMsr2msrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializMsr2msrBookOptions ();

    void                  initializMsr2msrStavesOptions ();

    void                  initializMsr2msrVoicesOptions ();

    void                  initializMsr2msrBreakOptions ();

    void                  initializMsr2msrCompressOptions ();

    void                  initializMsr2msrRepeatsOptions ();

    void                  initializMsr2msrNotesOptions ();

    void                  initializMsr2msrLyricsOptions ();

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

    void                  displayMsr2msrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // avoid msr2msr? (for TESTS JMI ???)
    Bool                  fAvoidMsr2msr;

    // book
    Bool                  fExpandToHarmonyBandBook;

    // staves
    std::set <std::string>
                          fIgnoreMsrStavesSet;
    S_oahStringSetElementAtom
                          fIgnoreMsrStavesSetAtom;

    std::set <std::string>
                          fKeepMsrStavesSet;
    S_oahStringSetElementAtom
                          fKeepMsrStavesSetAtom;

    // voices
    std::set <std::string>
                          fIgnoreMsrVoicesSet;
    S_oahStringSetElementAtom
                          fIgnoreMsrVoicesSetAtom;

    std::set <std::string>
                          fKeepMsrVoicesSet;
    S_oahStringSetElementAtom
                          fKeepMsrVoicesSetAtom;

    // breaks
    std::set <std::string>
                          fInserPageBreakAfterMeasureSet;
    S_oahStringSetElementAtom
                          fInserPageBreakAfterMeasureAtom;

    std::set <std::string> fInserLineBreakAfterMeasureSet;
    S_oahStringSetElementAtom
                          fInserLineBreakAfterMeasureAtom;

    // compress
    Bool                  fCoalesceEmptyMeasures;

    Bool                  fMergeStaffCommonRests;

    // repeats
    Bool                  fCreateImplicitInitialRepeatBarLine;
};
typedef SMARTP<msr2msrOahGroup> S_msr2msrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2msrOahGroup& elt);

EXP extern S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

//______________________________________________________________________________
EXP S_msr2msrOahGroup createGlobalMsr2msrOahGroup ();


}


#endif // ___msr2msrOah___
